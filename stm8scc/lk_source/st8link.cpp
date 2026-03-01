#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern "C" {
#include "common.h"
#include "lnk.h"
}
#include "memory.h"
#include "segment.h"
#include "symbol.h"
#include "st8link.h"

ST8link :: ST8link(Memory *ram, Memory *rom)
{
	memset(this, 0, sizeof(ST8link));
	dataMem = ram;
	codeMem = rom;
}

ST8link :: ~ST8link()
{
	deleteSymbols(&symbList);
}

int ST8link :: scanInclusion(void)
{
	Symbol *sym_list = NULL;

	// scan segments for including...
	do {
		deleteSymbols(&sym_list);		// clear symbol list

		if ( errorCount > 0 )
			return errorCount;

		logSegmentSymbols(codeSegGroup, &sym_list);
		logSegmentSymbols(dataSegGroup, &sym_list);
		logSegmentSymbols(miscSegGroup, &sym_list);

	} while ( scanSegLabels(codeSegGroup, sym_list) ||
			  scanSegLabels(dataSegGroup, sym_list) ||
			  scanSegLabels(miscSegGroup, sym_list)  );

	// remove unused segments (code & data)
	searchUnusedSegment(sym_list);
	removeUnusedSegment(&codeSegGroup);
	removeUnusedSegment(&dataSegGroup);

	deleteSymbols(&sym_list);
	return errorCount;
}

void ST8link :: addSeg(Segment *seg, bool add2lib)
{
	if ( seg == NULL ) return;

	line_t *lp = seg->lines;

	if ( add2lib )
	{
		appendSeg(&libSegGroup, seg);
		return;
	}

	if ( lp->type != 'S' )
	{
		appendSeg(&miscSegGroup, seg);
		return;
	}

	item_t *ip;
	for (int i = 1; (ip = itemPtr(lp, i)); i++)
	{
		if ( ip->type == TYPE_SYMBOL )
		{
			seg->addr = -1;
			if ( strcmp(ip->data.str, "REL") == 0 )
				seg->isREL = 1;
			if ( strcmp(ip->data.str, "ABS") == 0 )
				seg->isABS = 1;
			if ( strcmp(ip->data.str, "BEG") == 0 )
				seg->isBEG = 1;
			if ( strcmp(ip->data.str, "END") == 0 )
				seg->isEND = 1;
		}
		else if ( ip->type == TYPE_VALUE && seg->isABS )
		{
			seg->addr = ip->data.val;
		}
	}

	switch ( seg->type() )
	{
		case CONST_SEGMENT:
		case CODE_SEGMENT:	addSeg(&codeSegGroup, seg);		break;
		case DATA_SEGMENT:	addSeg(&dataSegGroup, seg);		break;
		default:			addSeg(&miscSegGroup, seg);		break;
	}
}

void ST8link :: addSeg(Segment **list, Segment *seg)
{
	Segment *sl = *list;
	if ( sl == NULL || seg->lines->type != 'S' )
	{
		appendSeg(list, seg);
	}
	else
	{
		char *s = seg->lines->items->data.str;
		for (Segment *last = NULL; sl; sl = sl->next)
		{
			int result = strcmp(sl->lines->items->data.str, s);
			if ( result == 0 && (seg->isBEG || sl->isEND) )
			{
				seg->next = sl;
				insertSeg(list, last, seg);
				return;
			}
			if ( result == 0 && seg->isABS && sl->isABS &&
				 seg->addr < sl->addr 					  )
			{
				seg->next = sl;
				insertSeg(list, last, seg);
				return;
			}
			if ( result > 0 )
			{
				seg->next = sl;
				insertSeg(list, last, seg);
				return;
			}
			if ( sl->next == NULL )
			{
				sl->next = seg;
				return;
			}
			last = sl;
		}
		printf("addSeg fail! - %s\n", s);
	}
}

void ST8link :: insertSeg(Segment **list, Segment *last, Segment *seg)
{
	if ( last ) last->next = seg;
	else		*list = seg;
}

void ST8link :: appendSeg(Segment **list, Segment *seg)
{
	if ( *list == NULL )
		*list = seg;
	else
	{
		Segment *sl = *list;
		while ( sl->next ) sl = sl->next;
		sl->next = seg;
	}
}