#ifndef _STM8S105C6_H
#define _STM8S105C6_H

#include <stm8s.h>

#define __DEVICE "stm8s105c6"

#define __RAM_SIZE			2048
#define __FLASH_SIZE    	(32*1024)
#define __STACK_INIT_ADDR	(__RAM_START + __RAM_SIZE - 1)

#define __RESET_VECTOR__	0x8000
#define __TRAP_VECTOR__		0x8004
#define __TLI_VECTOR__		0x8008
#define __AWU_VECTOR__		0x800c
#define __CLK_VECTOR__		0x8010
#define __EXTI0_VECTOR__	0x8014
#define __EXTI1_VECTOR__	0x8018
#define __EXTI2_VECTOR__	0x801c
#define __EXTI3_VECTOR__	0x8020
#define __EXTI4_VECTOR__	0x8024
#define __SPI_VECTOR__		0x8030
#define __TIM1UPD_VECTOR__	0x8034
#define __TIM1CAP_VECTOR__	0x8038
#define __TIM2UPD_VECTOR__	0x803c
#define __TIM2CAP_VECTOR__	0x8040
#define __TIM3UPD_VECTOR__	0x8044
#define __TIM3CAP_VECTOR__	0x8048
#define __I2C_VECTOR__		0x8054
#define __UART2_TX_CMPL_VECTOR__  0x8058
#define __UART2_RX_FULL_VECTOR__  0x805c
#define __ADC1_VECTOR__		0x8060
#define __TIM4UPD_VECTOR__	0x8064
#define __FLASH_VECTOR__	0x8068

#define	PORTA		(IO_Port_t*)0x5000
#define	PORTB		(IO_Port_t*)0x5005
#define	PORTC		(IO_Port_t*)0x500a
#define	PORTD		(IO_Port_t*)0x500f
#define	PORTE		(IO_Port_t*)0x5014
#define	PORTF		(IO_Port_t*)0x5019
#define	PORTG		(IO_Port_t*)0x501e
#define	PORTH		(IO_Port_t*)0x5023
#define	PORTI		(IO_Port_t*)0x5028

#define PA_ODR		REG8(0x5000)
#define PA_IDR		REG8(0x5001)
#define PA_DDR		REG8(0x5002)
#define PA_CR1		REG8(0x5003)
#define PA_CR2		REG8(0x5004)

#define PB_ODR		REG8(0x5005)
#define PB_IDR		REG8(0x5006)
#define PB_DDR		REG8(0x5007)
#define PB_CR1		REG8(0x5008)
#define PB_CR2		REG8(0x5009)

#define PC_ODR		REG8(0x500a)
#define PC_IDR		REG8(0x500b)
#define PC_DDR		REG8(0x500c)
#define PC_CR1		REG8(0x500d)
#define PC_CR2		REG8(0x500e)

#define PD_ODR		REG8(0x500f)
#define PD_IDR		REG8(0x5010)
#define PD_DDR		REG8(0x5011)
#define PD_CR1		REG8(0x5012)
#define PD_CR2		REG8(0x5013)

#define PE_ODR		REG8(0x5014)
#define PE_IDR		REG8(0x5015)
#define PE_DDR		REG8(0x5016)
#define PE_CR1		REG8(0x5017)
#define PE_CR2		REG8(0x5018)

#define PF_ODR		REG8(0x5019)
#define PF_IDR		REG8(0x501a)
#define PF_DDR		REG8(0x501b)
#define PF_CR1		REG8(0x501c)
#define PF_CR2		REG8(0x501d)

#define PG_ODR		REG8(0x501e)
#define PG_IDR		REG8(0x501f)
#define PG_DDR		REG8(0x5020)
#define PG_CR1		REG8(0x5021)
#define PG_CR2		REG8(0x5022)

#define PH_ODR		REG8(0x5023)
#define PH_IDR		REG8(0x5024)
#define PH_DDR		REG8(0x5025)
#define PH_CR1		REG8(0x5026)
#define PH_CR2		REG8(0x5027)

