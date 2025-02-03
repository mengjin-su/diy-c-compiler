#ifndef _P12F1840_H
#define _P12F1840_H

#define __DEVICE    "pic12f1840"
#include <pic16e.h>

#define __FLASH_SIZE        4096
#define __SRAM_SIZE         256
#define __END_STACK_ADDR    (__SRAM_SIZE-16)

#define STATUS_SHAD			REG8(0x64 | _BK31)
#define WREG_SHAD			REG8(0x65 | _BK31)
#define BSR_SHAD			REG8(0x66 | _BK31)
#define PCLATH_SHAD			REG8(0x67 | _BK31)
#define FSR0L_SHAD			REG8(0x68 | _BK31)
#define FSR0H_SHAD			REG8(0x69 | _BK31)
#define FSR1L_SHAD			REG8(0x6a | _BK31)
#define FSR1H_SHAD			REG8(0x6b | _BK31)
#define STKPTR				REG8(0x6d | _BK31)
#define TOSL				REG8(0x6e | _BK31)
#define TOSH				REG8(0x6f | _BK31)

#define PORTA       		REG8(0x0c | _BK0)
#define PIR1				REG8(0x11 | _BK0)
#define PIR2				REG8(0x12 | _BK0)
#define TMR0				REG8(0x15 | _BK0)
#define TMR1L				REG8(0x16 | _BK0)
#define TMR1H				REG8(0x17 | _BK0)
#define TMR1				REG16(0x16 | _BK0)
#define T1CON				REG8(0x18 | _BK0)
#define T1GCON				REG8(0x19 | _BK0)
#define TMR2				REG8(0x1a | _BK0)
#define PR2					REG8(0x1b | _BK0)
#define T2CON				REG8(0x1c | _BK0)

#define TRISA       		REG8(0x0c | _BK1)
#define PIE1				REG8(0x11 | _BK1)
#define PIE2				REG8(0x12 | _BK1)
#define OPTION_REG			REG8(0x15 | _BK1)
#define PCON				REG8(0x16 | _BK1)
#define WDTCON				REG8(0x17 | _BK1)
#define OSCTUNE				REG8(0x18 | _BK1)
#define OSCCON				REG8(0x19 | _BK1)
#define OSCSTAT				REG8(0x1a | _BK1)
#define ADRESL				REG8(0x1b | _BK1)
#define ADRESH				REG8(0x1c | _BK1)
#define ADRES				REG16(0x1b | _BK1)
#define ADCON0				REG8(0x1d | _BK1)
#define ADCON1				REG8(0x1e | _BK1)

#define LATA	       		REG8(0x0c | _BK2)
#define CM1CON0				REG8(0x11 | _BK2)
#define CM1CON1				REG8(0x12 | _BK2)
#define CMOUT				REG8(0x15 | _BK2)
#define BORCON				REG8(0x16 | _BK2)
#define FVRCON				REG8(0x17 | _BK2)
#define DACCON0				REG8(0x18 | _BK2)
#define DACCON1				REG8(0x19 | _BK2)
#define SRCON0				REG8(0x1a | _BK2)
#define SRCON1				REG8(0x1b | _BK2)
#define APFCON				REG8(0x1d | _BK2)

#define ANSELA				REG8(0x0c | _BK3)
#define EEADRL				REG8(0x11 | _BK3)
#define EEADRH				REG8(0x12 | _BK3)
#define EEDATL				REG8(0x13 | _BK3)
#define EEDATH				REG8(0x14 | _BK3)
#define EECON1				REG8(0x15 | _BK3)
#define EECON2				REG8(0x16 | _BK3)
#define VREGCON				REG8(0x17 | _BK3)
#define RCREG				REG8(0x19 | _BK3)
#define TXREG				REG8(0x1a | _BK3)
#define SPBRGL				REG8(0x1b | _BK3)
#define SPBRGH				REG8(0x1c | _BK3)
#define RCSTA				REG8(0x1d | _BK3)
#define TXSTA				REG8(0x1e | _BK3)
#define BAUDCON				REG8(0x1f | _BK3)

