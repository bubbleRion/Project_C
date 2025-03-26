#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libserialport.h>

int main()
{
    struct sp_port *port;
    enum sp_return result;

    // 직렬 포트 열기
    result = sp_get_port_by_name("/dev/ttyUSB0", &port); // /dev/ttyS0는 사용하려는 직렬 포트
    if (result != SP_OK)
    {
        printf("포트를 찾을 수 없습니다: %s\n", sp_last_error_message());
        return 1;
    }

    // 포트 설정
    result = sp_open(port, SP_MODE_READ_WRITE); // 읽기/쓰기 모드로 포트 열기
    if (result != SP_OK)
    {
        printf("포트를 열 수 없습니다: %s\n", sp_last_error_message());
        return 1;
    } // test

    // 직렬 포트 설정 (예: 9600 baud, 8 비트, 짝수 패리티)
    sp_set_baudrate(port, 9600);
    sp_set_bits(port, 8);
    sp_set_parity(port, SP_PARITY_NONE);
    sp_set_stopbits(port, 1);

    // 데이터 쓰기
    const char *message = "Hello, Serial Port!";
    result = sp_nonblocking_write(port, message, strlen(message));
    if (result == -1)
    {
        printf("데이터 쓰기 실패: %s\n", sp_last_error_message());
        sp_close(port);
        return 1;
    }
    printf("메시지를 성공적으로 보냈습니다: %s\n", message);

    // 데이터 읽기
    char buffer[256];
    int bytes_read = sp_nonblocking_read(port, buffer, sizeof(buffer) - 1);
    if (bytes_read > 0)
    {
        buffer[bytes_read] = '\0'; // null-terminate the string
        printf("받은 데이터: %s\n", buffer);
    }
    else if (bytes_read == 0)
    {
        printf("받을 데이터가 없습니다.\n");
    }
    else
    {
        printf("데이터 읽기 실패: %s\n", sp_last_error_message());
    }

    // 포트 닫기
    sp_close(port);
    printf("포트를 닫았습니다.\n");

    return 0;
}
