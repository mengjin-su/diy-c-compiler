//*****************************************************************************
//
// 	Name:           MMC.C - MultiMediaCard driver
//  Authors:        Mengjin Su
//
//*****************************************************************************
#include <stm8s105x6.h>
#include "define.h"
#include "mmcspi.h"
#include "spi.h"
#include "fat.h"

/* --- MMC response --- */
#define GOOD_R1_RESPONSE 0x00
#define MMC_MAX_RETRY   10

/* --- response token bits --- */
#define PAR_ERR         0x4000   // parameter error
#define ADDR_ERR        0x2000   // address error
#define ERASE_SEQ_ERR   0x1000   // erase seq error
#define COMM_CRC_ERR    0x0800   // comm CRC error
#define ILLEGAL_CMD     0x0400   // illegal command
#define ERASE_RESET     0x0200   // erase reset
#define IN_IDLE_STATE   0x0100   // in idle state
#define OUT_OF_RANGE    0x0080   // out of range
#define ERASE_PAR       0x0040   // erase parameter
#define WP_VIOLATION    0x0020   // WP violation
#define CARD_ECC_FAIL   0x0010   // card ECC failed
#define CC_ERR          0x0008   // CC error
#define GEN_ERR         0x0004   // (generic) error
#define WP_ERASE_SKIP   0x0002   // WP erase skip

static void MMC_endAck (void);

//////////////////////////////////////////////////////////////////////////////
static unsigned char MMC_ExecCmd (unsigned char cmd, unsigned long par)
{
    unsigned char stat, ret, crc, loop;
    unsigned int  cnt;

	loop = 0;
	do
	{
		ret = 0x00;
		crc = 0xff;

		if ( cmd == GO_IDLE_STATE )	//	CMD0
		{
			ret = 0x01;
			crc = 0x95;
			DISABLE_SD_CARD;		//	SPI_CS_HI;	// SS = hi

			for (cnt = 300; cnt--;)
				SPI_write (0xff);
		}

		ENABLE_SD_CARD;				// SPI_CS_LO;	// SS = lo
		char *p = &par;
		SPI_write(cmd | 0x40);
		SPI_write(p[0]);	// MSB
		SPI_write(p[1]);
		SPI_write(p[2]);
		SPI_write(p[3]);	// LSB
		SPI_write(crc);

		cnt = 1000;
		while ( SPI_read() != ret && cnt ) cnt--;

		stat = MMC_ERR;
		if ( cnt )
		{
			stat = MMC_OK;

			if ( cmd == READ_SINGLE_BLOCK )
			{
				cnt = 1000;
				while ( SPI_read () != 0xfe && cnt ) cnt--;

				if ( cnt == 0 )
					stat = MMC_ERR;
				else
					return MMC_OK;
			}
		}

		DISABLE_SD_CARD;	// SPI_CS_HI;
		SPI_write (0xff);
		++loop;
	} while ( stat != MMC_OK && loop < 5 );

	return stat;
}


//////////////////////////////////////////////////////////////////////////////
char MMC_Init (void)
{
    if ( MMC_ExecCmd(GO_IDLE_STATE, 0) != MMC_OK || // - CMD0
         MMC_ExecCmd(SEND_OP_COND,  0) != MMC_OK )  // - CMD1
	{
        return MMC_NO_RESP;
	}

	return MMC_ExecCmd(SET_BLOCKLEN, 512);
}

//////////////////////////////////////////////////////////////////////////////
char MMC_ChkStatus (void)
{
  	return MMC_ExecCmd(SEND_STATUS, 0);
}


//////////////////////////////////////////////////////////////////////////////
char MMC_ReadSegment (unsigned long sect, unsigned char seg)
{
	if ( seg == 0 )
	{
		DISABLE_SD_CARD;
		if ( MMC_ExecCmd (READ_SINGLE_BLOCK, MMC_SECTOR_SIZE*sect) != MMC_OK )
			return MMC_ERR;
	}

	unsigned char *buf = BUFFER;
    unsigned char cnt = SEGMENT_SIZE;    // buffer size = 32 bytes (per segment)

	do
	{   *buf++ = SPI_read ();
	} while ( --cnt );

	// if it's the last segment in a sector ...
	if ( seg >= (MMC_SECTOR_SIZE/SEGMENT_SIZE - 1) )
		MMC_endAck ();

	return MMC_OK;
}