#define PORTAbits		(REG8_t*)(&PORTA)
#define TRISAbits		(REG8_t*)(&TRISA)
#define LATAbits		(REG8_t*)(&LATA)

typedef struct {
	unsigned char RX9D		: 1;
	unsigned char OERR		: 1;
	unsigned char FERR		: 1;
	unsigned char ADDERR	: 1;
	unsigned char CREN		: 1;
	unsigned char SREN		: 1;
	unsigned char RX9		: 1;
	unsigned char SPEN		: 1;
} RCSTA_t;
#define RCSTAbits		(RCSTA_t*)(&RCSTA)

typedef struct {
	unsigned char TX9D		: 1;
	unsigned char TRMT		: 1;
	unsigned char BRGH		: 1;
	unsigned char SENDB		: 1;
	unsigned char SYNC		: 1;
	unsigned char TXEN		: 1;
	unsigned char TX9		: 1;
	unsigned char CSRC		: 1;
} TXSTA_t;
#define TXSTAbits		(TXSTA_t*)(&TXSTA)

#define WPUA				REG8(0x0c | _BK4)
#define SSP1BUF				REG8(0x11 | _BK4)
#define SSP1ADD				REG8(0x12 | _BK4)
#define SSP1MSK				REG8(0x13 | _BK4)
#define SSP1STAT			REG8(0x14 | _BK4)
#define SSP1CON1			REG8(0x15 | _BK4)
#define SSP1CON2			REG8(0x16 | _BK4)
#define SSP1CON3			REG8(0x17 | _BK4)

#define CCPR1L				REG8(0x11 | _BK5)
#define CCPR1H				REG8(0x12 | _BK5)
#define CCP1CON				REG8(0x13 | _BK5)
#define PWM1CON				REG8(0x14 | _BK5)
#define CCP1AS				REG8(0x15 | _BK5)
#define PSTR1CON			REG8(0x16 | _BK5)

#define IOCAP				REG8(0x11 | _BK7)
#define IOCAN				REG8(0x12 | _BK7)
#define IOCAF				REG8(0x13 | _BK7)
#define CLKRCON				REG8(0x1a | _BK7)
#define MDCON				REG8(0x1c | _BK7)
#define MDSRC				REG8(0x1d | _BK7)
#define MDCARL				REG8(0x1e | _BK7)
#define MDCARH				REG8(0x1f | _BK7)

typedef struct {
	unsigned char TMR1IF	: 1;
	unsigned char TMR2IF	: 1;
	unsigned char CCP1IF	: 1;
	unsigned char SSP1IF	: 1;
	unsigned char TXIF		: 1;
	unsigned char RCIF		: 1;
	unsigned char ADIF		: 1;
	unsigned char TMR1GIF	: 1;
} PIR1_t;
#define PIR1bits		(PIR1_t*)(&PIR1)

typedef struct {
	unsigned char dummy1	: 3;
	unsigned char BCLIF		: 1;
	unsigned char EEIF		: 1;
	unsigned char C1IF		: 1;
	unsigned char dummy2	: 1;
	unsigned char OSFIF		: 1;
} PIR2_t;
#define PIR2bits		(PIR2_t*)(&PIR2)

typedef struct {
	unsigned char TMR1ON	: 1;
	unsigned char dummy		: 1;
	unsigned char T1SYNC	: 1;
	unsigned char T1OSCEN	: 1;
	unsigned char T1CKPS	: 2;
	unsigned char TMR1CS0	: 1;
	unsigned char TMR1CS1	: 1;
} T1CON_t;
#define T1CONbits		(T1CON_t*)(&T1CON)

typedef struct {
	unsigned char T1GSS		: 2;
	unsigned char T1GVAL	: 1;
	unsigned char T1GGO		: 1;
	unsigned char T1GSPM	: 1;
	unsigned char T1GTM		: 1;
	unsigned char T1GPOL	: 1;
	unsigned char TMR1GE	: 1;
} T1GCON_t;
#define T1GCONbits		(T1GCON_t*)(&T1GCON)

