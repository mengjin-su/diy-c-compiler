#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pic16e_inst.h"
#include "pic16e_asm.h"

P16E_ASM :: P16E_ASM(FILE *fileout)
{
	fout = fileout;
	asmOpt = new P16E_asmOPT(fileout);
}

P16E_ASM :: ~P16E_ASM()
{
	delete asmOpt;
}

void P16E_ASM :: output(char *s)
{
	if ( s ) 
		asmOpt->output(s);
}

void P16E_ASM :: label(char *lbl, bool c, char *opr)
{
	asmOpt->label(lbl, c, opr);
}

void P16E_ASM :: code(char *inst)
{
	asmOpt->code(inst);
}

void P16E_ASM :: code(char *inst, char *opr1, char *opr2)
{
	asmOpt->code(inst, opr1, opr2);
}

void P16E_ASM :: code(char *inst, int opr1, char *opr2)
{
	asmOpt->code(inst, opr1, opr2);
}

void P16E_ASM :: code(char *inst, char *opr1, int opr2)
{
	asmOpt->code(inst, opr1, opr2);
}

void P16E_ASM :: code(char *inst, int opr1, int opr2)
{
	asmOpt->code(inst, opr1, opr2);
}
