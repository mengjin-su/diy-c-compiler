#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
extern "C" {
#include "common.h"
#include "lnk.h"
}
#include "ctrl.h"
#include "memory.h"
#include "segment.h"
#include "symbol.h"
#include "st8link.h"

#define INIT_CODE(s)		(strcmp(s->name(), "CODEi") == 0)
#define INIT_CODE_BEG(s)	(INIT_CODE(s) && s->isBEG)

void ST8link :: assignSegmentMem(Segment *seg_p, int mode)
{
	for (; seg_p; seg_p = seg_p->next)
	{
        int   seg_type = seg_p->type();// segment type
		char *seg_name = seg_p->name();// segment name
		int   flag = 0, addr;

		switch ( mode )
		{
			case RAM_LOC_BANK0:
			case RAM_LOC_ABS:
			case RAM_LOC_FLOAT:	addr = extendedMode? 0x0008:
												 	 0x0006;
								break;
            case ROM_LOC_ABS:
			case ROM_LOC_FLOAT:	addr = codeStartAddr;
								break;
		}

		if ( seg_p->isABS )
		{
			flag |= FIXED_ADDR_FLAG;
			addr  = seg_p->addr;
		}

        switch ( seg_type )
        {
            case CODE_SEGMENT:
            case CONST_SEGMENT:
				if ( !(flag & FIXED_ADDR_FLAG) && mode == ROM_LOC_ABS )
					continue;

				if (  (flag & FIXED_ADDR_FLAG) && mode != ROM_LOC_ABS )
					continue;

				if ( !(flag & FIXED_ADDR_FLAG) && seg_type == CONST_SEGMENT )
					flag |= _64KB_ADDR_FLAG;

				assignSegmentMem(codeMem, seg_p, addr, flag);
                break;

            case DATA_SEGMENT:
				switch ( mode )
				{
					case RAM_LOC_ABS:
						if ( !(flag & FIXED_ADDR_FLAG) )
							continue;
						break;

					case RAM_LOC_BANK0:
                		if ( (flag & FIXED_ADDR_FLAG) || strcmp(seg_name, "BANK0") )
                			continue;

						flag |= BANK0_ADDR_FLAG;
						break;

					case RAM_LOC_FLOAT:
						if ( (flag & FIXED_ADDR_FLAG) || !strcmp(seg_name, "BANK0") )
							continue;
				}
				assignSegmentMem(dataMem, seg_p, addr, flag);
                break;
        }
	}
}

void ST8link :: assignSegmentMem(Memory *mem, Segment *seg, int addr, int flag)
{
	char *sname= seg->name();	// segment name

	if ( INIT_CODE(seg) )		// init code
	{
		if ( !INIT_CODE_BEG(seg) )
			return;

		// process all init code...
		int init_size = 0;	// all those spaces are continious.
		for (Segment *isp = seg; isp && INIT_CODE(isp); isp = isp->next)
			init_size += isp->size();

		addr = mem->getSpace(addr, init_size, 0); // allow crossing pages!
		if ( addr < 0 )	// don't have memory space !!!
		{
			line_t *lp = seg->lines;
			printf("%s #%d, out of memory! --- %s\n",
					lp->fname, lp->lineno, sname? sname: "");
			errorCount++;
			return;
		}

		for (Segment *isp = seg; isp && INIT_CODE(isp); isp = isp->next)
		{
			isp->addr = addr;
			addr += isp->size();
		}
		return;
	}

	if ( mem )
		addr = mem->getSpace(addr, seg->size(), flag);

	if ( addr < 0 )	// don't have memory space !!!
	{
		line_t *lp = seg->lines;
		printf("%s #%d, out of memory! --- %s , flags = %d\n",
				lp->fname, lp->lineno, sname? sname: "", flag);
		errorCount++;
	}
	else //if ( !(flag & FIXED_ADDR_FLAG) )
	{
		seg->addr = addr;
	}
}
