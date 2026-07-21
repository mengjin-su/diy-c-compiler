#ifndef _UART2_
#define _UART2_

void UART2_init(unsigned long freq, unsigned long baudrate);
void UART2_sendChar(char c);
void UART2_sendStr(char *p);
void UART2_sendBytes(char *p, int length);
void UART2_sendNum16(unsigned int n);
void UART2_sendNum32(unsigned long n);

char UART2_readChar(char *p);
int  UART2_rxLength(void);

#endif	// end of _UART2