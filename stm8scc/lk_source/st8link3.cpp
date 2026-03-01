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
#include "fwriter.h"

void ST8link :: outputHex(char *filename)
{
	HexWriter *hexWriter = new HexWriter(filename);
#if 1
	for (int addr = 0x8000; addr < codeMem->size; addr++)
	{
		Segment *seg;
		switch ( codeMem->content(addr) )
		{
			case  1:
			case -2:
				for (seg = codeSegGroup; seg; seg = seg->next)
					if ( seg->addr == addr && seg->size() > 0 ) break;

				if ( seg )
				{
					int a = addr;
					for (line_t *lp = seg->lines; lp; lp = lp->next)
					{
						item_t *ip = lp->items;
						switch ( lp->type )
						{
							case 'B':
								for (; ip; ip = ip->next)
								{
									int w, size = ip->size;
									expValue(ip, seg, symbList, &w, a + size);
									switch ( size )
									{
										case 4:	hexWriter->outputByte(a++, w >> 24);
										case 3:	hexWriter->outputByte(a++, w >> 16);
										case 2:	hexWriter->outputByte(a++, w >> 8);
										case 1:	hexWriter->outputByte(a++, w);
									}
								}
								break;

							case 'R':
								a += ip->data.val;
								break;
						}
					}
					if ( a > addr )	addr = a - 1;
				}
				break;
			case -1:
				if ( addr < 0x8080 && (addr & 3) == 0 )
				{
					Symbol *sym = searchSymbol(symbList, (char*)"_$iret$");
					if ( sym )
					{
						hexWriter->outputByte(addr++, 0x82);
						hexWriter->outputByte(addr++, sym->addr >> 16);
						hexWriter->outputByte(addr++, sym->addr >> 8);
						hexWriter->outputByte(addr,   sym->addr);
					}
				}
				break;
		}
	}
#else
	for (Segment *seg = codeSegGroup; seg; seg = seg->next)
	{
		int addr = seg->addr; // seg->print();
		for (line_t *lp = seg->lines; lp; lp = lp->next)
		{
			item_t *ip = lp->items;
			switch ( lp->type )
			{
				case 'B':
					for (; ip; ip = ip->next)
					{
						int w, size = ip->size;
						expValue(ip, seg, symbList, &w, addr + size);
						switch ( size )
						{
							case 4:	hexWriter->outputByte(addr++, w >> 24);
							case 3:	hexWriter->outputByte(addr++, w >> 16);
							case 2:	hexWriter->outputByte(addr++, w >> 8);
							case 1:	hexWriter->outputByte(addr++, w);
						}
					}
					break;

				case 'R':
					addr += ip->data.val;
					break;
			}
		}
	}

	// fill up unclaimed vector table items...
	Symbol *sym = searchSymbol(symbList, (char*)"_$iret$");
	if ( sym )
	{
		for (int addr = 0x8000; addr < 0x8080; addr += 4)
		{
			if ( codeMem->content(addr) == -1 )
			{
				hexWriter->outputByte(addr,   0x82);
				hexWriter->outputByte(addr+1, sym->addr >> 16);
				hexWriter->outputByte(addr+2, sym->addr >> 8);
				hexWriter->outputByte(addr+3, sym->addr);
			}
		}
	}
#endif
	delete hexWriter;
}

void ST8link :: outputMap(char *filename, Memory *data)
{
	int usedMemSize, totalMemSize;

	MapWriter *mapWriter = new MapWriter(filename);

	// output RAM map ...
	usedMemSize = dataMem->memUsed(&totalMemSize);
	mapWriter->outputSeg(symbList, usedMemSize, totalMemSize);

	// output ROM map ...
	usedMemSize = codeMem->memUsed(&totalMemSize);
	mapWriter->outputSeg(codeSegGroup, symbList, usedMemSize, totalMemSize);

	mapWriter->close();
	delete mapWriter;
}