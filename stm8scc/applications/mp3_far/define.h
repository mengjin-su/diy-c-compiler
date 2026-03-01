#ifndef _DEFINE_H
#define _DEFINE_H

// --- basic data types ---
typedef char 		  	i08;
typedef int  		  	i16;
typedef long 		  	i32;
typedef unsigned char 	u08;
typedef unsigned int  	u16;
typedef unsigned long 	u32;

#define _NOP_			asm("nop")
#define SEI()			asm("rim")

////////////////////////////////////////////////////////////////////
#define sbi(byte,b)			byte |=  (1 << b)
#define cbi(byte,b)			byte &= ~(1 << b)

////////////////////////////////////////////////////////////////////
#define LED_CFG_DDR         sbi(PD_DDR,0)
#define LED_ON              cbi(PD_ODR,0)
#define LED_OFF             sbi(PD_ODR,0)

#define SD_CS_CFG_DDR		sbi(PA_DDR,4),sbi(PA_CR1,4)
#define SD_DISABLE			sbi(PA_ODR,4)
#define SD_ENABLE			cbi(PA_ODR,4)

// unchecked definitions
#define SD_SCK_UNHOLD		cbi(PC_DDR,6)
#define SD_SCK_HOLD			cbi(PC_ODR,6),sbi(PC_DDR,6)

// VS1011 control/communicatin pins ...
#define VS1011_INST_CS_PIN	1
#define VS1011_DATA_CS_PIN	3

#define VS1011_ICS_CFG_DDR	sbi(PB_DDR,VS1011_INST_CS_PIN),sbi(PB_CR1,VS1011_INST_CS_PIN)
#define VS1011_ENABLE_INST	cbi(PB_ODR,VS1011_INST_CS_PIN)
#define VS1011_DISABLE_INST	sbi(PB_ODR,VS1011_INST_CS_PIN)

#define VS1011_DCS_CFG_DDR	sbi(PB_DDR,VS1011_DATA_CS_PIN),sbi(PB_CR1,VS1011_DATA_CS_PIN)
#define VS1011_ENABLE_DATA	cbi(PB_ODR,VS1011_DATA_CS_PIN)
#define VS1011_DISABLE_DATA	sbi(PB_ODR,VS1011_DATA_CS_PIN)

#define VS1011_BUSY         !(PB_IDR & (1<<5))

#define VS1011_RST_CFG		sbi(PB_DDR,7),sbi(PB_CR1,7)
#define VS1011_RST_LO		cbi(PB_ODR,7)
#define VS1011_RST_HI		sbi(PB_ODR,7)

///////////////////////////////////////////////////////////////////
void delay (void);
void MEMCPY (unsigned char *s1, unsigned char *s2, unsigned char len);
char MEMCMP_ROM (char *s1, char *s2, char len);

#endif

