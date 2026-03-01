//*****************************************************************************
// Name:           MMC.H - MultiMediaCard driver head file
//
//  Authors:        Mengjin Su
//
//*****************************************************************************
#ifndef _MMC_H
#define _MMC_H

#include "fat.h"

/* --- following MMC commands are available in SPI mode --- */
#define GO_IDLE_STATE            0  // CMD0  - Resets the MultiMediaCard
#define SEND_OP_COND             1  // CMD1  - Activates the cards initialization
#define ALL_SEND_CID             2  // CMD2 - Asks all cards to send their CID
#define SET_RCA                  3  // CMD3  - Assigns relative card address
#define SEL_CARD                 7
#define SEND_CSD                 9  // CMD9  - Asks the selected card to send its card-specific data (CSD)
#define SEND_CID                10  // CMD10 - Asks the seleceted card to send its card identification (CID)
#define READ_DATA_TILL_STOP     11
#define SEND_STATUS             13  // CMD13 - Asks the selected cardto send its status register
#define SET_BLOCKLEN            16  // CMD16 - Selectsa block length (in bytes) for all following block cmd
#define READ_SINGLE_BLOCK       17  // CMD17 - Reads a block of the size selected by the CMD17
#define WRITE_BLOCK             24  // CMD24 - Writes a block of the size selected by the CMD17
#define PROGRAM_CSD             27  // CMD27 - Programming of the programmable bits of the CSD
#define SET_WRITE_PROT          28  // CMD28 - Sets the write protection
#define CLR_WRITE_PROT          29  // CMD29 - Clears the write protection
#define SEND_WRITE_PROT         30  // CMD30 - Requests for the write protection bits
#define TAG_SECTOR_START        32  // CMD32 - Sets the address of the first sector of the erase group
#define TAG_SECTOR_END          33  // CMD33 - Sets the address of the last sector
#define UNTAG_SECTOR            34  // CMD34 - Remove one previously selected secotorfrom the erase selection
#define TAG_ERASE_GROUP_START   35  // CMD35
#define TAG_ERASE_GROUP_END     36  // CMD36
#define UNTAG_ERASE_GROUP       37  // CMD37
#define ERASE                   38  // CMD38
#define LOCK_UNLOCK             42  // CMD42
#define APP_CMD                 55  // CMD55
#define GEN_CMD                 56  // CMD56
#define READ_OCR                58  // CMD58 - read Operation Condition Register
#define CRC_ON_OFF              59  // CMD59

/* --- MMC protocol command response type --- */
enum {
   MMC_R1_RESP = 1,  // R1 type response  - 48 bits  (6 bytes)
   MMC_R1b_RESP,    // R1b type response - 48 bits  (6 bytes)
   MMC_R2_RESP,     // R2 type response  - 136 bits (17 bytes)
   MMC_R3_RESP      // R3 type response  - 48 bits  (6 bytes)
};// MMC_RESP_TYPE;

/* --- MMC card --- */
#define MMC_SECTOR_SIZE 512
#define DIR_ENTRY_SIZE  32

/* --- MMC status --- */
#define char_MASK   0x1e00
#define char_IDLE   0x0000
#define char_READY  0x0200
#define char_IDENT  0x0400
#define char_STBY   0x0600
#define char_TRAN   0x0800
#define char_DATA   0x0a00
#define char_RCV    0x0c00
#define char_PRG    0x0e00
#define char_DIS    0x1000

#define MMC_ERR_MASK    0xfccf0000

/* ------------------------------------------ */
enum {
    MMC_OK,         // MMC driver operation successful
    MMC_RD_ERR,     // MMC driver reading error
    MMC_WR_ERR,     // MMC driver writing error
    MMC_CRC_ERR,    // MMC CRC error
    MMC_NO_RESP,
    MMC_NOT_RDY,    // MMC not ready
    MMC_ERR         // MMC driver operation failed
};


#define DISABLE_SD_CARD 	SD_DISABLE
#define ENABLE_SD_CARD 		SD_ENABLE
#define SECTOR_SIZE			MMC_SECTOR_SIZE

char MMC_Init(void);
char MMC_ReadSegment (unsigned long sect, unsigned char seg);
char MMC_ReadSegOffset (unsigned long sect, unsigned int offset);
char MMC_ReadSegOffsetLength (unsigned long sect, unsigned int offset, unsigned char length);
char MMC_NextSector (unsigned long sector);

unsigned int  MMC_BlkSize (void);
unsigned long MMC_NumOfBlk (void);
unsigned long MMC_GetStatus (void);
char MMC_ChkStatus (void);
void MMC_dumpSegment (unsigned char seg);
void MMC_flushSector (unsigned char seg);
char MMC_SkipSegment (unsigned long sect, unsigned char seg);

#endif // end of _MMC_H
