#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <time.h>
#include "../common.h"
extern "C" {
#include "../cc.h"
}
#include "../sizer.h"
#include "../pcoder.h"
#include "../display.h"
#include "pic16e.h"
#include "pic16e_asm.h"
#include "pic16e_reg.h"
#include "pic16e_inst.h"

static bool selectBit(int n, int *b);

void PIC16E :: addsub(int code, Item *ip0, Item *ip1)
{
	if ( ip1->type == CON_ITEM )
	{
		int n = (code == ADD_ASSIGN)? ip1->val.i: -ip1->val.i;
		incValue(ip0, n);
		return;
	}
	
	int size0 = ip0->acceSize();
	int size1 = ip1->acceSize();
	if ( useFSR(ip0, ip1) )
	{
		Item *acc = storeToACC(ip1, size0);
		addsub(code, ip0, acc);
		delete acc;
		return;
	}

	char *inst = (code == ADD_ASSIGN)? _ADDWF: _SUBWF;
	char *indf0 = setFSR0(ip0);
	char *indf1 = setFSR0(ip1);
	for(int i = 0; i < size0; i++)
	{
		if ( i < size1 )
			fetch(ip1, i, indf1);
		else if ( !ip1->acceSign() )
			regWREG->load(0);
		else if ( i == size1 )
			EXTEND_WREG;

		char *ds = acceItem(ip0, i, indf0);
		ASM_CODE(inst, regWREG->reset(ds), _F_);

		inst = (code == ADD_ASSIGN)? _ADDWFC: _SUBWFB;
	}
}

void PIC16E :: andorxor(int code, Item *ip0, int n)
{
	if ( isWREG(ip0) )
	{
		ASM_CODE((code == XOR_ASSIGN)? _XORLW:
				 (code == AND_ASSIGN)? _ANDLW:
									   _IORLW, n & 0xff);
		regWREG->reset();
		return;
	}
	
	int  size0 = ip0->acceSize();
	char *indf = setFSR0(ip0);
	for(int i =	0; i < size0; i++, n >>= 8)
	{
		char *inst = NULL;
		char *s = NULL;
		char *f = NULL;
		int  b;
		if ( selectBit(n, &b) && code == OR_ASSIGN )
		{
			inst = _BSF;
			s = acceItem(ip0, i, indf);
			f = STRBUF(); sprintf(f, "%d", b);
		}
		else if ( selectBit(~n, &b) && code == AND_ASSIGN )
		{
			inst = _BCF;
			s = acceItem(ip0, i, indf);
			f = STRBUF(); sprintf(f, "%d", b);
		}
		else
		{
			switch ( n & 0xff )
			{
				case 0x00:
					if ( code == AND_ASSIGN )
					{
						inst = _CLRF;
						s = acceItem(ip0, i, indf);
					}
					break;

				case 0xff:
					if ( code == AND_ASSIGN )
						break;
					if ( code == XOR_ASSIGN )
					{
						inst = _COMF;
						s = acceItem(ip0, i, indf);
						f = _F_;
						break;
					}
					// fall through...
				default:
					regWREG->load(n);
					inst = (code == AND_ASSIGN)? _ANDWF:
						   (code == XOR_ASSIGN)? _XORWF: _IORWF;
					s = acceItem(ip0, i, indf);
					f = _F_;
			}
		}
		if ( inst && s )
		{
			ASM_CODE(inst, s, f);
			regWREG->reset(s);
		}
	}
}

void PIC16E :: andorxor(int code, Item *ip0, Item *ip1)
{
	if ( ip1->type == CON_ITEM )
	{
		andorxor(code, ip0, ip1->val.i);
		return;
	}

	int size0 = ip0->acceSize();
	int size1 = ip1->acceSize();
	if ( useFSR(ip0, ip1) )
	{
		Item *acc = storeToACC(ip1, size0);
		andorxor(code, ip0, acc);
		delete acc;
		return;
	}
	
	int sign1 = ip1->acceSign();
	char *indf0 = setFSR0(ip0);				 
	char *indf1 = setFSR0(ip1);
	char *inst = (code == AND_ASSIGN)? _ANDWF:
				 (code == XOR_ASSIGN)? _XORWF: _IORWF;			
	for(int i = 0; i < size0; i++)
	{
		char *ds;

		if ( i >= size1 && !sign1 && code != AND_ASSIGN )
			return;

		if ( i < size1 )
			fetch(ip1, i, indf1);
		else if ( !sign1 )
		{
			ds = acceItem(ip0, i, indf0);
			asm16e->code(_CLRF, regWREG->reset(ds));
			continue;
		}
		else if ( i == size1 )
		{
			EXTEND_WREG;
		}

		ds = acceItem(ip0, i, indf0);
		asm16e->code(inst, regWREG->reset(ds), _F_);
	}
}

/////////////////////////////////////////////////////////////
static bool selectBit(int n, int *b)
{
	for (*b = 0; *b < 8; (*b)++)
		if ( (n & 0xff) == (1<<(*b)) )
			return true;

	return false;
}
