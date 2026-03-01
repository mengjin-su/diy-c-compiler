#include <string.h>
#include <ctype.h>
#include <time.h>
extern "C" {
#include "common.h"
#include "cc.h"
}
#include "item.h"
#include "fformat.h"

F_Format FFormat;

int F_Format :: reform(Item *ip, attrib *attr)
{
	bool in_float = (attr && !attr->ptrVect && attr->type == FLOAT);	// require float format?

	if ( ip->type != CON_ITEM )
		return in_float? -1: 0;

	if ( in_float )
	{
		if ( ip->attr->type != FLOAT )
		{
			toFloat(ip->val.i, &ip->val.i);
			ip->attr->type = FLOAT;
			ip->attr->isUnsigned = 0;
		}
	}
	else if ( ip->attr->type == FLOAT )
	{
		toInt(ip->val.i, &ip->val.i);
		ip->attr->type = LONG;
		ip->attr->isUnsigned = 0;
	}
	return 0;
}

void F_Format :: toInt(int iv, int *vp)
{
	float *fp = (float*)&iv;
	*vp = (int)(*fp);
}

void F_Format :: toFloat(int iv, int *vp)
{
	float fv = (float)iv;					// convert float to int format
	int  *np = (int*)&fv;
	*vp = *np;
}

node *F_Format :: mergeCons(int code, node *np0, node *np1, int *err)
{
	if ( np0->con.attr->type == FLOAT || np1->con.attr->type == FLOAT )
	{
		float fn0 = (np0->con.attr->type == FLOAT)? np0->con.fvalue: (float)np0->con.value;
		float fn1 = (np1->con.attr->type == FLOAT)? np1->con.fvalue: (float)np1->con.value;

		switch ( code )
		{
			case '+': 		return fconNode(fn0 + fn1);
			case '-':  		return fconNode(fn0 - fn1);
			case '*':  		return fconNode(fn0 * fn1);
			case '/': 		return fconNode(fn0 / fn1);
			case EQ_OP: 	return conNode((fn0 == fn1)? 1: 0, CHAR);
			case NE_OP: 	return conNode((fn0 != fn1)? 1: 0, CHAR);
			case '>': 		return conNode((fn0  > fn1)? 1: 0, CHAR);
			case '<':		return conNode((fn0  < fn1)? 1: 0, CHAR);
			case LE_OP:		return conNode((fn0 <= fn1)? 1: 0, CHAR);
			case GE_OP:		return conNode((fn0 >= fn1)? 1: 0, CHAR);
			case AND_OP:	return conNode((fn0 && fn1)? 1: 0, CHAR);
			case OR_OP:		return conNode((fn0 || fn1)? 1: 0, CHAR);
// the following operations are fake, with error flag set.
			case '%': 		*err = 1; return conNode((int)fn0 % (int)fn1, INT);
			case '&':  		*err = 1; return conNode((int)fn0 & (int)fn1, INT);
			case '|': 		*err = 1; return conNode((int)fn0 | (int)fn1, INT);
			case '^': 		*err = 1; return conNode((int)fn0 ^ (int)fn1, INT);
			case LEFT_OP: 	*err = 1; return conNode((int)fn0 << (int)fn1, INT);
			case RIGHT_OP:	*err = 1; return conNode((int)fn0 >> (int)fn1, INT);
		}
	}
	return NULL;
}
