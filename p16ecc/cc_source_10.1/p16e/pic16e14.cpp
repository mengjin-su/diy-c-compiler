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
#include "../fformat.h"

/*
	32-bit IEEE-754 standard is applied to float number format
*/

void PIC16E :: moveToFP(Item *ip0, Item *ip1)
{
	Item *ip = NULL;
	bool err = false;

	switch ( ip1->type )
	{
		case CON_ITEM:
			ip = ip1->clone();
			FFormat.toFloat(ip1->val.i, &ip->val.i);
			ip->attr->type = FLOAT;
			mov(ip0, ip);
			break;

		case ID_ITEM:
		case TEMP_ITEM:
		case ACC_ITEM:
			err = (ip1->attr->dimVect || ptrWeight(ip1->attr) > 0);
			ip = storeToACC(ip1, 4);
			ip->attr->type = FLOAT;

			if ( ip1->acceSign() )
				call((char*)"_int4ToFloatACC");
			else
				call((char*)"_uint4ToFloatACC");

			mov(ip0, ip);
			break;

		case PID_ITEM:
		case INDIR_ITEM:
		case DIR_ITEM:
			err = (ip1->attr->dimVect || ptrWeight(ip1->attr) > 1);
			ip = storeToACC(ip1, 4);
			ip->attr->type = FLOAT;

			if ( ip1->acceSign() )
				call((char*)"_int4ToFloatACC");
			else
				call((char*)"_uint4ToFloatACC");

			mov(ip0, ip);
			break;

		default:
			err = true;
			break;
	}

	if ( ip ) delete ip;
	if ( err ) errPrint("Can't convert to float!");
}

void PIC16E :: moveFromFP(Item *ip0, Item *ip1)
{
	Item *ip = NULL;
	bool err = false;

	switch ( ip1->type )
	{
		case CON_ITEM:
			ip = ip1->clone();
			FFormat.toInt(ip1->val.i, &ip->val.i);
			ip->attr->type = LONG;
			mov(ip0, ip);
			break;

		case ID_ITEM:
		case TEMP_ITEM:
		case ACC_ITEM:
		case PID_ITEM:
		case INDIR_ITEM:
		case DIR_ITEM:
			ip = storeToACC(ip1, 4);
			ip->attr->type = LONG;
			call((char*)"_floatToIntACC");
			mov(ip0, ip);
			break;

		default:
			err = true;
	}

	if ( ip ) delete ip;
	if ( err ) errPrint("Can't convert to int!");
}

void PIC16E :: pushToStackAsFloat(Item *ip)
{
	if ( ip->type == CON_ITEM )
	{
		if ( ip->attr->type != FLOAT )
		{
			Item *_ip = ip->clone();
			FFormat.toFloat(ip->val.i, &_ip->val.i);
			_ip->attr->type = FLOAT;
			pushStack(_ip, 4);
			delete _ip;
			return;
		}
	}
	pushStack(ip, 4);
	if ( !ip->acceFloat() )
	{
		if ( ip->acceSign() )
			call((char*)"_int4ToFloatStack");
		else
			call((char*)"_uint4ToFloatStack");
	}
}

void PIC16E :: storeToAccAsFloat(Item *ip)
{
	if ( ip->type == CON_ITEM )
	{
		if ( ip->attr->type != FLOAT )
		{
			Item *_ip = ip->clone();
			FFormat.toFloat(ip->val.i, &_ip->val.i);
			_ip->attr->type = FLOAT;
			storeToACC(_ip, 4);
			delete _ip;
			return;
		}
	}

	delete storeToACC(ip, 4);
	if ( !ip->acceFloat() )
	{
		if ( ip->acceSign() )
			call((char*)"_int4ToFloatACC");
		else
			call((char*)"_uint4ToFloatACC");
	}
}

void PIC16E :: addFP(Item *ip0, Item *ip1)
{
	pushToStackAsFloat(ip0);
	storeToAccAsFloat(ip1);
	call((char*)"_floatAdd");
}

void PIC16E :: subFP(Item *ip0, Item *ip1)
{
	pushToStackAsFloat(ip0);
	storeToAccAsFloat(ip1);
	call((char*)"_floatSub");
}

void PIC16E :: mulFP(Item *ip0, Item *ip1)
{
	pushToStackAsFloat(ip0);
	storeToAccAsFloat(ip1);
	call((char*)"_floatMul");
}

void PIC16E :: divFP(Item *ip0, Item *ip1)
{
	pushToStackAsFloat(ip0);
	storeToAccAsFloat(ip1);
	call((char*)"_floatDiv");
}

void PIC16E :: cmpFP(int code, Item *ip0, Item *ip1, Item *ip2)
{
	pushToStackAsFloat((code == P_JLT || code ==  P_JLE)? ip0: ip1);
	storeToAccAsFloat ((code == P_JLT || code ==  P_JLE)? ip1: ip0);

	switch ( code )
	{
		case P_JLT:
		case P_JGT: call((char*)"_floatCmpJLT");	break;
		case P_JLE:
		case P_JGE: call((char*)"_floatCmpJLE");	break;
	}

	regPCLATH->load(ip2->val.s);
	ASM_CODE(_BTFSC, STATUS, 0);	// C = 0, no jump
	ASM_CODE(_GOTO, ip2->val.s);
}

void PIC16E :: jeqjneFP(int code, Item *ip0, Item *ip1, Item *ip2)
{
	Item *ip = ip0;
	if ( !ip0->acceFloat() )
	{
		storeToAccAsFloat(ip0);
		ip = ip1;
	}
	else
		storeToAccAsFloat(ip1);

	char *indf0 = setFSR0(ip);
	for (int i = 0; i < 4; i++)
	{
		ASM_CODE(_MOVF, acceItem(ip, i, indf0), _W_);
		if ( i == 0 )
			ASM_CODE(_XORWF, 0x70, _F_);
		else
		{
			ASM_CODE(_XORWF, 0x70+i, _W_);
			ASM_CODE(_IORWF, 0x70, _F_);
		}
	}

	regPCLATH->load(ip2->val.s);
	if ( code == P_JEQ )
		ASM_CODE(_BTFSC, STATUS, 2);
	else
		ASM_CODE(_BTFSS, STATUS, 2);

	ASM_CODE(_GOTO, ip2->val.s);
}