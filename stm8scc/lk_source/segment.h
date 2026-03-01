#ifndef SEGMENT_H
#define SEGMENT_H

enum {
	CODE_SEGMENT=1,
	DATA_SEGMENT,
	CONST_SEGMENT,
	DUMMY_SEGMENT,
};

class Segment {
	public:
		Segment	*next;
		line_t	*lines;			// line pointer
		char    *fileName;		// file name
		int		isBEG: 1;		// begin of segment link
		int	 	isEND: 1;		// end of segment link
	//	int	 	isOVR: 1;		// segment overwrite
		int	 	isREL: 1;		// relocatable segment
		int	 	isABS: 1;		// absolute address seg.
		int	 	isUsed:  1;		// code will be used
		int		isLIB: 1;		// lib code
		int 	addr;			// ABS address
		int 	dataSize;		// function data size
		int 	startAddr;
		int 	maddr;			// memory mapped address

	public:
		Segment(char *file_name);
		~Segment();
		void addLine(line_t *lp);
		int  lineCount(void);
		int  type(void);		// segment type
		int  size(void);		// segment size
		char *name(void);		// segment name
		void print(void);
};

extern Segment 	*libSegGroup;
extern Segment 	*codeSegGroup;
extern Segment 	*dataSegGroup;
extern Segment 	*miscSegGroup;

void deleteSegments(Segment *list);
void printfSegments(Segment *list);

#endif