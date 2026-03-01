#include <stm8s105x6.h>
#include "define.h"
#include "play.h"
#include "fat.h"
#include "timer.h"
#include "spi.h"
#include "vs1011.h"

_bank0_ play_ctrl_t   PLAY_ctrlBlk;
_bank0_ char play_state;

////////////////////////////////////////////////////////////
void PLAY_init (FileDesc_t *file_desc)
{
	PLAY_ctrlBlk.fileDesc  = file_desc;
    PLAY_ctrlBlk.secRemain = (file_desc->size + 511) >> 9; 	// data sectors remain
	PLAY_ctrlBlk.inPlay    = 0;
}

////////////////////////////////////////////////////////////
char PLAY_music (void)
{
	unsigned char segment = PLAY_ctrlBlk.fileDesc->segment;

	do
	{
		// all segments (in last sector) have been played?
		if ( PLAY_ctrlBlk.inPlay && segment >= (MMC_SECTOR_SIZE/SEGMENT_SIZE) )
		{
			PLAY_ctrlBlk.secRemain--;

			if ( PLAY_ctrlBlk.secRemain == 0 )	// any sector left for playing?
			{
				// YES, then done.
				PLAY_fade_out ();
				return PLAY_DONE;
			}

			if ( !FILE_nextFileSector (PLAY_ctrlBlk.fileDesc) )
			{
				PLAY_fade_out ();
				return PLAY_ERR;
			}

			segment = 0;
		}

		if ( !VS1011_BUSY )
		{
			if ( !PLAY_ctrlBlk.inPlay )
			{
				// get the first sector in the first cluster of the music.
				if ( !FILE_nextFileSector (PLAY_ctrlBlk.fileDesc) )
					return PLAY_ERR;

				PLAY_ctrlBlk.inPlay = 1;
			}

            MMC_dumpSegment (segment++);
		}

	} while ( !VS1011_BUSY || segment >= (MMC_SECTOR_SIZE/SEGMENT_SIZE) );

	PLAY_ctrlBlk.fileDesc->segment = segment;
	return PLAY_RUN;
}

////////////////////////////////////////////////////////////
void PLAY_fade_out (void)
{
	unsigned char n = 0;

	SD_SCK_HOLD;
//	ENABLE_SPI2;
	ENABLE_SPI2();
	VS1011_ENABLE_DATA;

	do
	{
		while ( VS1011_BUSY );
        SPI2_write (0);
    } while ( --n );

	VS1011_DISABLE_DATA;
//	DISABLE_SPI2;
	DISABLE_SPI2();
	SD_SCK_UNHOLD;

	VS1011_reset ();
}

