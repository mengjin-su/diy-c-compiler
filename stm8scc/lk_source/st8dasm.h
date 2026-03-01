#ifndef ST8_DISASM_H
#define ST8_DISASM_H

#include <stdio.h>

enum {
	ST8_REGA = 1,
	ST8_REGX,
	ST8_REGY,
	ST8_REGSP,
	ST8_REGXH,
	ST8_REGXL,
	ST8_REGYH,
	ST8_REGYL,
	ST8_REGCC,
	ST8_EXP,		// mem
	ST8_EXP0,		// *mem
	ST8_INDIR,		// [mem]
	ST8_INDIR0,		// [*mem]
	ST8_INDEX_X,	// (X)
	ST8_INDEX_Y,	// (Y)
	ST8_INDIR_X,	// ([mem], X)
	ST8_INDIR_Y,	// ([mem], Y)
	ST8_INDIR0_X,	// ([*mem], X)
	ST8_INDIR0_Y,	// ([*mem], Y)
	ST8_OFFSET_X,	// (n, X)
	ST8_OFFSET_Y,	// (n, Y)
	ST8_OFFSET0_X,	// (n, X)
	ST8_OFFSET0_Y,	// (n, Y)
	ST8_OFFSET0_SP,	// (n, SP)
	ST8_IMMD,
};

typedef struct {
	char 		*mnemonic;
	unsigned int codeSize;		// bytes of whole instruction
	unsigned int code;			// instruction code
	unsigned int instSize;		// whole instruction size (bytes)
	unsigned int operands;
} ST8_inst_t;

// --- STM8S instruction set ---
#define _ADD		(char*)"add"
#define _ADDW		(char*)"addw"
#define _ADC		(char*)"adc"
#define _AND		(char*)"and"
#define _BCCM		(char*)"bccm"
#define _BCP		(char*)"bcp"
#define _BCPL		(char*)"bcpl"
#define _BRES		(char*)"bres"
#define _BSET		(char*)"bset"
#define _BTJF		(char*)"btjf"
#define _BTJT		(char*)"btjt"
#define _CALL		(char*)"call"
#define _CALLF		(char*)"callf"
#define _CALLR		(char*)"callr"
#define _CCF		(char*)"ccf"
#define _CLR		(char*)"clr"
#define _CLRW		(char*)"clrw"	// clears index reg.
#define _CP			(char*)"cp"		// compare
#define _CPW		(char*)"cpw"
#define _CPL		(char*)"cpl"
#define _CPLW		(char*)"cplw"
#define _DEC		(char*)"dec"
#define _DECW		(char*)"decw"
#define _DIV		(char*)"div"
#define _DIVW		(char*)"divw"
#define _EXG		(char*)"exg"
#define _EXGW		(char*)"exgw"
#define _HALT		(char*)"halt"
#define _INC		(char*)"inc"
#define _INCW		(char*)"incw"
#define _IRET		(char*)"iret"
#define _JP			(char*)"jp"
#define _JPF		(char*)"jpf"
#define _JRA		(char*)"jra"
#define _JRC		(char*)"jrc"
#define _JREQ		(char*)"jreq"
#define _JRF		(char*)"jrf"
#define _JRH		(char*)"jrh"
#define _JRIH		(char*)"jrih"
#define _JRIL		(char*)"jril"
#define _JRM		(char*)"jrm"
#define _JRMI		(char*)"jrmi"
#define _JRNC		(char*)"jrnc"
#define _JRNE		(char*)"jrne"
#define _JRNH		(char*)"jrnh"
#define _JRNM		(char*)"jrnm"
#define _JRNV		(char*)"jrnv"
#define _JRPL		(char*)"jrpl"
#define _JRSGE		(char*)"jrsge"
#define _JRSGT		(char*)"jrsgt"
#define _JRSLE		(char*)"jrsle"
#define _JRSLT		(char*)"jrslt"
#define _JRT		(char*)"jrt"
#define _JRUGT		(char*)"jrugt"
#define _JRUGE		(char*)"jruge"
#define _JRULE		(char*)"jrule"
#define _JRULT		(char*)"jrult"
#define _JRV		(char*)"jrv"
#define _LD			(char*)"ld"
#define _LDF		(char*)"ldf"
#define _LDW		(char*)"ldw"
#define _MOV		(char*)"mov"
#define _MUL		(char*)"mul"
#define _NEG		(char*)"neg"
#define _NOP		(char*)"nop"
#define _NEGW		(char*)"negw"
#define _OR			(char*)"or"
#define _POP		(char*)"pop"
#define _POPW		(char*)"popw"
#define _PUSH		(char*)"push"
#define _PUSHW		(char*)"pushw"
#define _RCF		(char*)"rcf"
#define _RET		(char*)"ret"
#define _RETF		(char*)"retf"
#define _RIM		(char*)"rim"
#define _RLC		(char*)"rlc"
#define _RLCW		(char*)"rlcw"
#define _RLWA		(char*)"rlwa"
#define _RRC		(char*)"rrc"
#define _RRCW		(char*)"rrcw"
#define _RRWA		(char*)"rrwa"
#define _RVF		(char*)"rvf"
#define _SBC		(char*)"sbc"
#define _SCF		(char*)"scf"
#define _SIM		(char*)"sim"
#define _SLA		(char*)"sla"
#define _SLAW		(char*)"slaw"
#define _SLL		(char*)"sll"
#define _SLLW		(char*)"sllw"
#define _SRA		(char*)"sra"
#define _SRAW		(char*)"sraw"
#define _SRL		(char*)"srl"
#define _SRLW		(char*)"srlw"
#define _SUB		(char*)"sub"
#define _SUBW		(char*)"subw"
#define _SWAP		(char*)"swap"
#define _SWAPW		(char*)"swapw"
#define _TNZ		(char*)"tnz"
#define _TNZW		(char*)"tnzw"
#define _TRAP		(char*)"trap"
#define _WFE		(char*)"wfe"
#define _WFI		(char*)"wfi"
#define _XOR		(char*)"xor"
#define _VECT		(char*)".vect"

///////////////////////////////////////////////////////////////////////
class STM8S_dasm {
	private:
		unsigned char codeBuf[20];	// longest instruction = 5 bytes
		unsigned int  codeAddr;
		unsigned char codeBufCnt;	// buffer contents length <= 5
		Symbol  	 *symbList;
		int			  segmentType;

		char 		 buf[256];		// output buffer
		bool		 outputReady;

	public:
		STM8S_dasm(Symbol *);
		~STM8S_dasm();
		char *dasmCode(int type, unsigned int addr, unsigned int value, int size);
		char *dumpConst(void);

	private:
		void dasmCode(void);
		void dasmCode(ST8_inst_t *);
		unsigned int ST8_relAddr(int inst_size, unsigned char offset);
		void ST8_appendAddr(char *buf, int addr);
		void ST8_appendOper(char *buf, ST8_inst_t *ip, int index);
};

//extern SYMBOL_LIST output_lst;

void ST8asm_const (unsigned int addr, unsigned int value, int size);
void ST8asm       (unsigned int addr, unsigned int value, int size);
void ST8asm (char *symbl, char label_type);
void ST8asm_dump (void);

#endif
