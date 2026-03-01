#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern "C" {
#include "common.h"
}
#include "symbol.h"
#include "segment.h"
#include "st8dasm.h"

/////////////////////////////////////////////////////////////////
const ST8_inst_t ST8_instCodeTable[] = {
	{_ADC, 1, 0xa9,  	2, (ST8_REGA << 8) | ST8_IMMD},
	{_ADC, 1, 0xb9, 	2, (ST8_REGA << 8) | ST8_EXP0},
	{_ADC, 1, 0xc9,  	3, (ST8_REGA << 8) | ST8_EXP},
	{_ADC, 1, 0xf9,  	1, (ST8_REGA << 8) | ST8_INDEX_X},
	{_ADC, 1, 0xe9,  	2, (ST8_REGA << 8) | ST8_OFFSET0_X},
	{_ADC, 1, 0xd9, 	3, (ST8_REGA << 8) | ST8_OFFSET_X},
	{_ADC, 2, 0x90f9,	2, (ST8_REGA << 8) | ST8_INDEX_Y},
	{_ADC, 2, 0x90e9,	3, (ST8_REGA << 8) | ST8_OFFSET0_Y},
	{_ADC, 2, 0x90d9,	4, (ST8_REGA << 8) | ST8_OFFSET_Y},
	{_ADC, 1, 0x19,		2, (ST8_REGA << 8) | ST8_OFFSET0_SP},
	{_ADC, 2, 0x92c9,	3, (ST8_REGA << 8) | ST8_INDIR0},
	{_ADC, 2, 0x72c9,	4, (ST8_REGA << 8) | ST8_INDIR},
	{_ADC, 2, 0x92d9,	3, (ST8_REGA << 8) | ST8_INDIR0_X},
	{_ADC, 2, 0x72d9,	4, (ST8_REGA << 8) | ST8_INDIR_X},
	{_ADC, 2, 0x91d9,	3, (ST8_REGA << 8) | ST8_INDIR0_Y},

	{_ADD, 1, 0xab,  	2, (ST8_REGA << 8) | ST8_IMMD},
	{_ADD, 1, 0xbb, 	2, (ST8_REGA << 8) | ST8_EXP0},
	{_ADD, 1, 0xcb,  	3, (ST8_REGA << 8) | ST8_EXP},
	{_ADD, 1, 0xfb,  	1, (ST8_REGA << 8) | ST8_INDEX_X},
	{_ADD, 1, 0xeb,  	2, (ST8_REGA << 8) | ST8_OFFSET0_X},
	{_ADD, 1, 0xdb, 	3, (ST8_REGA << 8) | ST8_OFFSET_X},
	{_ADD, 2, 0x90fb,	2, (ST8_REGA << 8) | ST8_INDEX_Y},
	{_ADD, 2, 0x90eb,	3, (ST8_REGA << 8) | ST8_OFFSET0_Y},
	{_ADD, 2, 0x90db,	4, (ST8_REGA << 8) | ST8_OFFSET_Y},
	{_ADD, 1, 0x1b,		2, (ST8_REGA << 8) | ST8_OFFSET0_SP},
	{_ADD, 2, 0x92cb,	3, (ST8_REGA << 8) | ST8_INDIR0},
	{_ADD, 2, 0x72cb,	4, (ST8_REGA << 8) | ST8_INDIR},
	{_ADD, 2, 0x92db,	3, (ST8_REGA << 8) | ST8_INDIR0_X},
	{_ADD, 2, 0x72db,	4, (ST8_REGA << 8) | ST8_INDIR_X},
	{_ADD, 2, 0x91db,	3, (ST8_REGA << 8) | ST8_INDIR0_Y},
	{_ADD, 1, 0x5b,		2, (ST8_REGSP<< 8) | ST8_IMMD},

	{_ADDW, 1, 0x1c,	3, (ST8_REGX<< 8) | ST8_IMMD},
	{_ADDW, 2, 0x72bb,	4, (ST8_REGX<< 8) | ST8_EXP},
	{_ADDW, 2, 0x72fb,	3, (ST8_REGX<< 8) | ST8_OFFSET0_SP},
	{_ADDW, 2, 0x72a9,	4, (ST8_REGY<< 8) | ST8_IMMD},
	{_ADDW, 2, 0x72b9,	4, (ST8_REGY<< 8) | ST8_EXP},
	{_ADDW, 2, 0x72f9,	3, (ST8_REGY<< 8) | ST8_OFFSET0_SP},

	{_AND, 1, 0xa4,  	2, (ST8_REGA << 8) | ST8_IMMD},
	{_AND, 1, 0xb4, 	2, (ST8_REGA << 8) | ST8_EXP0},
	{_AND, 1, 0xc4,  	3, (ST8_REGA << 8) | ST8_EXP},
	{_AND, 1, 0xf4,  	1, (ST8_REGA << 8) | ST8_INDEX_X},
	{_AND, 1, 0xe4,  	2, (ST8_REGA << 8) | ST8_OFFSET0_X},
	{_AND, 1, 0xd4, 	3, (ST8_REGA << 8) | ST8_OFFSET_X},
	{_AND, 2, 0x90f4,	2, (ST8_REGA << 8) | ST8_INDEX_Y},
	{_AND, 2, 0x90e4,	3, (ST8_REGA << 8) | ST8_OFFSET0_Y},
	{_AND, 2, 0x90d4,	4, (ST8_REGA << 8) | ST8_OFFSET_Y},
	{_AND, 1, 0x14,		2, (ST8_REGA << 8) | ST8_OFFSET0_SP},
	{_AND, 2, 0x92c4,	3, (ST8_REGA << 8) | ST8_INDIR0},
	{_AND, 2, 0x72c4,	4, (ST8_REGA << 8) | ST8_INDIR},
	{_AND, 2, 0x92d4,	3, (ST8_REGA << 8) | ST8_INDIR0_X},
	{_AND, 2, 0x72d4,	4, (ST8_REGA << 8) | ST8_INDIR_X},
	{_AND, 2, 0x91d4,	3, (ST8_REGA << 8) | ST8_INDIR0_Y},

	{_BCP, 1, 0xa5,		2, (ST8_REGA << 8) | ST8_IMMD},
	{_BCP, 1, 0xb5, 	2, (ST8_REGA << 8) | ST8_EXP0},
	{_BCP, 1, 0xc5,  	3, (ST8_REGA << 8) | ST8_EXP},
	{_BCP, 1, 0xf5,  	1, (ST8_REGA << 8) | ST8_INDEX_X},
	{_BCP, 1, 0xe5,  	2, (ST8_REGA << 8) | ST8_OFFSET0_X},
	{_BCP, 1, 0xd5, 	3, (ST8_REGA << 8) | ST8_OFFSET_X},
	{_BCP, 2, 0x90f5,	2, (ST8_REGA << 8) | ST8_INDEX_Y},
	{_BCP, 2, 0x90e5,	3, (ST8_REGA << 8) | ST8_OFFSET0_Y},
	{_BCP, 2, 0x90d5,	4, (ST8_REGA << 8) | ST8_OFFSET_Y},
	{_BCP, 1, 0x15,		2, (ST8_REGA << 8) | ST8_OFFSET0_SP},
	{_BCP, 2, 0x92c5,	3, (ST8_REGA << 8) | ST8_INDIR0},
	{_BCP, 2, 0x72c5,	4, (ST8_REGA << 8) | ST8_INDIR},
	{_BCP, 2, 0x92d5,	3, (ST8_REGA << 8) | ST8_INDIR0_X},
	{_BCP, 2, 0x72d5,	4, (ST8_REGA << 8) | ST8_INDIR_X},
	{_BCP, 2, 0x91d5,	3, (ST8_REGA << 8) | ST8_INDIR0_Y},

	{_BCCM, 2, 0x9011,	4, (ST8_EXP << 8) | ST8_IMMD},
	{_BCPL, 2, 0x9010,	4, (ST8_EXP << 8) | ST8_IMMD},
	{_BRES, 2, 0x7211,	4, (ST8_EXP << 8) | ST8_IMMD},
	{_BSET, 2, 0x7210,	4, (ST8_EXP << 8) | ST8_IMMD},

	{_BTJF, 2, 0x7201,	5, (ST8_EXP << 16) | (ST8_IMMD << 8) | ST8_EXP},
	{_BTJT, 2, 0x7200,	5, (ST8_EXP << 16) | (ST8_IMMD << 8) | ST8_EXP},

	{_CALL, 1, 0xcd,	3, ST8_EXP},
	{_CALL, 1, 0xfd,	1, ST8_INDEX_X},
	{_CALL, 1, 0xed,	2, ST8_OFFSET0_X},
	{_CALL, 1, 0xdd,	3, ST8_OFFSET_X},
	{_CALL, 2, 0x90fd,	2, ST8_INDEX_Y},
	{_CALL, 2, 0x90ed,	3, ST8_OFFSET0_Y},
	{_CALL, 2, 0x90dd,	4, ST8_OFFSET_Y},
	{_CALL, 2, 0x92cd,	3, ST8_INDIR0},
	{_CALL, 2, 0x72cd,	4, ST8_INDIR},
	{_CALL, 2, 0x92dd,	3, ST8_INDIR0_X},
	{_CALL, 2, 0x72dd,	4, ST8_INDIR_X},
	{_CALL, 2, 0x91dd,	3, ST8_INDIR0_Y},

	{_CALLF,1, 0x8d,	4, ST8_EXP0},
	{_CALLF,2, 0x928d,	4, ST8_INDIR},

	{_CALLR,1, 0xad,	2, ST8_EXP0},
	{_CCF, 	1, 0x8c,	1, 0},

	{_CLR, 1, 0x4f,		1, ST8_REGA},
	{_CLR, 1, 0x3f, 	2, ST8_EXP0},
	{_CLR, 2, 0x725f,  	4, ST8_EXP},
	{_CLR, 1, 0x7f,  	1, ST8_INDEX_X},
	{_CLR, 1, 0x6f,  	2, ST8_OFFSET0_X},
	{_CLR, 2, 0x724f,	3, ST8_OFFSET_X},
	{_CLR, 2, 0x907f,	2, ST8_INDEX_Y},
	{_CLR, 2, 0x906f,	3, ST8_OFFSET0_Y},
	{_CLR, 2, 0x904f,	4, ST8_OFFSET_Y},
	{_CLR, 1, 0x0f,		2, ST8_OFFSET0_SP},
	{_CLR, 2, 0x923f,	3, ST8_INDIR0},
	{_CLR, 2, 0x723f,	4, ST8_INDIR},
	{_CLR, 2, 0x926f,	3, ST8_INDIR0_X},
	{_CLR, 2, 0x726f,	4, ST8_INDIR_X},
	{_CLR, 2, 0x916f,	3, ST8_INDIR0_Y},

	{_CLRW, 1, 0x5f,	1, ST8_REGX},
	{_CLRW, 2, 0x905f,	2, ST8_REGY},

	{_CP, 1, 0xa1,	2, (ST8_REGA << 8) | ST8_IMMD},
	{_CP, 1, 0xb1,	2, (ST8_REGA << 8) | ST8_EXP0},
	{_CP, 1, 0xc1, 	3, (ST8_REGA << 8) | ST8_EXP},
	{_CP, 1, 0xf1, 	1, (ST8_REGA << 8) | ST8_INDEX_X},
	{_CP, 1, 0xe1, 	2, (ST8_REGA << 8) | ST8_OFFSET0_X},
	{_CP, 1, 0xd1,	3, (ST8_REGA << 8) | ST8_OFFSET_X},
	{_CP, 2, 0x90f1,2, (ST8_REGA << 8) | ST8_INDEX_Y},
	{_CP, 2, 0x90e1,3, (ST8_REGA << 8) | ST8_OFFSET0_Y},
	{_CP, 2, 0x90d1,4, (ST8_REGA << 8) | ST8_OFFSET_Y},
	{_CP, 1, 0x11,	2, (ST8_REGA << 8) | ST8_OFFSET0_SP},
	{_CP, 2, 0x92c1,3, (ST8_REGA << 8) | ST8_INDIR0},
	{_CP, 2, 0x72c1,4, (ST8_REGA << 8) | ST8_INDIR},
	{_CP, 2, 0x92d1,3, (ST8_REGA << 8) | ST8_INDIR0_X},
	{_CP, 2, 0x72d1,4, (ST8_REGA << 8) | ST8_INDIR_X},
	{_CP, 2, 0x91d1,3, (ST8_REGA << 8) | ST8_INDIR0_Y},

	{_CPW, 1, 0xa3,	3, (ST8_REGX << 8) | ST8_IMMD},
	{_CPW, 1, 0xb3,	2, (ST8_REGX << 8) | ST8_EXP0},
	{_CPW, 1, 0xc3,  3, (ST8_REGX << 8) | ST8_EXP},
	{_CPW, 2, 0x90f3,2, (ST8_REGX << 8) | ST8_INDEX_Y},
	{_CPW, 2, 0x90e3,3, (ST8_REGX << 8) | ST8_OFFSET0_Y},
	{_CPW, 2, 0x90d3,4, (ST8_REGX << 8) | ST8_OFFSET_Y},
	{_CPW, 1, 0x13,	2, (ST8_REGX << 8) | ST8_OFFSET0_SP},
	{_CPW, 2, 0x92c3,3, (ST8_REGX << 8) | ST8_INDIR0},
	{_CPW, 2, 0x72c3,4, (ST8_REGX << 8) | ST8_INDIR},
	{_CPW, 2, 0x91d3,3, (ST8_REGX << 8) | ST8_INDIR0_Y},

	{_CPW, 2, 0x90a3,4, (ST8_REGY << 8) | ST8_IMMD},
	{_CPW, 2, 0x90b3,3, (ST8_REGY << 8) | ST8_EXP0},
	{_CPW, 2, 0x90c3,4, (ST8_REGY << 8) | ST8_EXP},
	{_CPW, 1, 0xf3,	1, (ST8_REGY << 8) | ST8_INDEX_X},
	{_CPW, 1, 0xe3,	2, (ST8_REGY << 8) | ST8_OFFSET0_X},
	{_CPW, 1, 0xd3,	3, (ST8_REGY << 8) | ST8_OFFSET_X},
	{_CPW, 2, 0x91c3,3, (ST8_REGY << 8) | ST8_INDIR0},
	{_CPW, 2, 0x92d3,3, (ST8_REGY << 8) | ST8_INDIR0_X},
	{_CPW, 2, 0x72d3,4, (ST8_REGY << 8) | ST8_INDIR_X},

	{_CPL, 1, 0x43, 	1, ST8_REGA},
	{_CPL, 1, 0x33, 	2, ST8_EXP0},
	{_CPL, 2, 0x7253,4, ST8_EXP},
	{_CPL, 1, 0x73,	1, ST8_INDEX_X},
	{_CPL, 1, 0x63,	2, ST8_OFFSET0_X},
	{_CPL, 2, 0x7243,4, ST8_OFFSET_X},
	{_CPL, 2, 0x9073,2, ST8_INDEX_Y},
	{_CPL, 2, 0x9063,3, ST8_OFFSET0_Y},
	{_CPL, 2, 0x9043,4, ST8_OFFSET_Y},
	{_CPL, 1, 0x03,	2, ST8_OFFSET0_SP},
	{_CPL, 2, 0x9233,3, ST8_INDIR0},
	{_CPL, 2, 0x7233,4, ST8_INDIR},
	{_CPL, 2, 0x9263,3, ST8_INDIR0_X},
	{_CPL, 2, 0x7263,4, ST8_INDIR_X},
	{_CPL, 2, 0x9163,3, ST8_INDIR0_Y},

	{_CPLW, 1,0x53,	1, ST8_REGX},
	{_CPLW, 2,0x9053,2, ST8_REGY},

	{_DEC, 1, 0x4a,	1, ST8_REGA},
	{_DEC, 1, 0x3a, 	2, ST8_EXP0},
	{_DEC, 2, 0x725a,4, ST8_EXP},
	{_DEC, 1, 0x7a,	1, ST8_INDEX_X},
	{_DEC, 1, 0x6a,	2, ST8_OFFSET0_X},
	{_DEC, 2, 0x724a,4, ST8_OFFSET_X},
	{_DEC, 2, 0x907a,2, ST8_INDEX_Y},
	{_DEC, 2, 0x906a,3, ST8_OFFSET0_Y},
	{_DEC, 2, 0x904a,4, ST8_OFFSET_Y},
	{_DEC, 1, 0x0a,	2, ST8_OFFSET0_SP},
	{_DEC, 2, 0x923a,3, ST8_INDIR0},
	{_DEC, 2, 0x723a,4, ST8_INDIR},
	{_DEC, 2, 0x926a,3, ST8_INDIR0_X},
	{_DEC, 2, 0x726a,4, ST8_INDIR_X},
	{_DEC, 2, 0x916a,3, ST8_INDIR0_Y},
	{_DECW,1, 0x5a,	1, ST8_REGX},
	{_DECW,2, 0x905a,2, ST8_REGY},

	{_DIV, 1, 0x62,	1, (ST8_REGX << 8) | ST8_REGA},
	{_DIV, 2, 0x9062,2, (ST8_REGY << 8) | ST8_REGA},
	{_DIVW,1, 0x65,	1, (ST8_REGX << 8) | ST8_REGY},

	{_EXG, 1, 0x41,	1, (ST8_REGA << 8) | ST8_REGXL},
	{_EXG, 1, 0x61,	1, (ST8_REGA << 8) | ST8_REGYL},
	{_EXG, 1, 0x31,	3, (ST8_REGA << 8) | ST8_EXP},
	{_EXGW,1, 0x51,	1, (ST8_REGX << 8) | ST8_REGY},
	{_HALT,1, 0x8e,	1, 0},

	{_INC, 1, 0x4c,	1, ST8_REGA},
	{_INC, 1, 0x3c, 	2, ST8_EXP0},
	{_INC, 2, 0x725c,4, ST8_EXP},
	{_INC, 1, 0x7c,	1, ST8_INDEX_X},
	{_INC, 1, 0x6c,	2, ST8_OFFSET0_X},
	{_INC, 2, 0x724c,4, ST8_OFFSET_X},
	{_INC, 2, 0x907c,2, ST8_INDEX_Y},
	{_INC, 2, 0x906c,3, ST8_OFFSET0_Y},
	{_INC, 2, 0x904c,4, ST8_OFFSET_Y},
	{_INC, 1, 0x0c,	2, ST8_OFFSET0_SP},
	{_INC, 2, 0x923c,3, ST8_INDIR0},
	{_INC, 2, 0x723c,4, ST8_INDIR},
	{_INC, 2, 0x926c,3, ST8_INDIR0_X},
	{_INC, 2, 0x726c,4, ST8_INDIR_X},
	{_INC, 2, 0x916c,3, ST8_INDIR0_Y},
	{_INCW,1, 0x5c,	1, ST8_REGX},
	{_INCW,2, 0x905c,2, ST8_REGY},

	{_IRET,1, 0x80,	1, 0},

	{_JP,  1, 0xcc,	3, ST8_EXP},
	{_JP,  1, 0xfc,  1, ST8_INDEX_X},
	{_JP,  1, 0xec,	2, ST8_OFFSET0_X},
	{_JP,  1, 0xdc,	3, ST8_OFFSET_X},
	{_JP,  2, 0x90fc,2, ST8_INDEX_Y},
	{_JP,  2, 0x90ec,3, ST8_OFFSET0_Y},
	{_JP,  2, 0x90dc,4, ST8_OFFSET_Y},
	{_JP,  2, 0x92cc,3, ST8_INDIR0},
	{_JP,  2, 0x72cc,4, ST8_INDIR},
	{_JP,  2, 0x92dc,3, ST8_INDIR0_X},
	{_JP,  2, 0x72dc,4, ST8_INDIR_X},
	{_JP,  2, 0x91dc,3, ST8_INDIR0_Y},

	{_JPF, 1, 0xac,  4, ST8_EXP},
	{_JPF, 2, 0x92ac,4, ST8_INDIR},

	{_JRA,   1, 0x20,  2, ST8_EXP},
	{_JRC,   1, 0x25,  2, ST8_EXP},
	{_JREQ,  1, 0x27,  2, ST8_EXP},
	{_JRF,   1, 0x21,  2, ST8_EXP},
	{_JRH,   2, 0x9029,3, ST8_EXP},
	{_JRIH,  2, 0x902f,3, ST8_EXP},
	{_JRIL,  2, 0x902e,3, ST8_EXP},
	{_JRM,   2, 0x902d,3, ST8_EXP},
	{_JRMI,  1, 0x2b,  2, ST8_EXP},
	{_JRNC,  1, 0x24,  2, ST8_EXP},
	{_JRNE,  1, 0x26,  2, ST8_EXP},
	{_JRNH,  2, 0x9028,3, ST8_EXP},
	{_JRNM,  2, 0x902c,3, ST8_EXP},
	{_JRNV,  1, 0x28,  2, ST8_EXP},
	{_JRPL,  1, 0x2a,  2, ST8_EXP},
	{_JRSGE, 1, 0x2e,  2, ST8_EXP},
	{_JRSGT, 1, 0x2c,  2, ST8_EXP},
	{_JRSLE, 1, 0x2d,  2, ST8_EXP},
	{_JRSLT, 1, 0x2f,  2, ST8_EXP},
	{_JRT,   1, 0x20,  2, ST8_EXP},
	{_JRUGE, 1, 0x24,  2, ST8_EXP},
	{_JRUGT, 1, 0x22,  2, ST8_EXP},
	{_JRULE, 1, 0x23,  2, ST8_EXP},
	{_JRULT, 1, 0x25,  2, ST8_EXP},
	{_JRV,   1, 0x29,  2, ST8_EXP},

	{_LD, 1, 0xa6,	2, (ST8_REGA << 8) | ST8_IMMD},
	{_LD, 1, 0xb6, 	2, (ST8_REGA << 8) | ST8_EXP0},
	{_LD, 1, 0xc6,   3, (ST8_REGA << 8) | ST8_EXP},
	{_LD, 1, 0xf6,	1, (ST8_REGA << 8) | ST8_INDEX_X},
	{_LD, 1, 0xe6,	2, (ST8_REGA << 8) | ST8_OFFSET0_X},
	{_LD, 1, 0xd6,   4, (ST8_REGA << 8) | ST8_OFFSET_X},
	{_LD, 2, 0x90f6, 2, (ST8_REGA << 8) | ST8_INDEX_Y},
	{_LD, 2, 0x90e6, 3, (ST8_REGA << 8) | ST8_OFFSET0_Y},
	{_LD, 2, 0x90d6, 4, (ST8_REGA << 8) | ST8_OFFSET_Y},
	{_LD, 1, 0x7b,	2, (ST8_REGA << 8) | ST8_OFFSET0_SP},
	{_LD, 2, 0x92c6, 3, (ST8_REGA << 8) | ST8_INDIR0},
	{_LD, 2, 0x72c6, 4, (ST8_REGA << 8) | ST8_INDIR},
	{_LD, 2, 0x92d6, 3, (ST8_REGA << 8) | ST8_INDIR0_X},
	{_LD, 2, 0x72d6, 4, (ST8_REGA << 8) | ST8_INDIR_X},
	{_LD, 2, 0x91d6, 3, (ST8_REGA << 8) | ST8_INDIR0_Y},

	{_LD, 1, 0xb7, 	 2, (ST8_EXP0 << 8 ) | ST8_REGA},
	{_LD, 1, 0xc7,   3, (ST8_EXP << 8) | ST8_REGA},
	{_LD, 1, 0xf7,	 1, (ST8_INDEX_X << 8) | ST8_REGA},
	{_LD, 1, 0xe7,	 2, (ST8_OFFSET0_X << 8) | ST8_REGA},
	{_LD, 1, 0xd7,   4, (ST8_OFFSET_X << 8) | ST8_REGA},
	{_LD, 2, 0x90f7, 2, (ST8_INDEX_Y << 8) | ST8_REGA},
	{_LD, 2, 0x90e7, 3, (ST8_OFFSET0_Y << 8) | ST8_REGA},
	{_LD, 2, 0x90d7, 4, (ST8_OFFSET_Y << 8) | ST8_REGA},
	{_LD, 1, 0x6b,	 2, (ST8_OFFSET0_SP << 8) | ST8_REGA},
	{_LD, 2, 0x92c7, 3, (ST8_INDIR0 << 8) | ST8_REGA},
	{_LD, 2, 0x72c7, 4, (ST8_INDIR << 8) | ST8_REGA},
	{_LD, 2, 0x92d7, 3, (ST8_INDIR0_X << 8) | ST8_REGA},
	{_LD, 2, 0x72d7, 4, (ST8_INDIR_X << 8) | ST8_REGA},
	{_LD, 2, 0x91d7, 3, (ST8_INDIR0_Y << 8) | ST8_REGA},

	{_LD, 1, 0x97,   1, (ST8_REGXL << 8) | ST8_REGA},
	{_LD, 1, 0x9f,   1, (ST8_REGA << 8) | ST8_REGXL},
	{_LD, 2, 0x9097, 2, (ST8_REGYL << 8) | ST8_REGA},
	{_LD, 2, 0x909f, 2, (ST8_REGA << 8) | ST8_REGYL},
	{_LD, 1, 0x95,   1, (ST8_REGXH << 8) | ST8_REGA},
	{_LD, 1, 0x9e,   1, (ST8_REGA << 8) | ST8_REGXH},
	{_LD, 2, 0x9095, 2, (ST8_REGYH << 8) | ST8_REGA},
	{_LD, 2, 0x909e, 2, (ST8_REGA << 8) | ST8_REGYH},

	{_LDF,1, 0xbc, 	4, (ST8_REGA << 8) | ST8_EXP},
	{_LDF,1, 0xaf, 	4, (ST8_REGA << 8) | ST8_OFFSET_X},
	{_LDF,2, 0x90af,5, (ST8_REGA << 8) | ST8_OFFSET_Y},
	{_LDF,2, 0x92af,4, (ST8_REGA << 8) | ST8_INDIR_X},
	{_LDF,2, 0x91af,4, (ST8_REGA << 8) | ST8_INDIR_Y},
	{_LDF,2, 0x92bc,4, (ST8_REGA << 8) | ST8_INDIR},
	{_LDF,1, 0xbd,	4, (ST8_EXP << 8) | ST8_REGA},
	{_LDF,1, 0xa7,	4, (ST8_OFFSET_X << 8) | ST8_REGA},
	{_LDF,2, 0x90a7,5, (ST8_OFFSET_Y << 8) | ST8_REGA},
	{_LDF,2, 0x92a7,4, (ST8_INDIR_X << 8) | ST8_REGA},
	{_LDF,2, 0x91a7,4, (ST8_INDIR_Y << 8) | ST8_REGA},
	{_LDF,2, 0x92bd,4, (ST8_INDIR << 8) | ST8_REGA},

	{_LDW,1, 0xae,	3, (ST8_REGX << 8) | ST8_IMMD},
	{_LDW,1, 0xbe,	2, (ST8_REGX << 8) | ST8_EXP0},
	{_LDW,1, 0xce,	3, (ST8_REGX << 8) | ST8_EXP},
	{_LDW,1, 0xfe,	1, (ST8_REGX << 8) | ST8_INDEX_X},
	{_LDW,1, 0xee,	2, (ST8_REGX << 8) | ST8_OFFSET0_X},
	{_LDW,1, 0xde,	3, (ST8_REGX << 8) | ST8_OFFSET_X},
	{_LDW,1, 0x1e,	2, (ST8_REGX << 8) | ST8_OFFSET0_SP},
	{_LDW,2, 0x92ce,	3, (ST8_REGX << 8) | ST8_INDIR0},
	{_LDW,2, 0x72ce,	4, (ST8_REGX << 8) | ST8_INDIR},
	{_LDW,2, 0x92de,	3, (ST8_REGX << 8) | ST8_INDIR0_X},
	{_LDW,2, 0x72de,	4, (ST8_REGX << 8) | ST8_INDIR_X},
	{_LDW,1, 0xbf,	2, (ST8_EXP0 << 8) | ST8_REGX},
	{_LDW,1, 0xcf,	3, (ST8_EXP  << 8) | ST8_REGX},
	{_LDW,1, 0xff,	1, (ST8_INDEX_X << 8) | ST8_REGY},
	{_LDW,1, 0xef,	2, (ST8_OFFSET0_X << 8) | ST8_REGY},
	{_LDW,1, 0xdf,	3, (ST8_OFFSET_X << 8) | ST8_REGY},
	{_LDW,1, 0x1f,	2, (ST8_OFFSET0_SP << 8) | ST8_REGX},
	{_LDW,2, 0x92cf,	3, (ST8_INDIR0 << 8) | ST8_REGX},
	{_LDW,2, 0x72cf,	4, (ST8_INDIR << 8) | ST8_REGX},
	{_LDW,2, 0x92df,	3, (ST8_INDIR0_X << 8) | ST8_REGY},
	{_LDW,2, 0x72df,	4, (ST8_INDIR_X << 8) | ST8_REGY},

	{_LDW,2, 0x90ae,	4, (ST8_REGY << 8) | ST8_IMMD},
	{_LDW,2, 0x90be,	3, (ST8_REGY << 8) | ST8_EXP0},
	{_LDW,2, 0x90ce,	4, (ST8_REGY << 8) | ST8_EXP},
	{_LDW,2, 0x90fe,	2, (ST8_REGY << 8) | ST8_INDEX_Y},
	{_LDW,2, 0x90ee,	3, (ST8_REGY << 8) | ST8_OFFSET0_Y},
	{_LDW,2, 0x90de,	4, (ST8_REGY << 8) | ST8_OFFSET_Y},
	{_LDW,1, 0x16,	2, (ST8_REGY << 8) | ST8_OFFSET0_SP},
	{_LDW,2, 0x91ce,	3, (ST8_REGY << 8) | ST8_INDIR0},
	{_LDW,2, 0x91de,	3, (ST8_REGY << 8) | ST8_INDIR0_X},

	{_LDW,2, 0x90bf,	3, (ST8_EXP0 << 8) | ST8_REGY},
	{_LDW,2, 0x90cf,	4, (ST8_EXP  << 8) | ST8_REGY},
	{_LDW,2, 0x90ff,	2, (ST8_INDEX_Y << 8) | ST8_REGX},
	{_LDW,2, 0x90ef,	3, (ST8_OFFSET0_Y << 8) | ST8_REGX},
	{_LDW,2, 0x90df,	4, (ST8_OFFSET_Y << 8) | ST8_REGX},
	{_LDW,1, 0x17,	2, (ST8_OFFSET0_SP << 8) | ST8_REGY},
	{_LDW,2, 0x91cf,	3, (ST8_INDIR0 << 8) | ST8_REGY},
	{_LDW,2, 0x91df,	3, (ST8_INDIR0_Y << 8) | ST8_REGX},

	{_LDW,2, 0x92cf,	3, (ST8_INDIR0 << 8) | ST8_REGX},
	{_LDW,2, 0x72cf,	4, (ST8_INDIR << 8) | ST8_REGX},
	{_LDW,2, 0x92df,	3, (ST8_INDIR0_X << 8) | ST8_REGY},
	{_LDW,2, 0x72df,	4, (ST8_INDIR_X << 8) | ST8_REGY},

	{_LDW,2, 0x9093,	2, (ST8_REGY << 8) | ST8_REGX},
	{_LDW,1, 0x93,	1, (ST8_REGX << 8) | ST8_REGY},
	{_LDW,1, 0x96,	1, (ST8_REGX << 8) | ST8_REGSP},
	{_LDW,1, 0x94,	1, (ST8_REGSP << 8) | ST8_REGX},
	{_LDW,2, 0x9096,	2, (ST8_REGY << 8) | ST8_REGSP},
	{_LDW,2, 0x9094,	2, (ST8_REGSP << 8) | ST8_REGY},

	{_MOV,1,0x35,	4, (ST8_EXP << 8) | ST8_IMMD},
	{_MOV,1,0x45,	3, (ST8_EXP0 << 8) | ST8_EXP0},
	{_MOV,1,0x55,	5, (ST8_EXP << 8) | ST8_EXP},

	{_MUL,1,0x42,	1, (ST8_REGX << 8) | ST8_REGA},
	{_MUL,2,0x9042,	2, (ST8_REGY << 8) | ST8_REGA},

	{_NEG,1,0x40,	1, ST8_REGA},
	{_NEG,1,0x30,	2, ST8_EXP0},
	{_NEG,2,0x7250,	4, ST8_EXP},
	{_NEG,1,0x70,	1, ST8_INDEX_X},
	{_NEG,1,0x60,	2, ST8_OFFSET0_X},
	{_NEG,2,0x7240,	4, ST8_OFFSET_X},
	{_NEG,2,0x9070,	2, ST8_INDEX_Y},
	{_NEG,2,0x9060,	3, ST8_OFFSET0_Y},
	{_NEG,2,0x9040,	4, ST8_OFFSET_Y},
	{_NEG,1,0x00,	2, ST8_OFFSET0_SP},
	{_NEG,2,0x9230,	3, ST8_INDIR0},
	{_NEG,2,0x7230,	4, ST8_INDIR},
	{_NEG,2,0x9260,	3, ST8_INDIR0_X},
	{_NEG,2,0x7260,	4, ST8_INDIR_X},
	{_NEG,2,0x9160,	3, ST8_INDIR0_Y},
	{_NEGW,1,0x50,	1, ST8_REGX},
	{_NEGW,2,0x9050,	2, ST8_REGY},

	{_NOP, 1, 0x9d,	1, 0},

	{_OR, 1, 0xaa,  	2, (ST8_REGA << 8) | ST8_IMMD},
	{_OR, 1, 0xba, 	2, (ST8_REGA << 8) | ST8_EXP0},
	{_OR, 1, 0xca,  	3, (ST8_REGA << 8) | ST8_EXP},
	{_OR, 1, 0xfa,  	1, (ST8_REGA << 8) | ST8_INDEX_X},
	{_OR, 1, 0xea,  	2, (ST8_REGA << 8) | ST8_OFFSET0_X},
	{_OR, 1, 0xda, 	3, (ST8_REGA << 8) | ST8_OFFSET_X},
	{_OR, 2, 0x90fa,	2, (ST8_REGA << 8) | ST8_INDEX_Y},
	{_OR, 2, 0x90ea,	3, (ST8_REGA << 8) | ST8_OFFSET0_Y},
	{_OR, 2, 0x90da,	4, (ST8_REGA << 8) | ST8_OFFSET_Y},
	{_OR, 1, 0x1a,	2, (ST8_REGA << 8) | ST8_OFFSET0_SP},
	{_OR, 2, 0x92ca,	3, (ST8_REGA << 8) | ST8_INDIR0},
	{_OR, 2, 0x72ca,	4, (ST8_REGA << 8) | ST8_INDIR},
	{_OR, 2, 0x92da,	3, (ST8_REGA << 8) | ST8_INDIR0_X},
	{_OR, 2, 0x72da,	4, (ST8_REGA << 8) | ST8_INDIR_X},
	{_OR, 2, 0x91da,	3, (ST8_REGA << 8) | ST8_INDIR0_Y},

	{_POP,1, 0x84,	1, ST8_REGA},
	{_POP,1, 0x86,	1, ST8_REGCC},
	{_POP,1, 0x32,	3, ST8_EXP},
	{_POPW,1,0x85,	1, ST8_REGX},
	{_POPW,2,0x9085,	2, ST8_REGY},

	{_PUSH,1, 0x88,	1, ST8_REGA},
	{_PUSH,1, 0x8a,	1, ST8_REGCC},
	{_PUSH,1, 0x3b,	3, ST8_EXP},
	{_PUSH,1, 0x4b,	2, ST8_IMMD},
	{_PUSHW,1,0x89,	1, ST8_REGX},
	{_PUSHW,2,0x9089,2, ST8_REGY},

	{_RCF, 1, 0x98,	1, 0},
	{_RET, 1, 0x81, 	1, 0},
	{_RETF,1, 0x87, 	1, 0},
	{_RIM, 1, 0x9a,  1, 0},

	{_RLC,1,0x49,	1, ST8_REGA},
	{_RLC,1,0x39,	2, ST8_EXP0},
	{_RLC,2,0x7259,	4, ST8_EXP},
	{_RLC,1,0x79,	1, ST8_INDEX_X},
	{_RLC,1,0x69,	2, ST8_OFFSET0_X},
	{_RLC,2,0x7249,	4, ST8_OFFSET_X},
	{_RLC,2,0x9079,	2, ST8_INDEX_Y},
	{_RLC,2,0x9069,	3, ST8_OFFSET0_Y},
	{_RLC,2,0x9049,	4, ST8_OFFSET_Y},
	{_RLC,1,0x09,	2, ST8_OFFSET0_SP},
	{_RLC,2,0x9239,	3, ST8_INDIR0},
	{_RLC,2,0x7239,	4, ST8_INDIR},
	{_RLC,2,0x9269,	3, ST8_INDIR0_X},
	{_RLC,2,0x7269,	4, ST8_INDIR_X},
	{_RLC,2,0x9169,	3, ST8_INDIR0_Y},
	{_RLCW,1,0x59,	1, ST8_REGX},
	{_RLCW,2,0x9059,2, ST8_REGY},

	{_RLWA,1,0x02,	1, (ST8_REGX << 8) | ST8_REGA},
	{_RLWA,2,0x9002,2, (ST8_REGY << 8) | ST8_REGA},

	{_RRC,1,0x46,	1, ST8_REGA},
	{_RRC,1,0x36,	2, ST8_EXP0},
	{_RRC,2,0x7256,	4, ST8_EXP},
	{_RRC,1,0x76,	1, ST8_INDEX_X},
	{_RRC,1,0x66,	2, ST8_OFFSET0_X},
	{_RRC,2,0x7246,	4, ST8_OFFSET_X},
	{_RRC,2,0x9076,	2, ST8_INDEX_Y},
	{_RRC,2,0x9066,	3, ST8_OFFSET0_Y},
	{_RRC,2,0x9046,	4, ST8_OFFSET_Y},
	{_RRC,1,0x06,	2, ST8_OFFSET0_SP},
	{_RRC,2,0x9236,	3, ST8_INDIR0},
	{_RRC,2,0x7236,	4, ST8_INDIR},
	{_RRC,2,0x9266,	3, ST8_INDIR0_X},
	{_RRC,2,0x7266,	4, ST8_INDIR_X},
	{_RRC,2,0x9166,	3, ST8_INDIR0_Y},
	{_RRCW,1,0x56,	1, ST8_REGX},
	{_RRCW,2,0x9056,2, ST8_REGY},

	{_RRWA,1,0x01,	1, (ST8_REGX << 8) | ST8_REGA},
	{_RRWA,2,0x9001,2, (ST8_REGY << 8) | ST8_REGA},

	{_RVF, 1, 0x9c,	1, 0},

	{_SBC, 1, 0xa2,  	2, (ST8_REGA << 8) | ST8_IMMD},
	{_SBC, 1, 0xb2, 	2, (ST8_REGA << 8) | ST8_EXP0},
	{_SBC, 1, 0xc2,  	3, (ST8_REGA << 8) | ST8_EXP},
	{_SBC, 1, 0xf2,  	1, (ST8_REGA << 8) | ST8_INDEX_X},
	{_SBC, 1, 0xe2,  	2, (ST8_REGA << 8) | ST8_OFFSET0_X},
	{_SBC, 1, 0xd2, 	3, (ST8_REGA << 8) | ST8_OFFSET_X},
	{_SBC, 2, 0x90f2,	2, (ST8_REGA << 8) | ST8_INDEX_Y},
	{_SBC, 2, 0x90e2,	3, (ST8_REGA << 8) | ST8_OFFSET0_Y},
	{_SBC, 2, 0x90d2,	4, (ST8_REGA << 8) | ST8_OFFSET_Y},
	{_SBC, 1, 0x12,		2, (ST8_REGA << 8) | ST8_OFFSET0_SP},
	{_SBC, 2, 0x92c2,	3, (ST8_REGA << 8) | ST8_INDIR0},
	{_SBC, 2, 0x72c2,	4, (ST8_REGA << 8) | ST8_INDIR},
	{_SBC, 2, 0x92d2,	3, (ST8_REGA << 8) | ST8_INDIR0_X},
	{_SBC, 2, 0x72d2,	4, (ST8_REGA << 8) | ST8_INDIR_X},
	{_SBC, 2, 0x91d2,	3, (ST8_REGA << 8) | ST8_INDIR0_Y},

	{_SCF, 1, 0x99,		1, 0},
	{_SIM, 1, 0x9b,		1, 0},

	{_SLL,1,0x48,	1, ST8_REGA},
	{_SLL,1,0x38,	2, ST8_EXP0},
	{_SLL,2,0x7258,	4, ST8_EXP},
	{_SLL,1,0x78,	1, ST8_INDEX_X},
	{_SLL,1,0x68,	2, ST8_OFFSET0_X},
	{_SLL,2,0x7248,	4, ST8_OFFSET_X},
	{_SLL,2,0x9078,	2, ST8_INDEX_Y},
	{_SLL,2,0x9068,	3, ST8_OFFSET0_Y},
	{_SLL,2,0x9048,	4, ST8_OFFSET_Y},
	{_SLL,1,0x08,	2, ST8_OFFSET0_SP},
	{_SLL,2,0x9238,	3, ST8_INDIR0},
	{_SLL,2,0x7238,	4, ST8_INDIR},
	{_SLL,2,0x9268,	3, ST8_INDIR0_X},
	{_SLL,2,0x7268,	4, ST8_INDIR_X},
	{_SLL,2,0x9168,	3, ST8_INDIR0_Y},
	{_SLLW,1,0x58,	1, ST8_REGX},
	{_SLLW,2,0x9058,	2, ST8_REGY},

	{_SRA,1,0x47,	1, ST8_REGA},
	{_SRA,1,0x37,	2, ST8_EXP0},
	{_SRA,2,0x7257,	4, ST8_EXP},
	{_SRA,1,0x77,	1, ST8_INDEX_X},
	{_SRA,1,0x67,	2, ST8_OFFSET0_X},
	{_SRA,2,0x7247,	4, ST8_OFFSET_X},
	{_SRA,2,0x9077,	2, ST8_INDEX_Y},
	{_SRA,2,0x9067,	3, ST8_OFFSET0_Y},
	{_SRA,2,0x9047,	4, ST8_OFFSET_Y},
	{_SRA,1,0x07,	2, ST8_OFFSET0_SP},
	{_SRA,2,0x9237,	3, ST8_INDIR0},
	{_SRA,2,0x7237,	4, ST8_INDIR},
	{_SRA,2,0x9267,	3, ST8_INDIR0_X},
	{_SRA,2,0x7267,	4, ST8_INDIR_X},
	{_SRA,2,0x9167,	3, ST8_INDIR0_Y},
	{_SRAW,1,0x57,	1, ST8_REGX},
	{_SRAW,2,0x9057,	2, ST8_REGY},

	{_SRL,1,0x44,		1, ST8_REGA},
	{_SRL,1,0x34,		2, ST8_EXP0},
	{_SRL,2,0x7254,		4, ST8_EXP},
	{_SRL,1,0x74,		1, ST8_INDEX_X},
	{_SRL,1,0x64,		2, ST8_OFFSET0_X},
	{_SRL,2,0x7244,		4, ST8_OFFSET_X},
	{_SRL,2,0x9074,		2, ST8_INDEX_Y},
	{_SRL,2,0x9064,		3, ST8_OFFSET0_Y},
	{_SRL,2,0x9044,		4, ST8_OFFSET_Y},
	{_SRL,1,0x04,		2, ST8_OFFSET0_SP},
	{_SRL,2,0x9234,		3, ST8_INDIR0},
	{_SRL,2,0x7234,		4, ST8_INDIR},
	{_SRL,2,0x9264,		3, ST8_INDIR0_X},
	{_SRL,2,0x7264,		4, ST8_INDIR_X},
	{_SRL,2,0x9164,		3, ST8_INDIR0_Y},
	{_SRLW,1,0x54,		1, ST8_REGX},
	{_SRLW,2,0x9054,	2, ST8_REGY},

	{_SUB, 1, 0xa0,  	2, (ST8_REGA << 8) | ST8_IMMD},
	{_SUB, 1, 0xb0, 	2, (ST8_REGA << 8) | ST8_EXP0},
	{_SUB, 1, 0xc0,  	3, (ST8_REGA << 8) | ST8_EXP},
	{_SUB, 1, 0xf0,  	1, (ST8_REGA << 8) | ST8_INDEX_X},
	{_SUB, 1, 0xe0,  	2, (ST8_REGA << 8) | ST8_OFFSET0_X},
	{_SUB, 1, 0xd0, 	3, (ST8_REGA << 8) | ST8_OFFSET_X},
	{_SUB, 2, 0x90f0,	2, (ST8_REGA << 8) | ST8_INDEX_Y},
	{_SUB, 2, 0x90e0,	3, (ST8_REGA << 8) | ST8_OFFSET0_Y},
	{_SUB, 2, 0x90d0,	4, (ST8_REGA << 8) | ST8_OFFSET_Y},
	{_SUB, 1, 0x10,		2, (ST8_REGA << 8) | ST8_OFFSET0_SP},
	{_SUB, 2, 0x92c0,	3, (ST8_REGA << 8) | ST8_INDIR0},
	{_SUB, 2, 0x72c0,	4, (ST8_REGA << 8) | ST8_INDIR},
	{_SUB, 2, 0x92d0,	3, (ST8_REGA << 8) | ST8_INDIR0_X},
	{_SUB, 2, 0x72d0,	4, (ST8_REGA << 8) | ST8_INDIR_X},
	{_SUB, 2, 0x91d0,	3, (ST8_REGA << 8) | ST8_INDIR0_Y},
	{_SUB, 1, 0x52,		2, (ST8_REGSP<< 8) | ST8_IMMD},

	{_SUBW, 1, 0x1d,	3, (ST8_REGX<< 8) | ST8_IMMD},
	{_SUBW, 2, 0x72b0,	4, (ST8_REGX<< 8) | ST8_EXP},
	{_SUBW, 2, 0x72f0,	3, (ST8_REGX<< 8) | ST8_OFFSET0_SP},
	{_SUBW, 2, 0x72a2,	4, (ST8_REGY<< 8) | ST8_IMMD},
	{_SUBW, 2, 0x72b2,	4, (ST8_REGY<< 8) | ST8_EXP},
	{_SUBW, 2, 0x72f2,	3, (ST8_REGY<< 8) | ST8_OFFSET0_SP},

	{_SWAP,1,0x4e,		1, ST8_REGA},
	{_SWAP,1,0x3e,		2, ST8_EXP0},
	{_SWAP,2,0x725e,	4, ST8_EXP},
	{_SWAP,1,0x7e,		1, ST8_INDEX_X},
	{_SWAP,1,0x6e,		2, ST8_OFFSET0_X},
	{_SWAP,2,0x724e,	4, ST8_OFFSET_X},
	{_SWAP,2,0x907e,	2, ST8_INDEX_Y},
	{_SWAP,2,0x906e,	3, ST8_OFFSET0_Y},
	{_SWAP,2,0x904e,	4, ST8_OFFSET_Y},
	{_SWAP,1,0x0e,		2, ST8_OFFSET0_SP},
	{_SWAP,2,0x923e,	3, ST8_INDIR0},
	{_SWAP,2,0x723e,	4, ST8_INDIR},
	{_SWAP,2,0x926e,	3, ST8_INDIR0_X},
	{_SWAP,2,0x726e,	4, ST8_INDIR_X},
	{_SWAP,2,0x916e,	3, ST8_INDIR0_Y},
	{_SWAPW,1,0x5e,		1, ST8_REGX},
	{_SWAPW,2,0x905e,	2, ST8_REGY},

	{_TNZ,1,0x4d,		1, ST8_REGA},
	{_TNZ,1,0x3d,		2, ST8_EXP0},
	{_TNZ,2,0x725d,		4, ST8_EXP},
	{_TNZ,1,0x7d,		1, ST8_INDEX_X},
	{_TNZ,1,0x6d,		2, ST8_OFFSET0_X},
	{_TNZ,2,0x724d,		4, ST8_OFFSET_X},
	{_TNZ,2,0x907d,		2, ST8_INDEX_Y},
	{_TNZ,2,0x906d,		3, ST8_OFFSET0_Y},
	{_TNZ,2,0x904d,		4, ST8_OFFSET_Y},
	{_TNZ,1,0x0d,		2, ST8_OFFSET0_SP},
	{_TNZ,2,0x923d,		3, ST8_INDIR0},
	{_TNZ,2,0x723d,		4, ST8_INDIR},
	{_TNZ,2,0x926d,		3, ST8_INDIR0_X},
	{_TNZ,2,0x726d,		4, ST8_INDIR_X},
	{_TNZ,2,0x916d,		3, ST8_INDIR0_Y},
	{_TNZW,1,0x5d,		1, ST8_REGX},
	{_TNZW,2,0x905d,	2, ST8_REGY},

	{_TRAP,1,0x83,		1, 0},
	{_WFE, 2,0x728f,	2, 0},
	{_WFI, 1,0x8f,		1, 0},

	{_XOR,1, 0xa8,  	2, (ST8_REGA << 8) | ST8_IMMD},
	{_XOR,1, 0xb8, 		2, (ST8_REGA << 8) | ST8_EXP0},
	{_XOR,1, 0xc8,  	3, (ST8_REGA << 8) | ST8_EXP},
	{_XOR,1, 0xf8,  	1, (ST8_REGA << 8) | ST8_INDEX_X},
	{_XOR,1, 0xe8,  	2, (ST8_REGA << 8) | ST8_OFFSET0_X},
	{_XOR,1, 0xd8, 		3, (ST8_REGA << 8) | ST8_OFFSET_X},
	{_XOR,2, 0x90f8,	2, (ST8_REGA << 8) | ST8_INDEX_Y},
	{_XOR,2, 0x90e8,	3, (ST8_REGA << 8) | ST8_OFFSET0_Y},
	{_XOR,2, 0x90d8,	4, (ST8_REGA << 8) | ST8_OFFSET_Y},
	{_XOR,1, 0x18,		2, (ST8_REGA << 8) | ST8_OFFSET0_SP},
	{_XOR,2, 0x92c8,	3, (ST8_REGA << 8) | ST8_INDIR0},
	{_XOR,2, 0x72c8,	4, (ST8_REGA << 8) | ST8_INDIR},
	{_XOR,2, 0x92d8,	3, (ST8_REGA << 8) | ST8_INDIR0_X},
	{_XOR,2, 0x72d8,	4, (ST8_REGA << 8) | ST8_INDIR_X},
	{_XOR,2, 0x91d8,	3, (ST8_REGA << 8) | ST8_INDIR0_Y},
	{_VECT,1,0x82, 		4, ST8_EXP},

	{(char*)NULL, 0, 0, 0, 0}
};


