#include <pic12f1840.h>

void foo()
{
    if ( PORTAbits->b0 )
        LATAbits->b1 ^= 1;
}