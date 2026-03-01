#include <stdio.h>

typedef struct {
	int a[10];
	char b[][10];
} Test;

#define SEED 1

int main(int argc, char *argv[])
{
	int n = argc;

	if ( n > 1 )
	{
		#define BASE 10
	}
	else
	{
//		#undef BASE
//		#define BASE 20
	}

	n += SEED + BASE + sizeof(Test);

	printf("argc = %d\n", n);
	return 0;
}