#define CODE_FIELD_LENGTH	28

static char *ST8_reg(int oper);
static unsigned int ST8_relAddr(int inst_size, unsigned char offset);
static int  ST8_operCnt(ST8_inst_t *p);
static int  ST8_getOper(ST8_inst_t *p, int index);
static unsigned int ST8_getVal(unsigned char *buf, int length);

STM8S_dasm :: STM8S_dasm(Symbol *slist)
{
	codeAddr    = 0;
	codeBufCnt  = 0;
	segmentType = -1;
	symbList = slist;
	outputReady = false;
}

STM8S_dasm :: ~STM8S_dasm()
{
}

char *STM8S_dasm :: dasmCode(int type, unsigned int addr, unsigned int value, int size)
{
	// address breaks ...
	if ( (codeAddr + codeBufCnt) != addr || segmentType != type )
	{
//		dumpCode();
		codeBufCnt = 0;
	}

	segmentType = type;

	// an instruction starts
	if ( codeBufCnt == 0 )
		codeAddr = addr;

	// big-endian format
	while ( size-- )
		codeBuf[codeBufCnt++] = (value >> (size*8)) & 0xff;

	// disassemble code...
	dasmCode();

	if ( outputReady )
	{
		outputReady = false;
		return buf;
	}
	return NULL;
}

