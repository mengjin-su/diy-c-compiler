#include <string.h>
#include "includes.h"

#pragma isr_no_stack
#pragma acc_save 		1

#pragma FUSE0	0x3f8C	//_HFINTOSC_32MHZ
#pragma FUSE1	0x3fff	//_WDT_OFF
#pragma FUSE2   0x3f9f
#pragma FUSE3   0x3fff
#pragma FUSE4   0x3fff

void main()
{
  	OSCCON1 = 96;
  	OSCCON3 = 0;
  	OSCEN   = 0;
	OSCFRQ  = 5;
    OSCTUNE = 0x00;
    BORCON  = 0x00;

	INTCONbits->GIE = 1;	// enable interrupt
	ANSELA = 0;	        	// all inputs are digital

	TMR0_init();
	I2C_init();
	OLED_init();
	KEY_init();
	RADIO_init();

	for (;;)
	{
		RADIO_poll();
	}
}