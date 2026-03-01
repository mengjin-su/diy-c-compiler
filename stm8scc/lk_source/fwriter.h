#ifndef FWRITER_H
#define FWRITER_H
extern "C" {
#include "common.h"
#include "lnk.h"
}
#include "symbol.h"
#include "segment.h"

#define HEX_BUF_SIZE	128

class HexWriter {

	private:
		int startAddr;
		int currentAddr;
		int length;
		unsigned char buf[HEX_BUF_SIZE];
		FILE *fout;

	public:
		HexWriter(char *filename);
		~HexWriter();
//		void close(void);
		void outputByte(int addr, int word);
		void outputLine(void);
		void outputBank(int bank);
};

class MapWriter {
	public:
		FILE *fout;

	public:
		MapWriter(char *filename);
		void close(void);
		void outputSeg(Symbol *slist, int used, int total);
		void outputSeg(Segment *seg, Symbol *slist, int used, int total);
		void outputSkip0(void);
};

#endif