void STM8S_dasm :: dasmCode(void)
{
	ST8_inst_t *p = (ST8_inst_t *)ST8_instCodeTable;

	if ( segmentType == CONST_SEGMENT ) {
		if ( codeBufCnt >= 16 )
			outputReady = (dumpConst() != NULL);

		return;
	}

	for (; p->mnemonic; p++)
	{
		if ( codeBufCnt < p->instSize )
			continue;

		if ( codeBufCnt > 2 && p->codeSize == 2 &&
			 (p->operands == ((ST8_EXP << 8)  | ST8_IMMD) 				 ||
			  p->operands == ((ST8_EXP << 16) | (ST8_IMMD << 8) | ST8_EXP)) )
		{
			if ( codeBuf[0] == ((p->code >> 8) & 0xff) &&
				 (codeBuf[1] & 0xf1) == (char)(p->code & 0xff) )
				break;
		}
		else if ( p->codeSize == 1 )
		{
			if ( codeBuf[0] == (p->code & 0xff) )
				break;
		}
		else
		{
			if ( codeBuf[0] == ((p->code >> 8) & 0xff) && codeBuf[1] == (p->code & 0xff) )
				break;
		}
	}

	if ( p->mnemonic && codeBufCnt >= p->instSize )
	{
		dasmCode(p);

		memcpy(codeBuf, &codeBuf[p->instSize], codeBufCnt - p->instSize);
//		printf("%s: instSize=%d\n", p->symbl, p->instSize);
		codeBufCnt -= p->instSize;
	}
}

