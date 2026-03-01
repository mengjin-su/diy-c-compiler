#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern "C" {
#include "common.h"
#include "lnk.h"
}
#include "segment.h"

Segment	*libSegGroup  = NULL;
Segment	*codeSegGroup = NULL;
Segment	*dataSegGroup = NULL;
Segment	*miscSegGroup = NULL;

///////////////////////////////////////////////////////////////////////
Segment :: Segment(char *filename)
{
	memset(this, 0, sizeof(Segment));
	fileName = dupStr(filename);
}

Segment :: ~Segment()
{
	free(fileName);
	freeLines(lines);
}

void Segment :: addLine(line_t *lp)
{
	if ( lines == NULL )
		lines = lp;
	else
	{
		line_t *lptr = lines;
		while ( lptr->next )
			lptr = lptr->next;
		lptr->next = lp;
	}
}

int Segment :: type(void)
{
	if ( lines && lines->type == 'S' && lines->items->type == TYPE_SYMBOL )
	{
		if ( memcmp(lines->items->data.str, "BANK", 4) == 0 )
			return DATA_SEGMENT;

		if ( memcmp(lines->items->data.str, "CODE", 4) == 0 )
			return CODE_SEGMENT;

		if ( memcmp(lines->items->data.str, "CONST",5) == 0 )
			return CONST_SEGMENT;
	}
	return DUMMY_SEGMENT;
}

int Segment :: lineCount(void)
{
	int n = 0;
	for (line_t *lp = lines; lp; lp = lp->next)
		n++;

	return n;
}

int Segment :: size(void)
{
	int size = 0;
	for (line_t *lp = lines; lp; lp = lp->next)
	{
		switch ( lp->type )
		{
			case 'J':
			case 'U':
			case 'V':
			case 'B':	size += lineSize(lp);			break;
			case 'R':	size += lp->items->data.val;	break;
		}
	}
	return size;
}

char *Segment :: name(void)
{
	if ( lines && lines->items && lines->items->type == TYPE_SYMBOL )
		return lines->items->data.str;

	return (char*)"";
}

void Segment :: print(void)
{
	printf("Type=%d, name=%s", type(), name());
	line_t *lp = lines->next;
	if ( lp && (lp->type == 'U' || lp->type == 'G' || lp->type == 'L') )
	{
		item_t *ip = lp->items;
		if ( ip->type == TYPE_SYMBOL )
			printf(", Title=%s", ip->data.str);
	}
	printf(", file=%s, addr=0x%x, size=%d\n", fileName, addr, size());
/*
	lp = lines->next;
	for (; lp; lp = lp->next)
	{
		for (item_t *ip = lp->items; ip; ip = ip->next)
			switch ( ip->type )
			{
				case TYPE_VALUE:	printf("%d, ", ip->data.val);	break;
				case TYPE_SYMBOL:
				case TYPE_STRING:	printf("%s, ", ip->data.str);	break;
			}


		printf("..\n");
	}
*/
}

///////////////////////////////////////////////////////////////
void deleteSegments(Segment *list)
{
	while ( list )
	{
		Segment *next = list->next;
		delete list;
		list = next;
	}
}

void printfSegments(Segment *slist)
{
	for (; slist; slist = slist->next)
		slist->print();
}