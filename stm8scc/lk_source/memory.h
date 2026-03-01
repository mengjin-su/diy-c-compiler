#ifndef _MEM_H
#define _MEM_H

typedef enum {DATA_MEMORY, CODE_MEMORY} MEMORY_TYPE;

#define FIXED_ADDR_FLAG		0x01
#define BANK0_ADDR_FLAG		0x02
#define _64KB_ADDR_FLAG		0x04	// const segment boundry

class Memory {
	private:
		MEMORY_TYPE type;	// memory type
		char 		*mem;	// memory mark
	public:			
		int  		size;	// memory size

	public:
		Memory(MEMORY_TYPE type);
		~Memory();

		void resize(int size);
		void init(void);
		void reset(void);
		void blank(int start, int length);
		int  getSpace(int start, int size, int flags);
		int  memUsed(int *total);
		void printSpace(FILE *);
		void display(int);
		char content(int addr) { return mem[addr]; }

	private:
		void fillSpace(int start, int size, char val);
};


#endif