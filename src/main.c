#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "uart.h"
#include "wifi.h"

#define SWITCH_PIN PE4

int main(void) {
    // UART 초기화
    uart_init(115200);

    // 스위치 핀 설정
    DDRE &= ~(1 << SWITCH_PIN);
    PORTE |= (1 << SWITCH_PIN);  // 내부 풀업 저항 활성화

    sei();  // 전역 인터럽트 활성화

    while (1) {
        // 스위치 눌림 감지
        if (!(PINE & (1 << SWITCH_PIN))) {
            uart_send_string("1\r\n");
            while (!(PINE & (1 << SWITCH_PIN)));  // 스위치 릴리스 대기
        }
    }

    return 0;
}