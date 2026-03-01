//#include <stdio.h>

#define SEED	100

typedef char *Pointer;

int main()
{
    Pointer p;

    int n = 100;
    if ( SEED >= 100 )
    {
        n += 1;
        typedef unsigned char INT;
    }
    else
    {
        typedef unsigned int INT;
        n += BASE;
    }
    n += BASE - sizeof(Pointer);
    return 0;
}