void STM8S_dasm :: dasmCode(ST8_inst_t *ip)
{
	sprintf(buf, "%05X:", codeAddr);

	for (unsigned int i = 0; i < ip->instSize; i++)
		sprintf(&buf[strlen(buf)], " %02X", (int)(codeBuf[i] & 0xff));

	for (int i = strlen(buf); i < CODE_FIELD_LENGTH; i++)
		strcat(buf, " ");

	strcat(buf, ip->mnemonic);

	if ( ip->operands )
		for (int i = strlen(ip->mnemonic); i < 8; i++)
			strcat(buf, " ");

	int l = strlen(buf);
	int operCnt = ST8_operCnt(ip);	// operand count
	int offset  = ip->codeSize;		// operand offset
	int actual_addr;

	switch ( ip->operands )
	{
		case (ST8_EXP << 8) | ST8_IMMD:
			if ( (ip->instSize - offset) <= 2 )	// bres PORT, #n
			{
				actual_addr = ST8_getVal(&codeBuf[codeBufCnt-2], ip->instSize - offset);
				ST8_appendAddr(&buf[l], actual_addr);

				sprintf(&buf[strlen(buf)], ", #%d", (codeBuf[1] >> 1) & 7);
			}
			else								// mov addr, #N
			{
				actual_addr = ST8_getVal(&codeBuf[offset+1], 2);
				ST8_appendAddr(&buf[l], actual_addr);

				sprintf(&buf[strlen(buf)], ", #0x%X", ST8_getVal(&codeBuf[offset], 1));
			}
			break;

		case (ST8_EXP << 16) | (ST8_IMMD << 8) | ST8_EXP:	// btjf PORT, #n, rel_addr
			actual_addr = ST8_getVal(&codeBuf[codeBufCnt-3], 2);
			ST8_appendAddr(&buf[l], actual_addr);

			sprintf(&buf[strlen(buf)], ", #%d, ", (codeBuf[1] >> 1) & 7);

			actual_addr = ST8_relAddr(ip->instSize, codeBuf[codeBufCnt-1]);
			ST8_appendAddr(&buf[strlen(buf)], actual_addr);
			break;

		case ST8_EXP:
			if ( (ip->instSize - ip->codeSize) == 1 )
				actual_addr = ST8_relAddr(ip->instSize, codeBuf[codeBufCnt-1]);
			else
				actual_addr = ST8_getVal(&codeBuf[offset], ip->instSize - offset);

			ST8_appendAddr(&buf[l], actual_addr);
			break;

		case (ST8_EXP << 8) | ST8_EXP:
			ST8_appendAddr(&buf[l], ST8_getVal(&codeBuf[offset+2], 2));
			strcat(buf, ", ");
			ST8_appendAddr(&buf[strlen(buf)], ST8_getVal(&codeBuf[offset], 2));
			break;

		case (ST8_EXP0 << 8) | ST8_EXP0:
			sprintf	(&buf[l], "0x%X, 0x%X", ST8_getVal(&codeBuf[offset+1], 0),
											ST8_getVal(&codeBuf[offset  ], 0));
			break;

		default:
			for (int i = 0; i < operCnt; i++)
				ST8_appendOper(buf, ip, i);
	}

//	add(&codeList, buf);
//	printf("%s\n", buf);
	outputReady = true;
}

