#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern "C" {
#include "common.h"
#include "lnk.h"
}
#include "symbol.h"
#include "fcall.h"

void FcallMgr :: createBasicMatrix(void)
{
	basicFcallMatrix = new char *[funcCount];
	for (int i = 0; i < funcCount; i++)
	{
		basicFcallMatrix[i] = new char[funcCount];
		memset(basicFcallMatrix[i], 0, funcCount);
	}

	// build up the basic calling chain ...
	for (Fcall *fp = fcallList; fp; fp = fp->next)
	{
		FuncAttr *fp1 = getFunc(fp->fileName, fp->caller, IS_CALLER);
		FuncAttr *fp2 = getFunc(fp->fileName, fp->callee, IS_CALLEE);

		if ( fp1 && fp2 && fp1->funcIndex != fp2->funcIndex )
			FNODE(fp1->funcIndex, fp2->funcIndex) = 1;
	}
}

FuncAttr *FcallMgr :: getFunc(char *file, char *func, FUNC_MODE mode)
{
	for (FuncAttr *fp = funcList; fp; fp = fp->next)
	{
		if ( strcmp(func, fp->funcName) )
			continue;

		if ( strcmp(file, fp->fileName) == 0 ||	// within same file
			 (mode == IS_CALLEE && fp->global) )
			return fp;
	}

	return NULL;
}

FuncAttr *FcallMgr :: getFunc(int func_id)
{
	for (FuncAttr *fp = funcList; fp; fp = fp->next)
		if ( fp->funcIndex == func_id )	return fp;

	return NULL;
}
