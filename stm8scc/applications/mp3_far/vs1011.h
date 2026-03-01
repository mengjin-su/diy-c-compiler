#ifndef _VS1011_H
#define _VS1011_H

#define VS1011_CLK_FREQ		12000000	// need to be doubled up

enum {
	VS1011_REG_MODE,	// 0
	VS1011_REG_STATUS,	// 1
	VS1011_REG_BASS,	// 2
	VS1011_REG_CLOCKF,	// 3
	VS1011_REG_DECODE,
	VS1011_REG_AUDATA,
	VS1011_REG_WRAM,
	VS1011_REG_WRAMADDR,
	VS1011_REG_HDAT0,
	VS1011_REG_HDAT1,
	VS1011_REG_AIADDR,
	VS1011_REG_VOL
};

void VS1011_init (void);
void VS1011_reset (void);

void VS1011_wrReg (unsigned char reg, unsigned int data);
unsigned int VS1011_rdReg (unsigned char reg);

void VS1011_setVol (unsigned char val);
void VS1011_incVol (void);
void VS1011_decVol (void);
void VS1011_setBass (unsigned char val);
void VS1011_setTreb (unsigned char val);
void VS1011_play (unsigned char *ptr);

void VS1011_setBit (unsigned char reg, unsigned int word);

#endif
