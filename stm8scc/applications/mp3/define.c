#include <stm8s105x6.h>
#include "define.h"
#include "timer.h"

//////////////////////////////////////////////////////////////////////
void delay (void)
{
	unsigned int t = Timer0;
	while ( TMR0_Elapsed(t) == 0 );
}

//////////////////////////////////////////////////////////////////////
char MEMCMP_ROM (char *s1, char *s2, char len)
{
	while ( len-- )
	{
		char c1 = *s1++;
        char c2 = *s2++;

        if ( c1 >= 'a' ) c1 -= 'a' - 'A';
        if ( c2 >= 'a' ) c2 -= 'a' - 'A';

        if ( c1 != c2 )
			return 0;
	}

	return 1;
}
