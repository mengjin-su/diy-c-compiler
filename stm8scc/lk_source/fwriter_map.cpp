#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern "C" {
#include "common.h"
#include "lnk.h"
}
#include "fwriter.h"
#include "exp.h"
//#include "disasm.h"
#include "st8dasm.h"

MapWriter :: MapWriter(char *filename)
{
	fout = fopen(filename, "w");
	if ( fout == NULL ) fout = stdout;
	fprintf(fout, ">>>>>>>>>>>> MAP OUTPUT <<<<<<<<<<<<\n\n");
}

void MapWriter :: close(void)
{
	if ( fout ) fclose(fout);
}

void MapWriter :: outputSeg(Symbol *slist, int used, int total)
{
	float pct = (float)used*100/(float)total;
	fprintf(fout, "- DATA MEMORY %d bytes(%.2f%c) used\n", used, pct, '%');
	for (; slist; slist = slist->next)
	{
		Segment *seg = slist->segment;
		char *fname = seg->lines->fname;
		char *name  = slist->name;

		if ( seg->type() == DATA_SEGMENT &&
			 !strstr(name, "$sizeof$")   && !strstr(name, "$init$") )
		{
			int size = seg->size();
			int addr = slist->addr;
			fprintf(fout, "%s  %s (%d): 0x%X\n", fname, name, size, addr);
		}
	}
}

void MapWriter :: outputSeg(Segment *seg, Symbol *slist, int used, int total)
{
	float pct = (float)used*100/(float)total;
	fprintf(fout, "\n- CODE MEMORY %d bytes(%.2f%c) used\n", used, pct, '%');

	// output function names and their addresses ...
	fprintf(fout, "\nFunctions and Addresses\n");
	for (Symbol *sp = slist; sp; sp = sp->next)
	{
		if ( strchr(sp->name, '$') == NULL && sp->addr > 0x8000 )
		{
			fprintf(fout, "%s ", sp->name);
			for (int i = strlen(sp->name); i < 32; i++) fprintf(fout, ".");
			fprintf(fout, " %05X\n", sp->addr);
		}
	}
	fprintf(fout, "\n");

	STM8S_dasm *st8dasm = new STM8S_dasm(slist);	// prepare the disassembler
	for (; seg; seg = seg->next)
	{
		int addr = seg->addr;		// segment start address
		int type = seg->type();	// segment type (CODE, CONST, ...)

		for (line_t *lp = seg->lines; lp; lp = lp->next)
		{
			item_t *ip = lp->items;
			int v;
			switch ( lp->type )
			{
				case 'S':	// segment..
					outputSkip0();
					fprintf(fout, "(%s line#%d, %d bytes)\n",
								   seg->fileName, lp->lineno, seg->size());
					break;

				case 'G':	// global label
					outputSkip0();
					fprintf(fout, "%s::\n", ip->data.str);
					break;

				case 'L':	// local label
					outputSkip0();
					fprintf(fout, "%s:\n", ip->data.str);
					break;

				case ';':	// comment line
					if ( !seg->isLIB )
					{
						outputSkip0();
						fprintf(fout, ";%s\n", ip->data.str);
					}
					break;

				case 'B':	// data items
					for (; ip; ip = ip->next)
					{
						int size = ip->size;
						expValue(ip, seg, slist, &v, addr+size);
						char *s = st8dasm->dasmCode(type, addr, v, size);
						if ( s != NULL ) fprintf(fout, "%s\n", s);
						addr += size;
					}
					break;

				case 'R':	// memory reserve
					addr += ip->data.val;
					break;

				case 'J':
				case 'U':
				case 'V':
					addr += lp->insert;
					break;
			}
		}
		char *s = st8dasm->dumpConst();
		if ( s != NULL ) fprintf(fout, "%s\n", s);
	}
}

void MapWriter :: outputSkip0(void)
{
	for (int i = 0; i < 24; i++) fprintf(fout, " ");
}
