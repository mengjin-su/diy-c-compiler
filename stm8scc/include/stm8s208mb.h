#ifndef _STM8S208MB_H
#define _STM8S208MB_H
#include <stm8s.h>

#define __DEVICE "stm8s208mb"

#define __RAM_SIZE      (6*1024)
#define __FLASH_SIZE    (128*1024)

#define __RESET_VECTOR__		0x8000
#define __TRAP_VECTOR__			0x8004
#define __TLI_VECTOR__			0x8008
#define __AWU_VECTOR__			0x800c
#define __CLK_VECTOR__			0x8010
#define __EXTI0_VECTOR__		0x8014
#define __EXTI1_VECTOR__		0x8018
#define __EXTI2_VECTOR__		0x801c
#define __EXTI3_VECTOR__		0x8020
#define __EXTI4_VECTOR__		0x8024
#define __CAN_RX_VECTOR__   	0x8028
#define __CAN_TX_VECTOR__   	0x802c
#define __SPI_VECTOR__			0x8030
#define __TIM1UPD_VECTOR__		0x8034
#define __TIM1CAP_VECTOR__		0x8038
#define __TIM2UPD_VECTOR__		0x803c
#define __TIM2CAP_VECTOR__		0x8040
#define __TIM3UPD_VECTOR__		0x8044
#define __TIM3CAP_VECTOR__		0x8048
#define __UART1_TX_CMPL_VECTOR__ 0x804c
#define __UART1_RX_FULL_VECTOR__ 0x8050
#define __I2C_VECTOR__			0x8054
#define __UART3_TX_CMPL_VECTOR__ 0x8058
#define __UART3_RX_FULL_VECTOR__ 0x805c
#define __ADC2_VECTOR__     	0x8060
#define __TIM4UPD_VECTOR__		0x8064
#define __FLASH_VECTOR__		0x8068

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



#endif
