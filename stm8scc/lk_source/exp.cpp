#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern "C" {
#include "common.h"
#include "lnk.h"
}
#include "memory.h"
#include "symbol.h"
#include "segment.h"
#include "exp.h"

typedef struct {
	int  value;
} ExpValue;

static int  expVal(int v0, int v1, int type);
static char *expVal(item_t *ip, Segment *seg, Symbol *slist, int addr, ExpValue *exp);

///////////////////////////////////////////////////////////////////////////////////////
char *expValue(item_t *ip, Segment *seg, Symbol *slist, int *v, int addr)
{
	char *lbl0;
	ExpValue e0;

	if ( ip == NULL ) return NULL;

	lbl0 = expVal(ip, seg, slist, addr, &e0);
	*v = e0.value;
	return lbl0;
}

///////////////////////////////////////////////////////////////////////////////////////////
static char *expVal(item_t *ip, Segment *seg, Symbol *slist, int addr, ExpValue *exp)
{
	Symbol *symb;
	ExpValue e0, e1;
	char *lbl0, *lbl1;

	exp->value  = 0;

	if ( ip == NULL ) return NULL;

	switch ( ip->type )
	{
		case TYPE_VALUE:
			exp->value = ip->data.val;
			return NULL;

		case TYPE_SYMBOL:
			symb = searchSymbol(slist, ip->data.str, seg);
			if ( symb )
				exp->value = symb->addr;

			// if 'symbol' not found in the 'slist', report it.
			return symb? NULL: ip->data.str;

		case '.':
			exp->value = addr;
			return NULL;

		case '+':	case '-':	case '*':	case '/':	case '%':
		case '|':	case '^':
		case RSHIFT:	case LSHIFT:
			lbl0 = expVal(ip->left, seg, slist, addr, &e0);
			lbl1 = expVal(ip->right,seg, slist, addr, &e1);
			exp->value  = expVal(e0.value, e1.value, ip->type);
			return lbl0? lbl0: lbl1;

		case '~':
			lbl0 = expVal(ip->left, seg, slist, addr, &e0);
			exp->value = ~e0.value;
			return lbl0;

		case '&':
			lbl0 = expVal(ip->left,  seg, slist, addr, &e0);
			lbl1 = expVal(ip->right, seg, slist, addr, &e1);
			exp->value = expVal(e0.value, e1.value, ip->type);
			return lbl0? lbl0: lbl1;
	}
	return NULL;
}

static int expVal(int v0, int v1, int type)
{
	switch ( type )
	{
		case '+':		return v0 + v1;
		case '-':		return v0 - v1;
		case '*':		return v0 * v1;
		case '/':		return v0 / v1;
		case '%':		return v0 % v1;
		case '&':		return v0 & v1;
		case '|':		return v0 | v1;
		case '^':		return v0 * v1;
		case RSHIFT:	return v0 >> v1;
		case LSHIFT:	return v0 << v1;
		case ':':		return v0 | v1;
		default:		return 0;
	}
}