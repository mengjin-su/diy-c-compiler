#ifndef _STM8S_H
#define _STM8S_H

typedef char 			int8_t;
typedef int  			int16_t;
typedef short 			int24_t;
typedef long			int32_t;

typedef unsigned char 	uint8_t;
typedef unsigned int  	uint16_t;
typedef unsigned short 	uint24_t;
typedef unsigned long	uint32_t;

#define NULL			0

//////////////////////////////////////////////////////////////
#define REG8(a)		(*(uint8_t*)a)

typedef struct {
	uint8_t		ODR;
	uint8_t		IDR;
	uint8_t		DDR;
	uint8_t		CR1;
	uint8_t		CR2;
} IO_Port_t;

typedef struct {
	char		b0: 1;
	char		b1: 1;
	char		b2: 1;
	char		b3: 1;
	char		b4: 1;
	char		b5: 1;
	char		b6: 1;
	char		b7: 1;
} REG8_t;

typedef struct {
	char CPHA		: 1;
	char CPOL		: 1;
	char MSTR		: 1;
	char BR			: 3;
	char SPE		: 1;
	char LSB_FIRST	: 1;
} SPI_CR1_t;

typedef struct {
	char SSI		: 1;
	char SSM		: 1;
	char RXONLY		: 1;
	char _			: 1;
	char CRCNEXT	: 1;
	char CRCEN		: 1;
	char BDOE		: 1;
	char BDM		: 1;
} SPI_CR2_t;

typedef struct {
	char _			: 4;
	char WKIE		: 1;
	char ERRIE		: 1;
	char RXIE		: 1;
	char TXIE		: 1;
} SPI_ICR_t;

typedef struct {
	char RXNE		: 1;
	char TXE		: 1;
	char _			: 1;
	char WKUP		: 1;
	char CRCERR		: 1;
	char MODF		: 1;
	char OVR		: 1;
	char BSY		: 1;
} SPI_SR_t;

typedef struct {
	char PE			: 1;
	char _			: 5;
	char ENGC		: 1;
	char NOSTRETCH	: 1;
} I2C_CR1_t;

typedef struct {
	char START		: 1;
	char STOP		: 1;
	char ACK		: 1;
	char POS		: 1;
	char _			: 3;
	char SWRST		: 1;
} I2C_CR2_t;

typedef struct {
	char PREQ		: 6;
} I2C_FREQR_t;

typedef struct {
	char ADD0		: 1;
	char ADD7_1		: 7;
} I2C_OARL_t;

typedef struct {
	char _			: 1;
	char ADD9_8		: 2;
	char __			: 3;
	char ADDCONF	: 1;
	char ADDMODE	: 1;
} I2C_OARH_t;

typedef struct {
	char SB			: 1;
	char ADDR		: 1;
	char BTF		: 1;
	char ADD10		: 1;
	char STOPF		: 1;
	char _			: 1;
	char RXNE		: 1;
	char TXE		: 1;
} I2C_SR1_t;

typedef struct {
	char BERR		: 1;
	char ARLO		: 1;
	char AF			: 1;
	char OVR		: 1;
	char _			: 1;
	char WUFH		: 1;
} I2C_SR2_t;

typedef struct {
	char MSL		: 1;
	char BUSY		: 1;
	char TRA		: 1;
	char _			: 1;
	char GENCALL	: 1;
	char __			: 2;
	char DUALF		: 1;
} I2C_SR3_t;

typedef struct {
	char ITERREN	: 1;
	char ITEVTEN	: 1;
	char ITBUFEN	: 1;
} I2C_ITR_t;

typedef struct {
	char CCR11_8	: 4;
	char _			: 2;
	char DUTY		: 1;
	char FS			: 1;
} I2C_CCRH_t;

typedef struct {
	char TRISE		: 6;
} I2C_TRISER_t;

typedef struct {
	char PE			: 1;
	char FE			: 1;
	char NF			: 1;
	char OR_LHE		: 1;
	char IDLE		: 1;
	char RXNE		: 1;
	char TC			: 1;
	char TXE		: 1;
} UART_SR_t;

typedef struct {
	char UART_DIV11_4: 8;
} UART_BRR1_t;

typedef struct {
	char UART_DIV3_0: 4;
	char UART_DIV15_12: 4;
} UART_BRR2_t;

typedef struct {
	char PIEN		: 1;
	char PS			: 1;
	char PCEN		: 1;
	char WAKE		: 1;
	char M			: 1;
	char UARTD		: 1;
	char T8			: 1;
	char R8			: 1;
} UART_CR1_t;

typedef struct {
	char SBK		: 1;
	char RWU		: 1;
	char REN		: 1;
	char TEN		: 1;
	char ILIEN		: 1;
	char RIEN		: 1;
	char TCIEN		: 1;
	char TIEN		: 1;
} UART_CR2_t;

typedef struct {
	char LBCL		: 1;
	char CPHA		: 1;
	char CPOL		: 1;
	char CLKEN		: 1;
	char STOP		: 2;
	char LINEN		: 1;
} UART_CR3_t;

typedef struct {
	char ADD		: 4;
	char LBDF		: 1;
	char LBDL		: 1;
	char LBDIEN		: 1;
} UART_CR4_t;

