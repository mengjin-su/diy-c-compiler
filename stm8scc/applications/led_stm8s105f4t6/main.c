#include <stm8s105x4.h>

#define LED_ON		PE_ODRbits->b5 = 1
#define LED_OFF		PE_ODRbits->b5 = 0

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

	PE_DDRbits->b5 = 1;
}

void delay(unsigned char n)
{
    unsigned long num = n * 5;//scale(1055, 210);
	while ( num-- > 0 );
}

void fade(unsigned char n)
{
    LED_ON;
    delay(255-n);
    LED_OFF;
    delay(n);
}


////////////////////////////////////////////////////
int main(void)
{
	init();
//	SEI();

	unsigned char i;
    for(;;)
    {
        for (i = 0; i < 255/2; i++) { fade(i); }
        for (i = 255/2; i > 0; i--) { fade(i); }
    }
}