typedef struct {
	unsigned char T2CKPS	: 2;
	unsigned char TMR2ON	: 1;
	unsigned char T2OUTPS	: 4;
} T2CON_t;
#define T2CONbits		(T2CON_t*)(&T2CON)

typedef struct {
	unsigned char IOCIF		: 1;
	unsigned char INTF		: 1;
	unsigned char TMR0IF	: 1;
	unsigned char IOCIE		: 1;
	unsigned char INTE		: 1;
	unsigned char TMR0IE	: 1;
	unsigned char PEIE		: 1;
	unsigned char GIE		: 1;
} INTCON_t;
#define INTCONbits		(INTCON_t*)(&INTCON)

typedef struct {
	unsigned char TMR1IE	: 1;
	unsigned char TMR2IE	: 1;
	unsigned char CCP1IE	: 1;
	unsigned char SSP1IE	: 1;
	unsigned char TXIE		: 1;
	unsigned char RCIE		: 1;
	unsigned char ADIE		: 1;
	unsigned char TMR1GIE	: 1;
} PIE1_t;
#define PIE1bits		(PIE1_t*)(&PIE1)

typedef struct {
	unsigned char dummy1	: 3;
	unsigned char BCLIE		: 1;
	unsigned char EEIE		: 1;
	unsigned char C1IE		: 1;
	unsigned char dummy2	: 1;
	unsigned char OSFIE		: 1;
} PIE2_t;
#define PIE2bits		(PIE2_t*)(&PIE2)

typedef struct {
	unsigned char PS		: 3;
	unsigned char PSA		: 1;
	unsigned char TMR0SE	: 1;
	unsigned char TMR0CS	: 1;
	unsigned char INTEDG	: 1;
	unsigned char WPUEN		: 1;
} OPTION_REG_t;
#define OPTION_REGbits	(OPTION_REG_t*)(&OPTION_REG)

typedef struct {
	unsigned char BOR		: 1;
	unsigned char POR		: 1;
	unsigned char RI		: 1;
	unsigned char RMCLR		: 1;
	unsigned char RWDT		: 1;
	unsigned char dummy		: 1;
	unsigned char STKUNF	: 1;
	unsigned char STKOVF	: 1;
} PCON_t;
#define PCONbits		(PCON_t*)(&PCON)

typedef struct {
	unsigned char SWDTEN	: 1;
	unsigned char WDTPS		: 5;
} WDTCON_t;
#define WDTCONbits		(WDTCON_t*)(&WDTCON)

typedef struct {
	unsigned char SCS		: 2;
	unsigned char IRCF		: 4;
	unsigned char SPLLMULT	: 1;
	unsigned char SPLLEN	: 1;
} OSCCON_t;
#define OSCCONbits		(OSCCON_t*)(&OSCCON)

typedef struct {
	unsigned char HFIOFS	: 1;
	unsigned char LFIOFR	: 1;
	unsigned char MFIOFR	: 1;
	unsigned char HFIOFL	: 1;
	unsigned char HFIOFR	: 1;
	unsigned char OSTS		: 1;
	unsigned char PLLRDY	: 1;
	unsigned char T1OSCR	: 1;
} OSCSTAT_t;
#define OSCSTATbits		(OSCSTAT_t*)(&OSCSTAT)

typedef struct {
	unsigned char ADON		: 1;
	unsigned char GO		: 1;
	unsigned char CHS		: 5;
} ADCON0_t;
#define ADCON0bits		(ADCON0_t*)(&ADCON0)

typedef struct {
	unsigned char ADPREF	: 2;
	unsigned char dummy		: 2;
	unsigned char ADCS		: 3;
	unsigned char ADFM		: 1;
} ADCON1_t;
#define ADCON1bits		(ADCON1_t*)(&ADCON1)

