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
#include "exp.h"

// this enum list must be copied from st8cc...
enum {
	J_CODE_EQ,  J_CODE_NE,  J_CODE_C,   J_CODE_NC,
	J_CODE_PL,  J_CODE_MI,
	J_CODE_SGT, J_CODE_SGE, J_CODE_SLT, J_CODE_SLE,
	J_CODE_UGT, J_CODE_UGE, J_CODE_ULT, J_CODE_ULE
};

static int  updateVariableInstruction(Segment *segp, Symbol *symblist);
static void removeVariableInstruction(Segment *segp, Symbol *symblist);
static int  relativeBranchCode(int code, int extra_code);

//////////////////////////////////////////////////////////////////////////////////////////
int ST8link :: assignSegmentsAddress(void)
{
	do {
		deleteSymbols(&symbList);
		codeMem->reset();
		dataMem->reset();

		assignSegmentMem(dataSegGroup, RAM_LOC_ABS);	// assign ABS data
		assignSegmentMem(dataSegGroup, RAM_LOC_BANK0);	// assign BANK0 data
		assignSegmentMem(dataSegGroup, RAM_LOC_FLOAT);	// assign rest float data
		logSegmentSymbols(dataSegGroup, &symbList);

		assignSegmentMem(codeSegGroup, ROM_LOC_ABS);
		assignSegmentMem(codeSegGroup, ROM_LOC_FLOAT);
		logSegmentSymbols(codeSegGroup, &symbList);

		if ( errorCount > 0 ) return errorCount;

	} while ( updateVariableInstruction(codeSegGroup, symbList) );

	removeVariableInstruction(codeSegGroup, symbList);

	logSegmentSymbols(miscSegGroup, &symbList);

	return errorCount;
}

void ST8link :: logSegmentSymbols(Segment *seglist, Symbol **symlist)
{
	for (Segment *sp = seglist; sp; sp = sp->next)
	{
		int stype = sp->type();
		int addr  = sp->addr;
		for (line_t *lp = sp->lines; lp; lp = lp->next)
		{
			item_t *ip = lp->items;
			int ltype = lp->type;
			int value;
			char buf[4096];

			switch ( ltype )
			{
				case 'G':
				case 'L':
					if ( !strstr(ip->data.str, "$init$") &&
						 (stype == CONST_SEGMENT || stype == DATA_SEGMENT) )
					{
						value = sp->size();
						sprintf(buf, "%s$sizeof$", ip->data.str);
						logSymbol(symlist, ltype, buf, sp, value);
					}
					logSymbol(symlist, ltype, ip->data.str, sp, addr);
					break;

				case 'E':
					if ( ip->next )
					{
						value = ip->next->data.val;
						logSymbol(symlist, ltype, ip->data.str, sp, value);
					}
					break;

				case 'B':
				case 'R':
				case 'J':
				case 'U':
				case 'V':
					addr += lineSize(lp);
					break;
			}
		}
	}
}

bool ST8link :: scanSegLabels(Segment *seglist, Symbol *symlist)
{
	for (; seglist; seglist = seglist->next)
	for (line_t *lp = seglist->lines; lp; lp = lp->next)
	{
		switch ( lp->type )
		{
			case 'B':
			case 'R':
			case 'J':
			case 'U':
			case 'V':
				for (item_t *ip = lp->items; ip; ip = ip->next)
				{
					int v;
					char *lbl = expValue(ip, seglist, symlist, &v, 0);
					if ( lbl )
					{	// not found in current list, then search for lib files.
						Segment *seg = extractLibSegment(&libSegGroup, lbl, seglist);
						if ( seg )	// found the seg in .lib file
						{
							addSeg(seg, false);
							return true;
						}
						printf("%s #%d, symbol not defined: '%s'!\n",
							   lp->fname, lp->lineno, lbl);
						errorCount++;
					}
				}
				break;
		}
	}
	return false;
}

Segment *ST8link :: extractLibSegment(Segment **slist, char *lbl, Segment *seg)
{
	Segment *ss = NULL;
	for (Segment *sl = *slist; sl; sl = sl->next)
	for (line_t *lp = sl->lines; lp; lp = lp->next)
	{
		if ( (lp->type == 'G' || lp->type == 'L') &&
			 strcmp(lp->items->data.str, lbl) == 0 )
		{
			if ( strcmp(sl->fileName, seg->fileName) == 0 )
				ss = sl;
			else if ( !ss && lp->type == 'G' )
				ss = sl;
		}
	}

	if ( ss )
	{
		Segment *last = NULL;
		for (Segment *sl = *slist; sl; last = sl, sl = sl->next)
		{
			if ( ss == sl )
			{
				if ( last )	last->next = sl->next;
				else		*slist     = sl->next;

				sl->next = NULL;
				break;
			}
		}
	}
	return ss;
}

static int updateVariableInstruction(Segment *segp, Symbol *symblist)
{
	for (Segment *sp = segp; sp; sp = sp->next)
	{
		int addr = sp->addr;	// start address for the segment
		for (line_t *lp = sp->lines; lp; lp = lp->next)
		{
			int w, rel_addr;
			item_t *ip;
			switch ( lp->type )
			{
				case 'J':	// variable jump instruction
				case 'U':	// variable branch instruction
				case 'V':	// variable branch instruction
					ip = itemPtr(lp, itemCount(lp) - 1);
					expValue(ip, sp, symblist, &w, addr + lp->insert);
					rel_addr = w - (addr + lp->insert);	// relative range
					if ( (rel_addr < -128) || (rel_addr > 127) )	// -128 ~ 127 ?
					{
						int new_insert = (lp->type == 'J')? 2: 5;	// base size

						if ( itemCount(lp) == 1 )					// simple jump
							new_insert += (w > 0xffff)? 2: 1;
						else										// conditional branch
							new_insert += (w > 0xffff)? 4: 3;

						if ( lp->insert < new_insert )
						{
							lp->insert = new_insert;	// update instruction size.
							return 1;
						}
					}
			}
			addr += lineSize(lp);
		}
	}
	return 0;
}

