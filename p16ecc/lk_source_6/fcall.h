#ifndef _FCALL_H
#define _FCALL_H

#define FNODE(i,j)		(basicFcallMatrix[i][j])
#define ENODE(i,j)		(extendFcallMatrix[i][j])

typedef enum {IS_CALLER, IS_CALLEE} FUNC_MODE;

typedef struct _Fcall {
	char	*fileName;
	char	*caller;
	char	*callee;
	struct _Fcall *next;
} Fcall;

typedef struct _FuncAttr {
	char	*fileName;	// file name
    char	*funcName;	// function name
	int		 dataSize;	// function local data size
	int		 funcIndex;	// internal index
	bool	 global;
	Segment	*segment;
    struct _FuncAttr *next;
} FuncAttr;

class FcallMgr {

	private:
		FuncAttr *funcList;
		char 	 **basicFcallMatrix;
		char	 **extendFcallMatrix;

	public:
		Fcall	 *fcallList;
		int 	 funcCount;

	public:
		FcallMgr();
		~FcallMgr();

		void clear(void);
		void addFunc(line_t *lp, Segment *seg);
		void addFcall(line_t *lp);
		void setGlobal(line_t *lp, char *func);
		int  funcDataSize(int func_id);

		void printFunc(void);

		FuncAttr *getFunc(int func_id);
		FuncAttr *getFunc(char *file, char *func, FUNC_MODE mode);

		void outputCallPath(FILE *fout=NULL);
		void outputCallPath(FILE *fout, int *queue, int length);
		void createBasicMatrix(void);

	private:
		Fcall *searchFcall(char *fname, char *caller, char *callee);
};

extern FcallMgr fcallMgr;

#endif