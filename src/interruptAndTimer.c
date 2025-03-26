// ATmega128 사용 타이머와 인터럽트 입력으로 기기 연결 구현해야함
// 받아온 DB 활용해서 타이머 종료 후 추가 입력 전까지 완료 메시지 계속 전송하도록 구현해야함
// 일정 시간 입력 없을 때 전화번호 등을 다음 이용자에게 전달하도록 구현해야함

#include "lcd.h"
#include "uart0.h"
#include "at25160.h"

#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint8_t count;

volatile uint8_t startFlag = 0;
volatile uint8_t endFlag = 0;

ISR(TIMER1_OVF_vect);
ISR(INT4_vect);
ISR(INT5_vect);

int main(void)
{
    uart0Init();

    lcdInit();

    init_timer();
    uint32_t startTimer = 3000 / 1024;

    DDRE = 0x00; //_BV(PE0) | _BV(PE1) | _BV(PE2) | _BV(PE4) | _BV(PE5); //

    EIMSK = _BV(INT4) | _BV(INT5);
    EICRB = _BV(ISC41) | _BV(ISC51);

    sei();

    while (count < startTimer)
    {
        ;
    }

    PORTC = 0x0F;

    while (1)
    {
        if (startFlag == 1)
        {
            count = 0;
            endFlag = 0;
            while (count < startTimer)
            {
                PORTC = 0x03;
            }
            startFlag = 0;
            PORTC = 0x0c;
        }
        if (endFlag == 1)
        {
            startFlag = 0;
            PORTC = 0x00;
        }
    }
    return 0;
}

void init_timer()
{
    // 타이머1을 사용하여 프리스케일러 1024 설정
    // 16MHz / 1024 = 15.625kHz
    TCCR1B |= (1 << CS12) | (1 << CS10); // 프리스케일러 1024 설정
    TCNT1 = 0;                           // 타이머 초기화
    TIMSK |= (1 << TOIE1);               // 타이머1 오버플로 인터럽트 활성화
}

ISR(TIMER1_OVF_vect)
{
    count++;
}

ISR(INT4_vect)
{
    startFlag = 1;
}

ISR(INT5_vect)
{
    endFlag = 1;
}