#define PI_ODR		REG8(0x5028)
#define PI_IDR		REG8(0x5029)
#define PI_DDR		REG8(0x502a)
#define PI_CR1		REG8(0x502b)
#define PI_CR2		REG8(0x502c)

#define PA_ODRbits	(REG8_t*)(&PA_ODR)
#define PA_IDRbits	(REG8_t*)(&PA_IDR)
#define PA_DDRbits	(REG8_t*)(&PA_DDR)
#define PA_CR1bits	(REG8_t*)(&PA_CR1)
#define PA_CR2bits	(REG8_t*)(&PA_CR2)
#define PB_ODRbits	(REG8_t*)(&PB_ODR)
#define PB_IDRbits	(REG8_t*)(&PB_IDR)
#define PB_DDRbits	(REG8_t*)(&PB_DDR)
#define PB_CR1bits	(REG8_t*)(&PB_CR1)
#define PB_CR2bits	(REG8_t*)(&PB_CR2)
#define PC_ODRbits	(REG8_t*)(&PC_ODR)
#define PC_IDRbits	(REG8_t*)(&PC_IDR)
#define PC_DDRbits	(REG8_t*)(&PC_DDR)
#define PC_CR1bits	(REG8_t*)(&PC_CR1)
#define PC_CR2bits	(REG8_t*)(&PC_CR2)
#define PD_ODRbits	(REG8_t*)(&PD_ODR)
#define PD_IDRbits	(REG8_t*)(&PD_IDR)
#define PD_DDRbits	(REG8_t*)(&PD_DDR)
#define PD_CR1bits	(REG8_t*)(&PD_CR1)
#define PD_CR2bits	(REG8_t*)(&PD_CR2)
#define PE_ODRbits	(REG8_t*)(&PE_ODR)
#define PE_IDRbits	(REG8_t*)(&PE_IDR)
#define PE_DDRbits	(REG8_t*)(&PE_DDR)
#define PE_CR1bits	(REG8_t*)(&PE_CR1)
#define PE_CR2bits	(REG8_t*)(&PE_CR2)
#define PF_ODRbits	(REG8_t*)(&PF_ODR)
#define PF_IDRbits	(REG8_t*)(&PF_IDR)
#define PF_DDRbits	(REG8_t*)(&PF_DDR)
#define PF_CR1bits	(REG8_t*)(&PF_CR1)
#define PF_CR2bits	(REG8_t*)(&PF_CR2)
#define PG_ODRbits	(REG8_t*)(&PG_ODR)
#define PG_IDRbits	(REG8_t*)(&PG_IDR)
#define PG_DDRbits	(REG8_t*)(&PG_DDR)
#define PG_CR1bits	(REG8_t*)(&PG_CR1)
#define PG_CR2bits	(REG8_t*)(&PG_CR2)
#define PH_ODRbits	(REG8_t*)(&PH_ODR)
#define PH_IDRbits	(REG8_t*)(&PH_IDR)
#define PH_DDRbits	(REG8_t*)(&PH_DDR)
#define PH_CR1bits	(REG8_t*)(&PH_CR1)
#define PH_CR2bits	(REG8_t*)(&PH_CR2)
#define PI_ODRbits	(REG8_t*)(&PI_ODR)
#define PI_IDRbits	(REG8_t*)(&PI_IDR)
#define PI_DDRbits	(REG8_t*)(&PI_DDR)
#define PI_CR1bits	(REG8_t*)(&PI_CR1)
#define PI_CR2bits	(REG8_t*)(&PI_CR2)


#define FLASH_CR1	REG8(0x505a)
#define FLASH_CR2	REG8(0x505b)
#define FLASH_NCR2	REG8(0x505c)
#define FLASH_FPR	REG8(0x505d)
#define FLASH_NFPR	REG8(0x505e)
#define FLASH_IAPSR	REG8(0x505f)

#define FLASH_PUKR	REG8(0x5062)
#define FLASH_DUKR	REG8(0x5064)

#define EXTI_CR1	REG8(0x50a0)
#define EXTI_CR2	REG8(0x50a1)
#define RST_SR		REG8(0x50b3)

