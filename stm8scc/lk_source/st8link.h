#ifndef P16LINK_H
#define P16LINK_H

enum {
	RAM_LOC_ABS,
	RAM_LOC_BANK0,
	RAM_LOC_FLOAT,
	ROM_LOC_ABS,
	ROM_LOC_FLOAT,
};

class ST8link {

	private:
		Memory  	*codeMem;
		Memory  	*dataMem;
		Symbol  	*symbList;
		FILE 		*fout;
	public:
		int errorCount;

	public:
		ST8link(Memory *ram, Memory *rom);
		~ST8link();

		void addSeg(Segment *seg, bool add2lib);
		void run(Memory *ram, Memory *rom);

		int  scanSymbolInclusion(void);
		int  scanInclusion(void);

		// st8link3.cpp
		int  assignSegmentsAddress(void);
		void logDataSegLabels(Symbol **slist, Segment *seg);
		int  confirmSegmentMem(void);

		// st8link4.cpp
		void outputHex(char *filename);

		// st8link5.cpp
		void outputMap(char *filename, Memory *);

		// st8link6.cpp
		void searchUnusedSegment(Symbol *slist);
		bool searchUnusedSegment(Segment *segp, item_t *ip, Symbol *slist);
		void removeUnusedSegment(Segment **segp);

	private:
		void addSeg(Segment **list, Segment *seg);
		void insertSeg(Segment **list, Segment *last, Segment *seg);
		void appendSeg(Segment **list, Segment *seg);

		void logSegmentSymbols(Segment *seglist, Symbol **symlist);
		bool scanSegLabels(Segment *seglist, Symbol *symlist);
		void scanFuncLocalData(Symbol **symlist);

		Segment *extractLibSegment(Segment **slist, char *lbl, Segment *seg);
		void assignSegmentMem(Segment *sp, int flag);
		void assignSegmentMem(Memory *mem, Segment *seg, int addr, int flag);
};

#endif