static void removeVariableInstruction(Segment *segp, Symbol *symblist)
{
	for (Segment *sp = segp; sp; sp = sp->next)
	{
		int addr = sp->addr;	// start address for the segment
		line_t *last_lp = NULL;
		for (line_t *lp = sp->lines; lp; last_lp = lp, lp = lp->next)
		{
			#define TGT_ADDR		w[item_count - 1]
			#define REL_ADDR		(TGT_ADDR - (addr + basic_size))
			#define NEW_ITEM(t,v,s)	t = valItem(v), t->size = s
			item_t *ip, *item[5];
			int code, basic_size, extra_size;
			int w[3], item_count, length = 0;

			switch ( lp->type )
			{
				case 'J':	case 'U':	case 'V':
					basic_size = (lp->type == 'J')? 2: 5;
					extra_size = lp->insert - basic_size;
					item_count = itemCount(lp) & 3;
					for (int i = 0; i < item_count; i++)
						expValue(itemPtr(lp, i), sp, symblist, &w[i], addr + lp->insert);

					if ( lp->type == 'J' )
					{
						code = (item_count == 1)? -1: w[0];
						code = relativeBranchCode(code, extra_size);
						NEW_ITEM(item[0], code, 1);
						NEW_ITEM(item[1], REL_ADDR, 1);
						length = 2;	// 2 items, 2 bytes total
					}
					else
					{
						code = relativeBranchCode(lp->type, extra_size);
						NEW_ITEM(item[0], code | (w[1] << 1), 2);
						NEW_ITEM(item[1], w[0], 			  2);
						NEW_ITEM(item[2], REL_ADDR,			  1);
						length = 3;	// 3 items, 5 bytes total
					}

					switch ( extra_size )
					{
						case 1:	case 2:	// for ['J' label] instruction
							item[1]->data.val = TGT_ADDR;  item[1]->size += extra_size;
							break;

						case 3: case 4:	// for ['J' c label]  or  ['U'/'V' a b label]
							item[length-1]->data.val = extra_size;
							code = (extra_size == 3)? 0xCC: 0xAC;
							NEW_ITEM(item[length], code, 1);				  length++;
							NEW_ITEM(item[length], TGT_ADDR, extra_size - 1); length++;
							break;
					}
					// replace the line with new item line ...
					line_t *new_line = (line_t *)MALLOC(sizeof(line_t));
					*new_line = *lp;
					lp->src   = NULL;
					lp->fname = NULL;	freeLine(lp);

					lp = new_line;
					lp->type = 'B';
					lp->items = item[0];
					for (int i = 1; i < length; i++)
						appendItem(lp->items, item[i]);

					if ( last_lp == NULL )	sp->lines     = lp;
					else					last_lp->next = lp;
			}
			addr += lineSize(lp);	// adjust address for every line.
		}
	}
}

static int relativeBranchCode(int code, int extra_code)
{
	switch ( code )
	{
		case J_CODE_EQ:		return extra_code? relativeBranchCode(J_CODE_NE, 0): 0x27;
		case J_CODE_NE:		return extra_code? relativeBranchCode(J_CODE_EQ, 0): 0x26;
		case J_CODE_C:		return extra_code? relativeBranchCode(J_CODE_NC, 0): 0x25;
		case J_CODE_NC:		return extra_code? relativeBranchCode(J_CODE_C,  0): 0x24;
		case J_CODE_PL:		return extra_code? relativeBranchCode(J_CODE_MI, 0): 0x2a;
		case J_CODE_MI:		return extra_code? relativeBranchCode(J_CODE_PL, 0): 0x2b;
		case J_CODE_SGT:	return extra_code? relativeBranchCode(J_CODE_SLE,0): 0x2c;
		case J_CODE_SGE:	return extra_code? relativeBranchCode(J_CODE_SLT,0): 0x2e;
		case J_CODE_SLT:	return extra_code? relativeBranchCode(J_CODE_SGE,0): 0x2f;
		case J_CODE_SLE:	return extra_code? relativeBranchCode(J_CODE_SGT,0): 0x2d;
		case J_CODE_UGT:	return extra_code? relativeBranchCode(J_CODE_ULE,0): 0x22;
		case J_CODE_UGE:	return extra_code? relativeBranchCode(J_CODE_ULT,0): 0x24;
		case J_CODE_ULT:	return extra_code? relativeBranchCode(J_CODE_UGE,0): 0x25;
		case J_CODE_ULE:	return extra_code? relativeBranchCode(J_CODE_UGT,0): 0x23;
		case 'U':			return extra_code? relativeBranchCode('V', 0): 0x7201;
		case 'V':			return extra_code? relativeBranchCode('U', 0): 0x7200;
		case -1:			return extra_code==0? 0x20:
								   extra_code==1? 0xCC: 0xAC;
	}
	return 0;
}
