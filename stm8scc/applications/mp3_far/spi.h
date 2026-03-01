#ifndef _SPI_H
#define _SPI_H

#define SPI_SCK_PIN 	0	// PG.0
#define SPI_MOSI_PIN	6	// PA.6
#define SPI_MISO_PIN	4	// PC.4

#define SPI_CONFIG		sbi(PA_DDR, SPI_MOSI_PIN),sbi(PA_CR1, SPI_MOSI_PIN), cbi(PA_CR2, SPI_MOSI_PIN), \
						sbi(PG_DDR, SPI_SCK_PIN), sbi(PG_CR1, SPI_SCK_PIN),  cbi(PG_CR2, SPI_SCK_PIN)

#define SPI_MOSI_LO		cbi(PA_ODR, SPI_MOSI_PIN)
#define SPI_MOSI_HI		sbi(PA_ODR, SPI_MOSI_PIN)
#define SPI_SCK_LO		cbi(PG_ODR, SPI_SCK_PIN)
#define SPI_SCK_HI		sbi(PG_ODR, SPI_SCK_PIN)
#define SPI_MISO_HI		(PC_IDR & (1 << SPI_MISO_PIN))

#define SPI2_MOSI_LO	cbi(PC_ODR, SPI_MISO_PIN)
#define SPI2_MOSI_HI	sbi(PC_ODR, SPI_MISO_PIN)


///////////////////////////////////////////////////////////
void SPI_init(void);
void SPI_write (unsigned char data);
unsigned char SPI_read (void);

void ENABLE_SPI2 (void);
void DISABLE_SPI2 (void);
void SPI2_write (unsigned char data);
void SPI_burst (void);

#endif

