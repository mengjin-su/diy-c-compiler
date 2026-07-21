#include <stm8s105x6.h>
#include "uart2.h"

static unsigned char txBuf[256];		// must be a byte size!
static unsigned char txHead, txTail;
static unsigned char rxBuf[256];		// must be a byte size!
static unsigned char rxHead, rxTail;

void UART2_init(unsigned long sys_freq, unsigned long baudrate)
{
	// 1. Enable clock gate for UART2 in the Clock Controller
    CLK_PCKENR1 |= (1 << 3); // UART2 is typically assigned to bit 3

    // 2. Clear control registers to default (8 data bits, 1 stop bit, no parity)
    UART2_CR1 = 0x00;
    UART2_CR3 = 0x00;

	unsigned int div = sys_freq/baudrate;
    // 3. Set Baud Rate (Order matters: BRR2 must be written before BRR1)
    UART2_BRR2 = (div & 0x0f) | ((div >> 8) & 0xf0);	// bit0-3 and bit12~15
    UART2_BRR1 = (div >> 4);							// bit4-11

    // 4. Enable Transmitter (TEN) and Receiver (REN) in CR2
    // Bit 3 = TEN, Bit 2 = REN
    UART2_CR2 |= (1 << 3) | (1 << 2);

	txHead = txTail;
	rxHead = rxTail;
    // Enable Receive interrupt (RXNEIE)
//	UART2_CR2 |= 0x20;

	// Enable Transmit Empty interrupt (TIEN)
//	UART2_CR2 |= 0x80;

	UART2_CR2bits->TIEN = 0;	// disable tx
	UART2_CR2bits->RIEN = 1;	// enable rx
}

interrupt uart2_tx_isr(__UART2_TX_CMPL_VECTOR__)
{
	if ( txHead != txTail )
		UART2_DR = txBuf[txHead++];
	else
		UART2_CR2bits->TIEN = 0;
}

interrupt uart2_rx_isr(__UART2_RX_FULL_VECTOR__)
{
	unsigned char tail = rxTail + 1;

	if ( UART2_SRbits->RXNE )
	{
		rxBuf[rxTail] = UART2_DR;
		if ( tail != rxHead ) rxTail = tail;
	}
}

void UART2_sendChar(char c)
{
	unsigned char tail = txTail + 1;
	while ( UART2_CR2bits->TIEN && tail == txHead );	// buffer full, wait...

	txBuf[txTail] = c;
	txTail = tail;

	if ( UART2_CR2bits->TIEN == 0 && txHead != txTail )
	{
		UART2_DR = txBuf[txHead++];
		UART2_CR2bits->TIEN = 1;						// enable tx interrupt.
	}
}

void UART2_sendBytes(char *p, int length)
{
	while ( length-- )
		UART2_sendChar(*p++);
}

void UART2_sendStr(char *p)
{
	while ( *p )
		UART2_sendChar(*p++);
}

void UART2_sendNum16(unsigned int n)
{
	char buf[10], len = 0;

	do {
		char c = n % 10;
		buf[len++] = c + '0';
		n /= 10;
	} while ( n );

	while ( len-- ) UART2_sendChar(buf[len]);
}

void UART2_sendNum32(unsigned long n)
{
	char buf[16], len = 0;

	do {
		char c = n % 10;
		buf[len++] = c + '0';
		n /= 10;
	} while ( n );

	while ( len-- ) UART2_sendChar(buf[len]);
}

char UART2_readChar(char *p)
{
	if ( rxHead == rxTail )	// rx buffer empty
		return 0;

	*p = rxBuf[rxHead++];
	return 1;
}

int UART2_rxLength(void)
{
	unsigned char len = rxTail - rxHead;
	return len;
}