typedef struct {
	unsigned char C1SYNC	: 1;
	unsigned char C1HYS		: 1;
	unsigned char C1SP		: 1;
	unsigned char dummy		: 1;
	unsigned char C1POL		: 1;
	unsigned char C1OE		: 1;
	unsigned char C1OUT		: 1;
	unsigned char C1ON		: 1;
} CM1CON0_t;
#define CM1CON0bits		(CM1CON0_t*)(&CM1CON0)

typedef struct {
	unsigned char C1NCH		: 1;
	unsigned char dummy		: 3;
	unsigned char C1PCH		: 2;
	unsigned char C1INTN	: 1;
	unsigned char C1INTP	: 1;
} CM1CON1_t;
#define CM1CON1bits		(CM1CON1_t*)(&CM1CON1)

typedef struct {
	unsigned char MC1OUT	: 1;
} CMOUT_t;
#define CMOUTbits		(CMOUT_t*)(&CMOUT)

typedef struct {
	unsigned char BORRDY	: 1;
	unsigned char dummy		: 5;
	unsigned char BORFS		: 1;
	unsigned char SBOREN	: 1;
} BORCON_t;
#define BORCONbits		(BORCON_t*)(&BORCON)

typedef struct {
	unsigned char ADFVR		: 2;
	unsigned char CDAFVR	: 2;
	unsigned char TSRNG		: 1;
	unsigned char TSEN		: 1;
	unsigned char FVRRDY	: 1;
	unsigned char FVREN		: 1;
} FVRCON_t;
#define FVRCONbits		(FVRCON_t*)(&FVRCON)

typedef struct {
	unsigned char dummy1	: 2;
	unsigned char DACPSS	: 2;
	unsigned char dummy2	: 1;
	unsigned char DACOE		: 1;
	unsigned char DACLPS	: 1;
	unsigned char DACEN		: 1;
} DACCON0_t;
#define DACCON0bits		(DACCON0_t*)(&DACCON0)

typedef struct {
	unsigned char DACR		: 5;
} DACCON1_t;
#define DACCON1bits		(DACCON1_t*)(&DACCON1)

typedef struct {
	unsigned char dummy		: 4;
	unsigned char CKP		: 1;
	unsigned char SSPEN		: 1;
	unsigned char SSPOV		: 1;
	unsigned char WCOL		: 1;
} SSP1CON1_t;
#define SSP1CON1bits	(SSP1CON1_t*)(&SSP1CON1)

typedef struct {
	unsigned char SEN		: 1;
	unsigned char RSEN		: 1;
	unsigned char PEN		: 1;
	unsigned char RCEN		: 1;
	unsigned char ACKEN		: 1;
	unsigned char ACKDT		: 1;
	unsigned char ACKSTAT	: 1;
	unsigned char GCEN		: 1;
} SSP1CON2_t;
#define SSP1CON2bits	(SSP1CON2_t*)(&SSP1CON2)

typedef struct {
	unsigned char DHEN		: 1;
	unsigned char AHEN		: 1;
	unsigned char SBCDE		: 1;
	unsigned char SDAHT		: 1;
	unsigned char BOEN		: 1;
	unsigned char SCIE		: 1;
	unsigned char PCIE		: 1;
	unsigned char ACKTIM	: 1;
} SSP1CON3_t;
#define SSP1CON3bits	(SSP1CON3_t*)(&SSP1CON3)

typedef struct {
	unsigned char BF		: 1;
	unsigned char UA		: 1;
	unsigned char RW		: 1;
	unsigned char S			: 1;
	unsigned char P			: 1;
	unsigned char DA		: 1;
	unsigned char CKE		: 1;
	unsigned char SMP		: 1;
} SSP1STAT_t;
#define SSP1STATbits	(SSP1STAT_t*)(&SSP1STAT)

