#ifndef _FFORMAT_H
#define _FFORMAT_H

#include "item.h"

class F_Format {
	public:
		int  reform(Item *ip, attrib *attr);
		void toInt(int iv, int *vp);
		void toFloat(int iv, int *vp);
		node*mergeCons(int code, node *np0, node *np1, int *err);
};

extern F_Format FFormat;

#endif