typedef struct {
	char _			: 1;
	char IREN		: 1;
	char IRLP		: 1;
	char HDSEL		: 1;
	char NACK		: 1;
	char SCEN		: 1;
} UART_CR5_t;

typedef struct {
	char LSF		: 1;
	char LHDF		: 1;
	char LHDIEN		: 1;
	char _			: 1;
	char LASE		: 1;
	char LSLV		: 1;
	char __			: 1;
	char LDUM		: 1;
} UART_CR6_t;

/* 8-bit timer registers */
typedef struct {
	char CEN		: 1;
	char UDIS		: 1;
	char URS		: 1;
	char OPM		: 1;
	char DIR		: 1;
	char CMS		: 2;
	char ARPE		: 1;
} TIM1_CR1_t;

typedef struct {
	char CCPC		: 1;
	char _			: 1;
	char COMS		: 1;
	char __			: 1;
	char MMS		: 3;
} TIM1_CR2_t;

typedef struct {
	char SMS		: 3;
	char _			: 1;
	char TS			: 3;
	char MSM		: 1;
} TIM1_SMCR_t;

typedef struct {
	char ETF		: 4;
	char ETPS		: 2;
	char ECE		: 1;
	char ETP		: 1;
} TIM1_ETR_t;

typedef struct {
	char UIE		: 1;
	char CC1IE		: 1;
	char CC2IE		: 1;
	char CC3IE		: 1;
	char CC4IE		: 1;
	char COMIE		: 1;
	char TIE		: 1;
	char BIE		: 1;
} TIM1_IER_t;

typedef struct {
	char UIF		: 1;
	char CC1IF		: 1;
	char CC2IF		: 1;
	char CC3IF		: 1;
	char CC4IF		: 1;
	char COMIF		: 1;
	char TIF		: 1;
	char BIF		: 1;
} TIM1_SR1_t;

typedef struct {
	char _			: 1;
	char CC1OF		: 1;
	char CC2OF		: 1;
	char CC3OF		: 1;
	char CC4OF		: 1;
} TIM1_SR2_t;

typedef struct {
	char UG			: 1;
	char CC1G		: 1;
	char CC2G		: 1;
	char CC3G		: 1;
	char CC4G		: 1;
	char COMG		: 1;
	char TG			: 1;
	char BG			: 1;
} TIM1_EGR_t;

typedef struct {
	char CC1S		: 2;
	char OC1FE		: 1;
	char OC1PE		: 1;
	char OC1M		: 3;
	char OC1CE		: 1;
} TIM1_CCMR1_t;		// CC1S = output mode

typedef struct {
	char CC1S		: 2;
	char IC1PSC		: 2;
	char IC1F		: 4;
} TIM1_CCMR1_IN_t;	// CC1S = input mode

typedef struct {
	char CC2S		: 2;
	char OC2FE		: 1;
	char OC2PE		: 1;
	char OC2M		: 3;
	char OC2CE		: 1;
} TIM1_CCMR2_t;		// CC2S = output mode

typedef struct {
	char CC2S		: 2;
	char IC2PSC		: 2;
	char IC2F		: 4;
} TIM1_CCMR2_IN_t;	// CC2S = input mode

typedef struct {
	char CC3S		: 2;
	char OC3FE		: 1;
	char OC3PE		: 1;
	char OC3M		: 3;
	char OC3CE		: 1;
} TIM1_CCMR3_t;		// CC3S = output mode

typedef struct {
	char CC3S		: 2;
	char IC3PSC		: 2;
	char IC3F		: 4;
} TIM1_CCMR3_IN_t;	// CC3S = input mode

typedef struct {
	char CC4S		: 2;
	char OC4FE		: 1;
	char OC4PE		: 1;
	char OC4M		: 3;
	char OC4CE		: 1;
} TIM1_CCMR4_t;		// CC4S = output mode

typedef struct {
	char CC4S		: 2;
	char IC4PSC		: 2;
	char IC4F		: 4;
} TIM1_CCMR4_IN_t;	// CC4S = input mode

typedef struct {
	char CC1E		: 1;
	char CC1P		: 1;
	char CC1NE		: 1;
	char CC1NP		: 1;
	char CC2E		: 1;
	char CC2P		: 1;
	char CC2NE		: 1;
	char CC2NP		: 1;
} TIM1_CCER1_t;

typedef struct {
	char CC3E		: 1;
	char CC3P		: 1;
	char CC3NE		: 1;
	char CC3NP		: 1;
	char CC4E		: 1;
	char CC4P		: 1;
} TIM1_CCER2_t;

typedef struct {
	char LOCK		: 2;
	char OSSI		: 1;
	char OSSR		: 1;
	char BKE		: 1;
	char BKP		: 1;
	char AOE		: 1;
	char MOE		: 1;
} TIM1_BKR_t;

typedef struct {
	char OIS1		: 1;
	char OIS1N		: 1;
	char OIS2		: 1;
	char OIS2N		: 1;
	char OIS3		: 1;
	char OIS3N		: 1;
	char OIS4		: 1;
	char OIS4N		: 1;
} TIM1_OISR_t;

/* 8-bit timer registers */
typedef struct {
	char CEN		: 1;
	char UDIS		: 1;
	char URS		: 1;
	char OPM		: 1;
	char _			: 3;
	char ARPE		: 1;
} TIM2_CR1_t;

#endif