typedef struct {
	unsigned char dummy1	: 2;
	unsigned char P2SEL		: 1;
	unsigned char T1GSEL	: 1;
	unsigned char dummy2	: 1;
	unsigned char SSSEL		: 1;
	unsigned char SDOSEL	: 1;
	unsigned char CLKRSEL	: 1;
} APFCON_t;
#define APFCONbits		(APFCON_t*)(&APFCON)

typedef struct {
	unsigned char SRPR		: 1;
	unsigned char SRPS		: 1;
	unsigned char SRNQEN	: 1;
	unsigned char SRQEN		: 1;
	unsigned char SRCLK		: 3;
	unsigned char SRLEN		: 1;
} SRCON0_t;
#define SRCON0bits		(SRCON0_t*)(&SRCON0)

typedef struct {
	unsigned char SRRC1E	: 1;
	unsigned char dummy1	: 1;
	unsigned char SRRCKE	: 1;
	unsigned char SRRPE		: 1;
	unsigned char SRSC1E	: 1;
	unsigned char dummy2	: 1;
	unsigned char SRSCKE	: 1;
	unsigned char SRSPE		: 1;
} SRCON1_t;
#define SRCON1bits		(SRCON1_t*)(&SRCON1)

typedef struct {
	unsigned char RD		: 1;
	unsigned char WR		: 1;
	unsigned char WREN		: 1;
	unsigned char WRERR		: 1;
	unsigned char FREE		: 1;
	unsigned char LWLO		: 1;
	unsigned char CFGS		: 1;
	unsigned char EEPGD		: 1;
} EECON1_t;
#define EECON1bits		(EECON1_t*)(&EECON1)

typedef struct {
	unsigned char CCP1M		: 4;
	unsigned char DC1B		: 2;
	unsigned char P1M		: 2;
} CCP1CON_t;
#define CCP1CONbits		(CCP1CON_t*)(&CCP1CON)

typedef struct {
	unsigned char P1DC		: 7;
	unsigned char P1RSEN	: 1;
} PWM1CON_t;
#define PWM1CONbits		(PWM1CON_t*)(&PWM1CON)

typedef struct {
	unsigned char PSS1BD	: 2;
	unsigned char PSS1AC	: 2;
	unsigned char CCP1_AS	: 3;
	unsigned char CCP1_ASE	: 1;
} CCP1AS_t;
#define CCP1ASbits		(CCP1AS_t*)(&CCP1AS)

typedef struct {
	unsigned char STR1A		: 1;
	unsigned char STR1B		: 1;
	unsigned char dummy		: 2;
	unsigned char STR1STNC	: 1;
} PSTR1CON_t;
#define PSTR1CONbits	(PSTR1CON_t*)(&PSTR1CON)

typedef struct {
	unsigned char IOCAP0	: 1;
	unsigned char IOCAP1	: 1;
	unsigned char IOCAP2	: 1;
	unsigned char IOCAP3	: 1;
	unsigned char IOCAP4	: 1;
	unsigned char IOCAP5	: 1;
} IOCAP_t;
#define IOCAPbits		(IOCAP_t*)(&IOCAP)

typedef struct {
	unsigned char IOCAN0	: 1;
	unsigned char IOCAN1	: 1;
	unsigned char IOCAN2	: 1;
	unsigned char IOCAN3	: 1;
	unsigned char IOCAN4	: 1;
	unsigned char IOCAN5	: 1;
} IOCAN_t;
#define IOCANbits		(IOCAN_t*)(&IOCAN)

typedef struct {
	unsigned char IOCAF0	: 1;
	unsigned char IOCAF1	: 1;
	unsigned char IOCAF2	: 1;
	unsigned char IOCAF3	: 1;
	unsigned char IOCAF4	: 1;
	unsigned char IOCAF5	: 1;
} IOCAF_t;
#define IOCAFbits		(IOCAF_t*)(&IOCAF)

typedef struct {
	unsigned char CLKRDIV	: 3;
	unsigned char CLKRDC	: 2;
	unsigned char CLKRSLR	: 1;
	unsigned char CLKROE	: 1;
	unsigned char CLKREN	: 1;
} CLKRCON_t;
#define CLKRCONbits		(CLKRCON_t*)(&CLKRCON)

