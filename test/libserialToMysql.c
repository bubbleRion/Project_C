// 라이브러리
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libserialport.h>
#include <mysql/mysql.h>

// 호출 함수
void errorExit();                                                          // 에러 발생시 종료
void initDatabase();                                                       // DB 접속 함수
void autoResetStatus();                                                    // 50분이 지나면 자동으로 세탁기 종료
void checkAcceptKey();                                                     // 승인이 들어왔을 때만 인터럽트를 발생 가능하게 함
void updateDatabase(const char *message);                                  // 세탁기 스위치 인터럽트를 발생 시켰을 때 세탁기를 작동으로
void logAllStatusChanges();                                                // 세탁기 상태에 대한 로그를 조회하는 함수 안쓸수도 있음
void logStatusChange(const char *phonenumber, int machine_id, int status); // 세탁기 상태에 대한 로그를 생성하는 함수 안쓸수도 있음
// gcc -o serial_to_mysql serial_to_mysql.c -lserialport -lmysqlclient

// 시리얼포트 위치 및 보드레이트 설정
#define SERIAL_PORT "/dev/ttyUSB1"
#define BAUDRATE 115200

MYSQL *conn;            // Mysql 접속에 대한 전역변수
struct sp_port *port;   // 포트에 전달하는 것에 대한 전역 변수
enum sp_return result2; // 디버그용

void errorExit(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
} // 에러 발생시 종료

void initDatabase()
{

    const char *server = "localhost";
    const char *user = "root";
    const char *password = "0000";
    const char *database = "mydb";

    conn = mysql_init(NULL);
    if (conn == NULL)
    {
        fprintf(stderr, "mysql_init() failed\n");
        exit(EXIT_FAILURE);
    }

    if (mysql_real_connect(conn, server, user, password, database, 3360, NULL, 0) == NULL)
    {
        fprintf(stderr, "mysql_real_connect() failed: %s\n", mysql_error(conn));
        exit(EXIT_FAILURE);
    }
} // Mysql 접속용

void autoResetStatus()
{

    char query[] = "UPDATE washing_machine_status SET RUNNING_STATUS = 0 WHERE TIMESTAMPDIFF(MINUTE, last_updated, NOW()) > 1;"; // 30분 후 자동 종료
    if (mysql_query(conn, query))
    {
        fprintf(stderr, "UPDATE failed: %s\n", mysql_error(conn));
    }
} // 50분 후 자동종료

void checkAcceptKey()
{
    char query[] = "SELECT ACCEPT_KEY FROM washing_machine_status WHERE phonenumber = '1012345678';";
    if (mysql_query(conn, query))
    {
        fprintf(stderr, "SELECT failed: %s\n", mysql_error(conn));
        mysql_close(conn);
        return;
    }

    MYSQL_RES *result = mysql_store_result(conn);
    if (result == NULL)
    {
        fprintf(stderr, "mysql_store_result() failed\n");
        mysql_close(conn);
        return;
    }

    MYSQL_ROW row = mysql_fetch_row(result);
    if (row)
    {
        int accept_key = atoi(row[0]); // 받아온 값이 1인지 0인지 확인
        // printf("%d 보냄", accept_key); //디버그용
        if (accept_key == 1)
        {
            sp_blocking_write(port, "1", 1, 1000); // ATmega128에 "1" 전송
            // result2 = sp_blocking_write(port, "1", 1, 1000);
            // if (result2 != SP_OK)
            // {
            //     printf("Failed");
            // }
            // printf("인터럽트 활성화");
        }
        else
        {
            sp_blocking_write(port, "0", 1, 1000); // ATmega128에 "0" 전송

            // printf("인터럽트 비활성화");
        }
    }

    mysql_free_result(result);
} // Mysql에서 인터럽트 가능 여부 판단

void updateDatabase(const char *message)
{

    char query[256];
    int machine_id = atoi(&message[0]); // 'a1' -> 1번 세탁기
    snprintf(query, sizeof(query),
             "UPDATE washing_machine_status SET MACHINE_NUMBER = '%d', RUNNING_STATUS = 1 WHERE phonenumber = '1012345678';", machine_id);
    if (mysql_query(conn, query))
    {
        fprintf(stderr, "UPDATE failed: %s\n", mysql_error(conn));
    }
} // 인터럽트 발생 시 DB 수정

void logAllStatusChanges()
{
    char query[] = "SELECT phonenumber, MACHINE_NUMBER, RUNNING_STATUS FROM washing_machine_status;";

    // 쿼리 실행
    if (mysql_query(conn, query))
    {
        fprintf(stderr, "SELECT failed: %s\n", mysql_error(conn));
        return;
    }

    MYSQL_RES *result = mysql_store_result(conn);
    if (result == NULL)
    {
        fprintf(stderr, "mysql_store_result() failed: %s\n", mysql_error(conn));
        return;
    }

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) // 모든 행을 순회
    {
        const char *phonenumber = row[0]; // 전화번호
        int machine_id = atoi(row[1]);    // 기기 번호
        int status = atoi(row[2]);        // 실행 상태

        // logStatusChange() 호출하여 로그 테이블에 INSERT
        logStatusChange(phonenumber, machine_id, status);
    }

    mysql_free_result(result); // 결과 메모리 해제
}
void logStatusChange(const char *phonenumber, int machine_id, int status)
{
    char query[512];
    snprintf(query, sizeof(query),
             "INSERT INTO status_log (phonenumber, MACHINE_NUMBER, RUNNING_STATUS) VALUES ('%s', %d, %d);",
             phonenumber, machine_id, status);

    if (mysql_query(conn, query))
    {
        fprintf(stderr, "INSERT log failed: %s\n", mysql_error(conn));
    }
} // 기록 생성 및 조회 안쓸수도있음

int main()
{
    enum sp_return error;

    error = sp_get_port_by_name(SERIAL_PORT, &port);
    if (error != SP_OK)
        errorExit("Failed to get port");

    error = sp_open(port, SP_MODE_READ_WRITE);
    if (error != SP_OK)
        errorExit("Failed to open port");

    sp_set_baudrate(port, BAUDRATE);
    sp_set_parity(port, SP_PARITY_NONE);
    sp_set_bits(port, 8);
    sp_set_stopbits(port, 1);

    char buffer[256];
    int bytes_read;
    initDatabase();

    while (1)
    {
        checkAcceptKey();
        memset(buffer, 0, sizeof(buffer));
        bytes_read = sp_blocking_read(port, buffer, sizeof(buffer) - 1, 1000);
        autoResetStatus();

        if (bytes_read > 0)
        {
            buffer[bytes_read] = '\0'; // 문자열 종료
            printf("Received: %s\n", buffer);

            updateDatabase(buffer);
            logAllStatusChanges();
        }
    }

    mysql_close(conn);
    sp_close(port);
    sp_free_port(port);
    return 0;
}
