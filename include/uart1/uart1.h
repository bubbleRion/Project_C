#ifndef UART1_H_

#define UART1_H_

void uart1Init(void);
void iart1Transmit(char data);
unsigned uart1Receive(void);
void uart1PrintString(char *str);
void uart1Print1ByteNumber(unsigned char n);

#endif //UART1_H_