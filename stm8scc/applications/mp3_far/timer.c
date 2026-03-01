#include <stm8s105x6.h>
#include "define.h"
#include "timer.h"
#include "key.h"
#include "play.h"

volatile _bank0_ unsigned int Timer0;

//////////////////////////////////////////////
interrupt tm4_isr(__TIM4UPD_VECTOR__)
{
	TIM4_SR = 0;	// clear interrupt flag(s)
	Timer0++;

	switch ( play_state )
	{
		case PLAY_RUN:
			if ( Timer0 & 0x100 )
                LED_ON;
			else
                LED_OFF;
			break;

		case PLAY_READY:
		case PLAY_PAUSE:
            LED_ON;
			break;
			
		default:
            if ( Timer0 & (0x200/8) )
                LED_ON;
			else
                LED_OFF;
	}

	KEY_scan ();
}

//////////////////////////////////////////////
void TMR0_Init (void)
//void timer4_init ()
{
#if 0
	TIM4_PSCR = 0x40;
	TIM4_CR1  = 0x85;
	TIM4_IER  = 0x40;
    TIM4_CNTR = 0x88;
	TIM4_SR   = 0;
#else
    TIM4_PSCR = 0x07;  /* Configure TIM4 prescaler. */
    TIM4_ARR  = 0xfe;
    TIM4_CNTR = 0xe7;
    TIM4_CR1  = 0x81;
    TIM4_IER  = 0x41;
#endif
}
