#include <avr/io.h>

#define F_CPU 16000000UL                   // ATmega128의 클럭 속도 (16MHz)
#define BAUD 115200                        // 원하는 통신 속도
#define MYUBRR ((F_CPU / (16 * BAUD)) - 1) // UBRR 계산

void uart0Init(void)
{
    UCSR0A |= 0x00;                                       // 2배속 모드
    UCSR0B = (1 << RXCIE0) | (1 << RXEN0) | (1 << TXEN0); // 0x18;  // 0b00011000 Rx, Tx enable
    UCSR0C = 0x16;                                        // 0b00010110  비동기, no Parity, 1 stop bit

    UBRR0H = (MYUBRR >> 8);
    UBRR0L = MYUBRR; //  9600 bps
}

void uart0Transmit(char data)
{
    while (!(UCSR0A & _BV(UDRE0)))
        ;
    UDR0 = data;
}

unsigned char uart0Receive(void)
{
    while (!(UCSR0A & _BV(RXC0))) // 문자 버퍼에 있으면 루프 탈출
        ;
    return UDR0;
}

void uart0PrintString(char *str)
{
    for (int i = 0; str[i]; ++i)
    {
        uart0Transmit(str[i]);
        if (i > 500)
            break;
    }
}

void uart0Print1ByteNumber(unsigned char n)
{
    uint8_t numString[4] = "0";
    int i, index = 0;
    if (n > 0 && n < 1000)
    {
        for (i = 0; n != 0; i++)
        {
            // n = 123
            numString[i] = n % 10 + '0'; // '3' '2' '1'
            n = n / 10;
        }
        numString[i] = '\0';
        index = i - 1;
    }

    for (i = index; i >= 0; i--)
        uart0Transmit(numString[i]); // "123"
}