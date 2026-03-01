#include <stm8s105x6.h>
#include "key.h"
#include "timer.h"
#include "key.h"
#include "define.h"

enum {KEY_IDLE, KEY_PRESSED, KEY_WAIT1, KEY_WAIT2};
static _bank0_ char KEY_stat = KEY_IDLE;
static _bank0_ char KEY_val  = 0;

//////////////////////////////////////////////////////////////
void KEY_init (void)
{
	sbi (PA_CR1, KEY_PLAY_PIN);	// weak pull-up
	sbi (PC_CR1, KEY_UP_PIN);
	sbi (PE_CR1, KEY_DOWN_PIN);
}

//////////////////////////////////////////////////////////////
void KEY_scan (void)
{
    static unsigned char key_hold;
    static unsigned int  key_time;
    unsigned char key = 0;

	if ( (PA_IDR & (1 << KEY_PLAY_PIN)) == 0 ) 
		key = KEY_PLAY;

	if ( (PC_IDR & (1 << KEY_UP_PIN)) == 0 ) 
		key |= KEY_UP;

	if ( (PE_IDR & (1 << KEY_DOWN_PIN)) == 0 ) 
		key |= KEY_DOWN;
		
    switch ( KEY_stat )
    {
        case KEY_IDLE:
            if ( key )
            {
                KEY_val  = 0;
                key_hold = key;
                KEY_stat = KEY_PRESSED;
                key_time = Timer0;				// wait 20mS = 5 ticks
            }
            break;

		case KEY_PRESSED:
			if ( key != key_hold )
				KEY_stat = KEY_IDLE;

			if ( TMR0_Elapsed (key_time) >= 10 )	// remove debouncing
			{
				KEY_val  = key;
				KEY_stat = KEY_WAIT1;
            }
			break;

		case KEY_WAIT1:
            if ( key == 0 )
            {
                key_time = Timer0;
                KEY_stat = KEY_WAIT2;
			}
			break;

		case KEY_WAIT2:
            if ( key )
			{
                KEY_stat = KEY_WAIT1;
			}
			else if ( TMR0_Elapsed(key_time) >= 10 )	// remove debouncing
			{
				KEY_stat = KEY_IDLE;
			}
			break;

		default:
			KEY_stat = KEY_IDLE;
	}
}

//////////////////////////////////////////////////////////////
char KEY_get (void)
{
	char tmp = KEY_val;
	KEY_val = 0;
	return tmp;
}