//////////////////////////////////////////////////////////////////////////////
char MMC_SkipSegment (unsigned long sect, unsigned char seg)
{
	if ( seg < (MMC_SECTOR_SIZE/SEGMENT_SIZE) )
	{
		DISABLE_SD_CARD;
		if ( MMC_ExecCmd (READ_SINGLE_BLOCK, MMC_SECTOR_SIZE*sect) != MMC_OK )
			return MMC_ERR;

	    seg <<= 4;    // buffer size = 32 bytes (per segment)

		do
		{   SPI_read ();
			SPI_read ();
		} while ( --seg );
	}
	return MMC_OK;
}

//////////////////////////////////////////////////////////////////////////////
char MMC_ReadSegOffset (unsigned long sect, unsigned int offset)
{
	DISABLE_SD_CARD;

    if ( MMC_ExecCmd (READ_SINGLE_BLOCK, MMC_SECTOR_SIZE*sect) != MMC_OK )
        return MMC_ERR;

	unsigned char *buf = BUFFER;
    unsigned int sect_size = MMC_SECTOR_SIZE;  // 512
	unsigned int length = SEGMENT_SIZE;

    do
    {
        char data = SPI_read ();

        if ( offset )
        {
            offset--;
        }
        else if ( length )
        {
            length--;
            *buf++ = data;
        }
    } while ( --sect_size );

	MMC_endAck ();
    return MMC_OK;
}

//////////////////////////////////////////////////////////////////////////////
char MMC_ReadSegOffsetLength (unsigned long sect, unsigned int offset, unsigned char length)
{
	DISABLE_SD_CARD;

    if ( MMC_ExecCmd (READ_SINGLE_BLOCK, MMC_SECTOR_SIZE*sect) != MMC_OK )
        return MMC_ERR;

	unsigned char *buf = BUFFER;
    unsigned int sect_size = MMC_SECTOR_SIZE;  // 512

    do
    {
        char data = SPI_read ();

        if ( offset )
        {
            offset--;
        }
        else if ( length )
        {
            length--;
            *buf++ = data;
        }
    } while ( --sect_size );

	MMC_endAck ();
    return MMC_OK;
}

//////////////////////////////////////////////////////////////////////////////
char MMC_NextSector (unsigned long sector)
{
	DISABLE_SD_CARD;

    if ( MMC_ExecCmd (READ_SINGLE_BLOCK, MMC_SECTOR_SIZE*sector) == MMC_OK )
        return MMC_OK;

	DISABLE_SD_CARD;

	SPI_write (0xff);
    return MMC_ERR;
}

///////////////////////////////////////////////////////////////////////////////
void MMC_dumpSegment (unsigned char seg)
{
	VS1011_ENABLE_DATA;
    SPI_burst();	// read 32 bytes from SD (and write to VS1011)
    VS1011_DISABLE_DATA;

	// if it's the last segment in a sector ...
    if ( seg >= (MMC_SECTOR_SIZE/SEGMENT_SIZE-1) )
    	MMC_endAck ();
}

///////////////////////////////////////////////////////////////////////////////
void MMC_flushSector (unsigned char seg)
{
	if ( seg >= (MMC_SECTOR_SIZE/SEGMENT_SIZE) )
		return;

	do
	{
		SPI_burst();		// read 32 bytes from SD

	} while ( ++seg < (MMC_SECTOR_SIZE/SEGMENT_SIZE) );

	MMC_endAck ();
}

///////////////////////////////////////////////////////////////////////////////
static void MMC_endAck (void)
{
    SPI_read(); // read sector CRC16
    SPI_read();

    DISABLE_SD_CARD;
    SPI_write (0xff);
}