typedef struct {
	unsigned char MDBIT		: 1;
	unsigned char dummy		: 2;
	unsigned char MDOUT		: 1;
	unsigned char MDOPOL	: 1;
	unsigned char MDSLR		: 1;
	unsigned char MDOE		: 1;
	unsigned char MDEN		: 1;
} MDCON_t;
#define MDCONbits		(MDCON_t*)(&MDCON)

typedef struct {
	unsigned char MDMS		: 4;
	unsigned char dummy		: 3;
	unsigned char MDMSODIS	: 1;
} MDSRC_t;
#define MDSRCbits		(MDSRC_t*)(&MDSRC)

typedef struct {
	unsigned char MDCL		: 4;
	unsigned char dummy		: 1;
	unsigned char MDCLSYNC	: 1;
	unsigned char MDCLPOL	: 1;
	unsigned char MDCLODIS	: 1;
} MDCARL_t;
#define MDCARLbits		(MDCARL_t*)(&MDCARL)

typedef struct {
	unsigned char MDCH		: 4;
	unsigned char dummy		: 1;
	unsigned char MDCHSYNC	: 1;
	unsigned char MDCHPOL	: 1;
	unsigned char MDCHODIS	: 1;
} MDCARH_t;
#define MDCARHbits		(MDCARH_t*)(&MDCARH)


#define FUSE0          (0x8007)
	#define _FCMEN_ENA		0x3fff	// bit13	- Fail-safe clock monitor enable
	#define _FCMEN_DIS		0x1fff	// bit13	- Fail-safe clock monitor disable
	#define _IESO_ENA		0x3fff	// bit12	- Int/Ext swichover enable
	#define _IESO_DIS		0x2fff	// bit12	- Int/Ext swichover disable
	#define _CLKOUTEN_ENA	0x37ff	// bit11 	- Clock out enable
	#define _BOR_DIS		0x39ff	// bit10-9	- Brown-out reset enable
	#define _BOR_SBOREN		0x3bff	// bit10-9
	#define _BOR_SLEEP		0x3dff	// bit10-9
	#define _CP_ENA			0x3f7f	// bit7		- Code Protection
	#define _MCLR_DIS		0x3fbf	// bit6		- MCLR/VPP pin function select
	#define _PWRT_ENA		0x3fdf	// bit5		- Power-up timer enable
	#define _WDT_ENA_RUN	0x3ff7	// bit4-3	- Watchdog timer enable
	#define _WDT_SWDTEN		0x3fef	// bit4-3
	#define _WDT_DIS		0x3fe7	// bit4-3
	#define _FOSC_ECH		0x3fff	// bit2-0	- Oscillator selection
	#define _FOSC_ECM		0x3ffe	// bit2-0
	#define _FOSC_ECL		0x3ffd	// bit2-0
	#define _FOSC_INTOSC	0x3ffc	// bit2-0
	#define _FOSC_EXTRC		0x3ffb	// bit2-0
	#define _FOSC_HS		0x3ffa	// bit2-0
	#define _FOSC_XT		0x3ff9	// bit2-0
	#define _FOSC_LP		0x3ff8	// bit2-0
#define FUSE1          (0x8008)
	#define _LVP_HV_ONLY	0x1fff	// bit13
	#define _DEBUG_ENA		0x2fff	// bit12
	#define _LPBOR_ENA		0x37ff	// bit11
	#define _BORV_H_TRIP	0x3bff	// bit10
	#define _STVREN_ENA		0x3dff	// bit9
	#define _STVREN_DIS		0x3fff	// bit9
	#define _PLL_DISABLE	0x3eff	// bit8
	#define _FLASH_WP_01FF	0x3ffe	// bit1-0
	#define _FLASH_WP_0FFF	0x3ffd	// bit1-0
	#define _FLASH_WP_1FFF	0x3ffc	// bit1-0

#endif
