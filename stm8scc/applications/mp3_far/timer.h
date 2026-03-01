#ifndef _TIMER_H
#define _TIMER_H

enum {LED_BUSY, LED_PLAY, LED_READY, LED_DEBUG};

extern volatile _bank0_ unsigned int Timer0;

#define toggle_led	PD_ODR ^= 1

void TMR0_Init (void);
#define TMR0_Elapsed(t)    (unsigned int)(Timer0-t)

#endif

