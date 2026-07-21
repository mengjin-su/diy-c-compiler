#include <stdio.h>
#include <string.h>

int main()
{
    char *p = "\\";
    while ( *p ) printf ("%c", *p++);
    printf("\n");
    return 0;
}
