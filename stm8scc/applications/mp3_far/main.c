/*
 * ----------------------------------------------------------------------------
 * "STM8S Discovery based MP3 Player" (Revision 42):
 * <mengjins@hotmail.com>
 * ----------------------------------------------------------------------------
 */
#include <stm8s105x6.h>
#include "define.h"
#include "spi.h"
#include "timer.h"
#include "key.h"
#include "play.h"
#include "vs1011.h"
#include "sound.h"
#include "fat.h"

void ding(void);

////////////////////////////////////////////////////
void init(void)
{
	// config system clock t o 16MHz
	CLK_ECKR |= 1;	 			// activate ext clock
	while ( !(CLK_ECKR & 2) );	// wait for ext clock ready

	CLK_CKDIVR &= 0xf8; 		// no clock divider

	CLK_SWR = 0XB4; 			// select ext clock
	while ( !(CLK_SWCR & 8) );	// wait for ext clock

	CLK_SWCR |= 2;	 			// enable ext clock

    LED_CFG_DDR;				// configure LED pin.
    SPI_init ();				// SPI interface init.
    TMR0_Init();				// system timer init.
	KEY_init ();
	VS1011_RST_CFG;
}

////////////////////////////////////////////////////
int main(void)
{
    unsigned char key;
    unsigned int files;
	unsigned int index;
	unsigned int t;

	init();
	SEI();

	play_state = PLAY_INIT;

    /* reset VS1011 chip ... */
	VS1011_RST_LO;
    for (t = Timer0; t == Timer0; );
	VS1011_RST_HI;

	VS1011_init();	// VS1011 decoder init.
	delay();

    for (;;)
	{
        FileDesc_t *f;

        if ( !fatInit() )
        {
             delay();
             continue;
        }

        files = FILE_searchMP3Files();
		index = 1;
	    VS1011_setVol(35);		// set audio volume (default)


		while ( files )
		{
			if ( play_state == PLAY_INIT || play_state == PLAY_DONE )
			{
				if ( index > files )
					index = 1;
                else if ( index == 0 )
					index = files;

				f = FILE_searchMP3File(index);

				if ( f == NULL )
                    break;

				PLAY_init(f);

				if ( play_state == PLAY_INIT )
					play_state = PLAY_READY;
				else
					play_state = PLAY_RUN;
			}

			if ( play_state != PLAY_RUN )
			{
                key = KEY_get();

				switch ( key )
				{
                    case KEY_UP:    index++; 	break;	// next file
                    case KEY_DOWN:  index--;	break;	// last file
                    case KEY_PLAY:	play_state = PLAY_RUN;
				}

				// remove the remaining data of last (unfinished) song,
				// if switch to another song ...
				if ( key == KEY_UP || key == KEY_DOWN )
				{
					// read the remaining of the current sector
					if ( play_state == PLAY_PAUSE )
					{
						PLAY_fade_out();
						MMC_flushSector(PLAY_ctrlBlk.fileDesc->segment);
					}

					ding();
                    play_state = PLAY_INIT;     // ready a new MP3.
				}

				continue;
			}

			do
			{
                play_state = PLAY_music();

				if ( play_state == PLAY_RUN )
				{
                    switch ( KEY_get() )
					{
						case KEY_UP:	VS1011_incVol();	   		break;
						case KEY_DOWN:	VS1011_decVol();			break;
						case KEY_PLAY:	play_state = PLAY_PAUSE;	break;
					}
				}

			} while ( play_state == PLAY_RUN );

			if ( play_state != PLAY_PAUSE )
				PLAY_fade_out();

			if ( play_state == PLAY_DONE )
				index++;
        }
	}
}

////////////////////////////////////////////////////////////////////////
void ding(void)
{
	char *p = ballon_snd;
	unsigned int length;

	SD_SCK_HOLD;
	ENABLE_SPI2();

    VS1011_ENABLE_DATA;

	for (length = 0; length < sizeof(ballon_snd); length++)
	{
		while ( VS1011_BUSY );
		SPI2_write(*p++);
	}

	VS1011_DISABLE_DATA;
	DISABLE_SPI2();
	SD_SCK_UNHOLD;
}


