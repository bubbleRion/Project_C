#include "lcd.h"
#include "uart0.h"
#include "at25160.h"

#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint8_t count[4];
volatile int i = 0;
volatile uint8_t washingMachineFlag = 0;
volatile uint8_t startFlag = 0;

ISR(TIMER1_OVF_vect);
ISR(INT4_vect);
ISR(INT5_vect);
ISR(INT6_vect);
ISR(INT7_vect);

void init_timer();
void washingStart();

uint8_t startTimer = 30/4.194304;

int main(void)
{
    uart0Init();
    lcdInit();
    init_timer();

    DDRE = 0x00; // 버튼 입력을 위한 설정 (PE0, PE1, PE2 등)

    // External Interrupt 설정
    EIMSK = _BV(INT4) | _BV(INT5) | _BV(INT6) | _BV(INT7);     // 인터럽트 마스크
    EICRB = _BV(ISC41) | _BV(ISC51) | _BV(ISC61) | _BV(ISC71); // 인터럽트 트리거 설정 (하강 엣지)

    sei(); // 전역 인터럽트 활성화

    DDRC = 0xFF;  // PORTC를 출력용으로 설정 (8비트 LED 제어)
    PORTC = 0x00; // 초기 상태에서 모든 LED 끄기

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
    // 각 타이머 값(count[i])이 타이머 종료 기준에 도달했을 때 작업 수행
    for (int i = 0; i < 4; i++)
    {
        if (count[i] >= startTimer)
        {
            // 타이머 종료 후, 해당 LED를 끄고 완료 메시지 출력
            PORTC &= ~(1 << i); // 해당 LED 끄기
            uart0Transmit('a'); // 완료 메시지 전송
            count[i] = 0;       // 타이머 값 리셋
        }
    }
}

ISR(TIMER1_OVF_vect)
{
    count[0]++;
    count[1]++;
    count[2]++;
    count[3]++;
}

ISR(INT4_vect)
{
    // 버튼 1이 눌리면 PORTC의 비트 0만 켬 (첫 번째 LED)
    washingMachineFlag = 0x01; // 00000001
    PORTC |= washingMachineFlag;
    count[0] = 0;
}

ISR(INT5_vect)
{
    // 버튼 2가 눌리면 PORTC의 비트 1만 켬 (두 번째 LED)
    washingMachineFlag = 0x02; // 00000010
    PORTC |= washingMachineFlag;
    count[1] = 0;
}

ISR(INT6_vect)
{
    // 버튼 3이 눌리면 PORTC의 비트 2만 켬 (세 번째 LED)
    washingMachineFlag = 0x04; // 00000100
    PORTC |= washingMachineFlag;
    count[2] = 0;
}

ISR(INT7_vect)
{
    // 버튼 4가 눌리면 PORTC의 비트 3만 켬 (네 번째 LED)
    washingMachineFlag = 0x08; // 00001000
    PORTC |= washingMachineFlag;
    count[3] = 0;
}
