#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
extern "C" {
#include "common.h"
#include "lnk.h"
}
#include "memory.h"
#include "segment.h"
#include "symbol.h"
#include "st8link.h"
#include "exp.h"

void ST8link :: searchUnusedSegment(Symbol *slist)
{
	bool updated;
	do {
		updated = false;

		for (Segment *segp = codeSegGroup; segp; segp = segp->next)
		{
			int addr = segp->addr;

			if ( memcmp(segp->name(), "CODE", 4) == 0 && segp->isABS &&
				(addr >= 0x8000 && addr < 0x8080) && !segp->isUsed )
			{
				segp->isUsed = 1;
				updated = true;
			}

			if( strcmp(segp->name(), "CODEi") == 0 && !segp->isUsed )
			{
				segp->isUsed = 1;
				updated = true;
			}

			if ( segp->isUsed )
			{
				for (line_t *lp = segp->lines->next; lp; lp = lp->next)
				{
					switch ( lp->type )
					{
						case 'B':
						case 'J':
						case 'U':
						case 'V':
							for (item_t *ip = lp->items; ip; ip = ip->next)
							{
								if ( searchUnusedSegment(segp, ip, slist) )
									updated = true;
							}
							break;
					}
				}
			}
		}
	} while ( updated );
}

bool ST8link :: searchUnusedSegment(Segment *segp, item_t *ip, Symbol *slist)
{
	if ( ip == NULL )
		return false;

	Symbol *symb;
	switch ( ip->type )
	{
		case TYPE_SYMBOL:
			symb = searchSymbol(slist, ip->data.str, segp);
			if ( symb && symb->segment && !symb->segment->isUsed )
			{
				symb->segment->isUsed = 1;
				return true;
			}
			return false;

		case ':':
		case '+':	case '-':	case '*':	case '/':	case '%':
		case '&':	case '|':	case '^':
		case RSHIFT:	case LSHIFT:
			return ( searchUnusedSegment(segp, ip->left,  slist) ||
					 searchUnusedSegment(segp, ip->right, slist) );
	}
	return false;
}

void ST8link :: removeUnusedSegment(Segment **segp)
{
	bool done;
	do {
		done = true;
		Segment *last = NULL;
		for (Segment *sp = *segp; sp; last = sp, sp = sp->next)
		{
			if ( !sp->isUsed )
			{
				Segment *temp = sp->next;
				sp->next = NULL;
				delete sp;

				if ( last == NULL )
					*segp = temp;
				else
					last->next = temp;

				done = false;
				break;
			}
		}
	} while ( !done );
}