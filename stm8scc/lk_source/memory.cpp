#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ctrl.h"
#include "memory.h"

Memory :: Memory(MEMORY_TYPE _type)
{
	type = _type;
	switch ( type )
	{
		case DATA_MEMORY:	size = 8192;				break;
		case CODE_MEMORY:	size = 1024*128 +0x8000;	break;
	}
	init();
}

Memory :: ~Memory()
{
	delete [] mem;
}

void Memory :: resize(int _size)
{
	if ( type == CODE_MEMORY ) _size += 0x8000;
	if ( _size != size )
	{
		delete [] mem;
		size = _size;
		init();
	}
}

void Memory :: init(void)
{
	mem = new char[size];
	memset(mem, 0, size);
	if ( type == CODE_MEMORY )
	{
		int fill_size = (size >= 0x8080)? 0x8080: size;
		memset(mem, -1, fill_size);
	}
	else
		memset(mem, -1, extendedMode? 8: 6);
}

void Memory :: blank(int start, int length)
{
	for (int a = 0; a < length && (start+a) < size; a++)
		mem[start+a] = -1;
}

int Memory :: getSpace(int start_addr, int req_size, int flags)
{
    int a;
	for (int l = req_size; l > 0; a++)
	{
		if ( l == req_size )
			a = start_addr;

		if ( a >= size ) 		// exceeds memory limit
			return -1;

		if ( mem[a] == 0 )		// location available...
		{
            if ( --l == 0 )
				break;			// finished.
		}
		else if ( flags & FIXED_ADDR_FLAG )
		{
			if ( mem[a] != -1 )
				return -1;

			if ( --l == 0 )
				break;
		}
		else
		{
			l = req_size;		// restart
			start_addr++;
		}

		// exceeds bank0 access range
		if ( type == DATA_MEMORY && (flags & BANK0_ADDR_FLAG) && a >= 256 )
			return -1;

		// exceeds 64KB boundry
		if ( (flags & _64KB_ADDR_FLAG) && (a & 0xffff) == 0 && l < req_size )
		{
			l = req_size;		// restart
			start_addr++;
		}
	}

	fillSpace(start_addr, req_size, 1);
	return start_addr;
}

void Memory :: fillSpace(int start, int length, char val)
{
	for (int a = start; length-- && a < size; a++)
	{
		if ( mem[a] == 0 ) mem[a] = val;
		if ( mem[a]  < 0 ) mem[a] = -2;
	}
}

void Memory :: reset(void)
{
	for (int i = 0; i < size; i++)
		mem[i] = (mem[i] >= 0)? 0: -1;
}

int Memory :: memUsed(int *total)
{
	int n = 0;
	*total = 0;

	if (  type == DATA_MEMORY )
		n = extendedMode? 8: 6;

	for (int i = 0; i < size; i++)
	{
		if ( type == CODE_MEMORY && (i >= 0x8000 && i < 0x8080) )
		{
			n++;
			(*total)++;
		}
		else
		{
			if ( mem[i] > 0 ) n++;
			if ( mem[i] >= 0 || type == DATA_MEMORY ) (*total)++;
		}
	}
	return n;
}

void Memory :: display(int addr)
{
	for (int i = 0; i < addr; i++)
	{
		if ( (i & 0x0f) == 0 ) printf("\n%03x: ", i);
		printf("%c ", mem[i]+'0');
	}
	printf("\n");
}