#define CLK_ICKR	REG8(0x50c0)
#define CLK_ECKR	REG8(0x50c1)
#define CLK_CMSR	REG8(0x50c3)
#define CLK_SWR		REG8(0x50c4)
#define CLK_SWCR	REG8(0x50c5)
#define CLK_CKDIVR	REG8(0x50c6)
#define CLK_PCKENR1	REG8(0x50c7)
#define CLK_CSSR	REG8(0x50c8)
#define CLK_CCOR	REG8(0x50c9)
#define CLK_PCKENR2	REG8(0x50ca)
#define CLK_CANCCR	REG8(0x50cb)
#define CLK_HSITRIMR	REG8(0x50cc)
#define CLK_SWIMCCR	REG8(0x50cd)

#define WWDG_CR		REG8(0x50d1)
#define WWDG_WR		REG8(0x50d2)

#define IWDG_KR		REG8(0x50e0)
#define IWDG_PR		REG8(0x50e1)
#define IWDG_RLR	REG8(0x50e2)

#define AWU_CSR1	REG8(0x50f0)
#define AWU_APR		REG8(0x50f1)
#define AWU_TBR		REG8(0x50f2)
#define BEEP_CSR	REG8(0x50f3)

#define SPI_CR1		REG8(0x5200)
#define SPI_CR2		REG8(0x5201)
#define SPI_ICR		REG8(0x5202)
#define SPI_SR		REG8(0x5203)
#define SPI_DR		REG8(0x5204)
#define SPI_CRCPR	REG8(0x5205)
#define SPI_RXCRCR	REG8(0x5206)
#define SPI_TXCRCR	REG8(0x5207)

#define I2C_CR1		REG8(0x5210)
#define I2C_CR2		REG8(0x5211)
#define I2C_FREQR	REG8(0x5212)
#define I2C_OARL	REG8(0x5213)
#define I2C_OARH	REG8(0x5214)
#define I2C_DR		REG8(0x5216)
#define I2C_SR1		REG8(0x5217)
#define I2C_SR2		REG8(0x5218)
#define I2C_SR3		REG8(0x5219)
#define I2C_ITR		REG8(0x521a)
#define I2C_CCRL	REG8(0x521b)
#define I2C_CCRH	REG8(0x521c)
#define I2C_TRISER	REG8(0x521d)
#define I2C_PECR	REG8(0x521e)

#define UART2_SR	REG8(0x5240)
#define UART2_DR	REG8(0x5241)
#define UART2_BRR1	REG8(0x5242)
#define UART2_BRR2	REG8(0x5243)
#define UART2_CR1	REG8(0x5244)
#define UART2_CR2	REG8(0x5245)
#define UART2_CR3	REG8(0x5246)
#define UART2_CR4	REG8(0x5247)
#define UART2_CR6	REG8(0x5249)
#define UART2_GTR	REG8(0x524a)
#define UART2_PSCR	REG8(0x524b)

#define TIM1_CR1	REG8(0x5250)
#define TIM1_CR2	REG8(0x5251)
#define TIM1_SMCR	REG8(0x5252)
#define TIM1_ETR	REG8(0x5253)
#define TIM1_IER	REG8(0x5254)
#define TIM1_SR1	REG8(0x5255)
#define TIM1_SR2	REG8(0x5256)
#define TIM1_EGR	REG8(0x5257)
#define TIM1_CCMR1	REG8(0x5258)
#define TIM1_CCMR2	REG8(0x5259)
#define TIM1_CCMR3	REG8(0x525a)
#define TIM1_CCMR4	REG8(0x525b)
#define TIM1_CCER1	REG8(0x525c)
#define TIM1_CCER2	REG8(0x525d)
#define TIM1_CNTRH	REG8(0x525e)
#define TIM1_CNTRL	REG8(0x525f)
#define TIM1_PSCRH	REG8(0x5260)
#define TIM1_PSCRL	REG8(0x5261)
#define TIM1_ARRH	REG8(0x5262)
#define TIM1_ARRL	REG8(0x5263)
#define TIM1_RCR	REG8(0x5264)
#define TIM1_CCR1H	REG8(0x5265)
#define TIM1_CCR1L	REG8(0x5266)
#define TIM1_CCR2H	REG8(0x5267)
#define TIM1_CCR2L	REG8(0x5268)
#define TIM1_CCR3H	REG8(0x5269)
#define TIM1_CCR3L	REG8(0x526a)
#define TIM1_CCR4H	REG8(0x526b)
#define TIM1_CCR4L	REG8(0x526c)
#define TIM1_BKR	REG8(0x526d)
#define TIM1_DTR	REG8(0x526e)
#define TIM1_OISR	REG8(0x526f)

