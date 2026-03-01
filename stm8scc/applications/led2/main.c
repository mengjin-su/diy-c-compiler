#include <stm8s103f3p6.h>

#define LED_ON		PB_ODRbits->b5 = 0 // PB_ODR &= ~(1<<5)
#define LED_OFF		PB_ODRbits->b5 = 1 // PB_ODR |=  (1<<5)

void init()
{
	CLK_CKDIVR = 0;
	PB_DDR |= 1<<5;	// PB5 : output
	PB_CR1 |= 1<<5;	// push-pull mode
	PB_CR2  = 0;	// 2Mhz speed
}

int scale(short x, short y)
{
    while ( (x % y) != 5 );
    return 5;
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

void main()
{
	init();

	unsigned char i;
    for(;;)
    {
        for (i = 0; i < 255; i++) { fade(i); }
        for (i = 255; i > 0; i--) { fade(i); }
    }
}
