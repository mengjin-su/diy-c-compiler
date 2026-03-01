#include "includes.h"

unsigned char tmr0Count = 0;

void TMR0_init(void)
{
	T0CON1bits->CS   = 2;	// Fsoc/4
	T0CON1bits->CKPS = 5;	// scale = 32

	T0CON0bits->MD16 = 0;	// 8-bit mode
	T0CON0bits->EN   = 1;	// enable

	PIR0bits->TMR0IF = 0;	// clear timer0 interrupt
    PIE0bits->TMR0IE = 1;	// enable timer0 interrupt
    INTCONbits->PEIE = 1;

    TMR0L = -250;
}

void TMR0_delayMs(unsigned char ms)
{
	unsigned char t = tmr0Count;
	while ( (unsigned char)(tmr0Count - t) < ms );
}

interrupt tmr0_isr()
{
	if ( PIR0bits->TMR0IF )	// 1KHz -> 1ms
	{
		TMR0L = -250;
		PIR0bits->TMR0IF = 0;
        tmr0Count++;

        if ( TRISAbits->b5 )
	    	KEY_scan();
    }
}