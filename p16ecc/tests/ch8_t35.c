#include <pic12f1840.h>

void foo()
{
    char a;
    STATUSbits->Z = a;     // write to Zero flag bit in STATUS
    OSCCONbits->IRCF = 2;
}