///////////////////////////////////////////////////////////////////////
void STM8S_dasm :: ST8_appendOper(char *buf, ST8_inst_t *ip, int index)
{
	if ( index )
		strcat(buf, ", ");

	int leng = ip->instSize - ip->codeSize;
	int oper = ST8_getOper(ip, index);
	int offset = strlen(buf);
	int actual_addr;

	switch ( oper )
	{
		case ST8_REGA:	case ST8_REGX:	case ST8_REGY:	case ST8_REGSP:
		case ST8_REGXH:	case ST8_REGXL:	case ST8_REGYH:	case ST8_REGYL:	case ST8_REGCC:
			strcat(buf, ST8_reg(oper));
			break;

		case ST8_EXP:		// mem
		case ST8_EXP0:		// *mem
			actual_addr = ST8_getVal(&codeBuf[ip->codeSize], leng);
			ST8_appendAddr(&buf[offset], actual_addr);
			break;

		case ST8_INDIR:		// [mem]
		case ST8_INDIR0:	// [*mem]
			sprintf(&buf[offset], "[%d]", ST8_getVal(&codeBuf[ip->codeSize], leng));
			break;

		case ST8_INDEX_X:	// (X)
			strcat(buf, "(X)");
			break;

		case ST8_INDEX_Y:	// (Y)
			strcat(buf, "(Y)");
			break;

		case ST8_INDIR_X:	// ([mem], X)
		case ST8_INDIR0_X:	// ([*mem], X)
			sprintf(&buf[offset], "([%d], X)", ST8_getVal(&codeBuf[ip->codeSize], leng));
			break;

		case ST8_INDIR_Y:	// ([mem], Y)
		case ST8_INDIR0_Y:	// ([*mem], Y)
			sprintf(&buf[offset], "([%d], Y)", ST8_getVal(&codeBuf[ip->codeSize], leng));
			break;

		case ST8_OFFSET_X:	// (n, X)
		case ST8_OFFSET0_X:	// (n, X)
			sprintf(&buf[offset], "(%d, X)", ST8_getVal(&codeBuf[ip->codeSize], leng));
			break;

		case ST8_OFFSET_Y:	// (n, Y)
		case ST8_OFFSET0_Y:	// (n, Y)
			sprintf(&buf[offset], "(%d, Y)", ST8_getVal(&codeBuf[ip->codeSize], leng));
			break;

		case ST8_OFFSET0_SP:// (n, SP)
			sprintf(&buf[offset], "(%d, SP)", ST8_getVal(&codeBuf[ip->codeSize], leng));
			break;

		case ST8_IMMD:		// #N
			sprintf(&buf[offset], "#0x%X", ST8_getVal(&codeBuf[ip->codeSize], leng));
			break;
	}
}