#define TIM2_CR1	REG8(0x5300)
#define TIM2_IER	REG8(0x5301)
#define TIM2_SR1	REG8(0x5302)
#define TIM2_SR2	REG8(0x5303)
#define TIM2_EGR	REG8(0x5304)
#define TIM2_CCMR1	REG8(0x5305)
#define TIM2_CCMR2	REG8(0x5306)
#define TIM2_CCMR3	REG8(0x5307)
#define TIM2_CCER1	REG8(0x5308)
#define TIM2_CCER2	REG8(0x5309)
#define TIM2_CNTRH	REG8(0x530a)
#define TIM2_CNTRL	REG8(0x530b)
#define TIM2_PSCR	REG8(0x530c)
#define TIM2_ARRH	REG8(0x530d)
#define TIM2_ARRL	REG8(0x530e)
#define TIM2_CCR1H	REG8(0x530f)
#define TIM2_CCR1L	REG8(0x5310)
#define TIM2_CCR2H	REG8(0x5311)
#define TIM2_CCR2L	REG8(0x5312)
#define TIM2_CCR3H	REG8(0x5313)
#define TIM2_CCR3L	REG8(0x5314)

#define TIM3_CR1	REG8(0x5320)
#define TIM3_IER	REG8(0x5321)
#define TIM3_SR1	REG8(0x5322)
#define TIM3_SR2	REG8(0x5323)
#define TIM3_EGR	REG8(0x5324)
#define TIM3_CCMR1	REG8(0x5325)
#define TIM3_CCMR2	REG8(0x5326)
#define TIM3_CCER1	REG8(0x5327)
#define TIM3_CNTRH	REG8(0x5328)
#define TIM3_CNTRL	REG8(0x5329)
#define TIM3_PSCR	REG8(0x532a)
#define TIM3_ARRH	REG8(0x532b)
#define TIM3_ARRL	REG8(0x532c)
#define TIM3_CCR1H	REG8(0x532d)
#define TIM3_CCR1L	REG8(0x532e)
#define TIM3_CCR2H	REG8(0x532f)
#define TIM3_CCR2L	REG8(0x5330)

#define TIM4_CR1	REG8(0x5340)
#define TIM4_IER	REG8(0x5341)
#define TIM4_SR		REG8(0x5342)
#define TIM4_EGR	REG8(0x5343)
#define TIM4_CNTR	REG8(0x5344)
#define TIM4_PSCR	REG8(0x5345)
#define TIM4_ARR	REG8(0x5346)

#define ADC_DBxR	REG8(0x53e0)
#define ADC_CSR		REG8(0x5400)
#define ADC_CR1		REG8(0x5401)
#define ADC_CR2		REG8(0x5402)
#define ADC_CR3		REG8(0x5403)
#define ADC_DRH		REG8(0x5404)
#define ADC_DRL		REG8(0x5405)
#define ADC_TDRH	REG8(0x5406)
#define ADC_TDRL	REG8(0x5407)
#define ADC_HTRH	REG8(0x5408)
#define ADC_HTRL	REG8(0x5409)
#define ADC_LTRH	REG8(0x540a)
#define ADC_LTRL	REG8(0x540b)
#define ADC_AWSRH	REG8(0x540c)
#define ADC_AWSRL	REG8(0x540d)
#define ADC_AWCRH	REG8(0x540e)
#define ADC_AWCRL	REG8(0x540f)

#endif
