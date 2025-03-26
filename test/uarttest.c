#include <stdio.h>
#include <stdlib.h>
#include <libserialport.h>

int main()
{
    struct sp_port *port;
    enum sp_return result;

    // 직렬 포트 열기
    result = sp_get_port_by_name("/dev/ttyUSB0", &port); // 포트 이름은 시스템에 맞게 설정
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
    }

    // Baud rate 설정 (9600)
    result = sp_set_baudrate(port, 115200);
    if (result != SP_OK)
    {
        printf("Baud rate 설정 실패: %s\n", sp_last_error_message());
        sp_close(port);
        return 1;
    }

    // 데이터 비트 설정 (8비트)
    result = sp_set_bits(port, 8);
    if (result != SP_OK)
    {
        printf("데이터 비트 설정 실패: %s\n", sp_last_error_message());
        sp_close(port);
        return 1;
    }

    // 패리티 비트 설정 (None)
    result = sp_set_parity(port, SP_PARITY_NONE);
    if (result != SP_OK)
    {
        printf("패리티 설정 실패: %s\n", sp_last_error_message());
        sp_close(port);
        return 1;
    }

    // 정지 비트 설정 (1비트)
    result = sp_set_stopbits(port, 1);
    if (result != SP_OK)
    {
        printf("정지 비트 설정 실패: %s\n", sp_last_error_message());
        sp_close(port);
        return 1;
    }

    // 직렬 포트 설정 완료
    printf("직렬 포트가 설정되었습니다.\n");

    // 데이터 읽기
    char buffer[256];
    int bytes_read;

    while (1)
    {
        bytes_read = sp_blocking_read(port, buffer, sizeof(buffer) - 1, 10000);
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
    }

    // 포트 닫기
    sp_close(port);
    return 0;
}
