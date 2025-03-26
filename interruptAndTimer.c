// ATmega128 사용 타이머와 인터럽트 입력으로 기기 연결 구현해야함
// 받아온 DB 활용해서 타이머 종료 후 추가 입력 전까지 완료 메시지 계속 전송하도록 구현해야함
// 일정 시간 입력 없을 때 전화번호 등을 다음 이용자에게 전달하도록 구현해야함

#include "lcd.h"
#include "uart0.h"
#include "at25160.h"

#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint8_t count[4];
volatile int i = 0;
volatile uint8_t washingMachineFlag = 0;
volatile startFlag = 0;

ISR(TIMER1_OVF_vect);
ISR(INT4_vect);
ISR(INT5_vect);
void init_timer();
void washingStart();
uint32_t startTimer = 3000 / 1024;

int main(void)
{
    uart0Init();

    lcdInit();

    init_timer();

    DDRE = 0x00; //_BV(PE0) | _BV(PE1) | _BV(PE2) | _BV(PE4) | _BV(PE5); //

    EIMSK = _BV(INT4) | _BV(INT5) | _BV(INT6) | _BV(INT7);
    EICRB = _BV(ISC41) | _BV(ISC51) | _BV(ISC61) | _BV(ISC71);

    sei();

    DDRC = 0x0f;
    PORTC = 0x0F;

    while (1)
    {
        if (washingMachineFlag > 0)
        {
            washingStart();
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

void washingStart()
{
    char check;
    // 각 타이머 값(count[i])이 타이머 종료 기준에 도달했을 때 작업 수행
    for (int i = 0; i < 4; i++)
    {
        if (count[i] >= startTimer)
        {
            // 타이머 종료 후, 해당 LED를 끄고 완료 메시지 출력
            PORTC &= ~(1 << i);  // 해당 LED 끄기
            uart0Transmit('a');  // 완료 메시지 전송
            count[i] = 0;  // 타이머 값 리셋
        }
        else
        {
            // 타이머 값 증가 (각 버튼에 대해 독립적인 타이머 실행)
            PORTC |= (washingMachineFlag & (1 << i));  // 해당하는 LED만 켬
        }
    }
}

void resetTimers()
{
    // 타이머 리셋 (모든 타이머를 0으로 설정)
    for (int i = 0; i < 4; i++)
    {
        count[i] = 0;
    }
}

ISR(TIMER1_OVF_vect)
{
    // 타이머 오버플로우 시, 각 버튼에 대한 타이머 값 증가
    for (int i = 0; i < 4; i++)
    {
        count[i]++;
    }
}

ISR(INT4_vect)
{
    washingMachineFlag = 1;
    startFlag = 1;
}

ISR(INT5_vect)
{
    washingMachineFlag = 2;
    startFlag = 2;
}

ISR(INT6_vect)
{
    washingMachineFlag = 4;
    startFlag = 3;
}

ISR(INT7_vect)
{
    washingMachineFlag = 8;
    startFlag = 4;
}