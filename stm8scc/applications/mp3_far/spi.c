#include <stm8s105x6.h>
#include "define.h"
#include "spi.h"

///////////////////////////////////////////////////////////
void SPI_init(void)
{
	SPI_CONFIG;
	SPI_SCK_LO;

    SD_CS_CFG_DDR;
    SD_DISABLE;

    SD_SCK_UNHOLD;

	VS1011_ICS_CFG_DDR;
	VS1011_DISABLE_INST;

	VS1011_DCS_CFG_DDR;
	VS1011_DISABLE_DATA;
}

///////////////////////////////////////////////////////////
void SPI_write (unsigned char data)
{
    char cnt = 8;
    // transfer rate = 8MHz/2 = 4Mbit
    do
    {
		SPI_MOSI_LO;
		if ( data & 0x80 )
		{
			SPI_MOSI_HI;
			asm("nop");
		}
		SPI_SCK_HI;
		data <<= 1;
		SPI_SCK_LO;
	} while ( --cnt );
}

///////////////////////////////////////////////////////////
unsigned char SPI_read (void)
{
    SPI_MOSI_HI;

    unsigned char cnt = 8, data;

    do
    {
		SPI_SCK_HI;
        data <<= 1;
		if ( SPI_MISO_HI )
			data++;

		SPI_SCK_LO;
	} while ( --cnt );

	return data;
}

///////////////////////////////////////////////////////////
void ENABLE_SPI2 (void)
{
//	SD_SCK_HOLD;				// SD card SCK = 0
	sbi (PC_DDR, SPI_MISO_PIN);	// set MISO as output
	sbi (PC_CR1, SPI_MISO_PIN);
}

///////////////////////////////////////////////////////////
void DISABLE_SPI2 (void)
{
//	SD_SCK_UNHOLD;				// SD card SCK = 0
	cbi (PC_DDR, SPI_MISO_PIN);	// set MISO as input
}

///////////////////////////////////////////////////////////
void SPI2_write (unsigned char data)
{
    char cnt = 8;
    // transfer rate = 8MHz/2 = 4Mbit
    do
    {
		SPI2_MOSI_LO;
		if ( data & 0x80 )
			SPI2_MOSI_HI;

		SPI_SCK_HI;
		data <<= 1;
		SPI_SCK_LO;
	} while ( --cnt );
}

///////////////////////////////////////////////////////////
void SPI_burst (void)
{
#if 0
	unsigned char i = 32;
	while ( i-- )
		SPI_write (0xff);
#else
    SPI_MOSI_HI;
    unsigned char i = 0;
    do
    {
        SPI_SCK_HI; _NOP_;	_NOP_;
		SPI_SCK_LO;
    } while ( --i );
#endif
}

