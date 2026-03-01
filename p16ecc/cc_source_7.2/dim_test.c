//#include <stdio.h>

extern int a[][20];
int a[10][20], x, *p;

int *foo();

int main()
{
   int n;
   n = 100;
   n = a[n][2];
   if ( *&*&*p) x++;
   n = *foo();
   return 0;
}