///////////////////////////////////////////////////////////////////////
void STM8S_dasm :: ST8_appendAddr(char *buf, int addr)
{
	sprintf(buf, "0x%X", addr);

	Symbol *sp = searchSymbol(symbList, addr, "$sizeof$");
	if ( sp )
		sprintf(&buf[strlen(buf)], " {%s}", sp->name);
}

char *STM8S_dasm :: dumpConst(void)
{
	if ( codeBufCnt > 0 ) {
		sprintf(buf, "%05X:", codeAddr);
		for (int i = 0; i < codeBufCnt; i++)
			sprintf(&buf[strlen(buf)], " %02X", codeBuf[i]);

		codeBufCnt = 0;
		return buf;
	}
	return NULL;
}

unsigned int STM8S_dasm :: ST8_relAddr(int inst_size, unsigned char offset)
{
	unsigned int addr = codeAddr + inst_size + offset;

	if ( offset & 0x80 )
		addr += ~0xff;

	return addr & 0x0fffff;
}

///////////////////////////////////////////////////////////////////////
static char *ST8_reg(int oper)
{
	switch ( oper & 0xff )
	{
		case ST8_REGA:	return (char*)"A";
		case ST8_REGSP:	return (char*)"SP";
		case ST8_REGX:	return (char*)"X";
		case ST8_REGY:	return (char*)"Y";
		case ST8_REGXH:	return (char*)"XH";
		case ST8_REGXL:	return (char*)"XL";
		case ST8_REGYH:	return (char*)"YH";
		case ST8_REGYL:	return (char*)"YL";
		case ST8_REGCC:	return (char*)"CC";
		default:		return (char*)"";
	}
}

///////////////////////////////////////////////////////////////////////
static int  ST8_operCnt(ST8_inst_t *ip)
{
	return (ip->operands & 0xff0000)? 3:
		   (ip->operands & 0xff00)?   2:
		   (ip->operands & 0xff)?     1: 0;
}

///////////////////////////////////////////////////////////////////////
static int  ST8_getOper(ST8_inst_t *ip, int index)
{
	int cnt = ST8_operCnt(ip);
	return (ip->operands >> (8*(cnt - index - 1))) & 0xff;
}

///////////////////////////////////////////////////////////////////////
static unsigned int ST8_getVal(unsigned char *buf, int length)
{
	unsigned int val = 0;
	while ( length-- )
	{
		val <<= 8;
		val  += *buf++;
	}
	return val;
}

