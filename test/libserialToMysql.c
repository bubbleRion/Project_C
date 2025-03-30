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

    const char *server = "192.168.0.18";
    const char *user = "myuser";
    const char *password = "0000";
    const char *database = "project"; // DB받아올 포트로 고치기

    conn = mysql_init(NULL);
    if (conn == NULL)
    {
        fprintf(stderr, "mysql_init() failed\n");
        exit(EXIT_FAILURE);
    }

    if (mysql_real_connect(conn, server, user, password, database, 3306, NULL, 0) == NULL)
    {
        fprintf(stderr, "mysql_real_connect() failed: %s\n", mysql_error(conn));
        exit(EXIT_FAILURE);
    }
} // Mysql 접속용

void autoResetStatus()
{

    char query[] = "UPDATE washer_status SET is_switch = 0 WHERE TIMESTAMPDIFF(MINUTE, date, NOW()) > 1;"; // 30분 후 자동 종료
    if (mysql_query(conn, query))
    {
        fprintf(stderr, "UPDATE failed: %s\n", mysql_error(conn));
    }
} // 50분 후 자동종료

void checkAcceptKey()
{
    char query[] = "SELECT id, is_working FROM washer_status WHERE id IN (1, 2, 3, 4) AND is_working = 1;";
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

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)))
    {
        int id = atoi(row[0]);
        int is_working = atoi(row[1]);

        // printf("ID: %d, is_working: %d\n", id, is_working); // 디버깅용 출력

        // ATmega128로 전송
        if (id == 1)
        {
            sp_blocking_write(port, "1", 1, 1000);
            // printf("인터럽트 1 활성화");
        }
        if (id == 2)
        {
            sp_blocking_write(port, "2", 1, 1000);
            // printf("인터럽트 2 활성화");
        }
        if (id == 3)
        {
            sp_blocking_write(port, "3", 1, 1000);
            // printf("인터럽트 3 활성화");
        }
        if (id == 4)
        {
            sp_blocking_write(port, "4", 1, 1000);
            // printf("인터럽트 4 활성화");
        }
    }

    // MYSQL_ROW row = mysql_fetch_row(result);
    // if (row)
    // {
    //     int is_working = atoi(row[0]); // 받아온 값이 1인지 0인지 확인
    //     // printf("%d 보냄", is_working); //디버그용
    //     if (is_working == 1)
    //     {
    //         sp_blocking_write(port, "1", 1, 1000); // ATmega128에 "1" 전송
    //         // result2 = sp_blocking_write(port, "1", 1, 1000);
    //         // if (result2 != SP_OK)
    //         // {
    //         //     printf("Failed");
    //         // }
    //         // printf("인터럽트 활성화");
    //     }
    //     else
    //     {
    //         sp_blocking_write(port, "0", 1, 1000); // ATmega128에 "0" 전송

    //         // printf("인터럽트 비활성화");
    //     }
    // }

    mysql_free_result(result);
} // Mysql에서 인터럽트 가능 여부 판단

void updateDatabase(const char *message)
{

    char query[256];
    int machine_id = atoi(&message[0]); // 예: '1' -> machine_id = 1
    // printf("updateDBTEST"); //디버그
    // 먼저 DB에서 해당 machine_id와 일치하는 id가 있는지 확인
    snprintf(query, sizeof(query),
             "UPDATE washer_status SET date = CURRENT_TIMESTAMP WHERE id = %d;", machine_id);
    //  "UPDATE washer_status SET date = CURRENT_TIMESTAMP WHERE id = %d;", machine_id);

    if (mysql_query(conn, query))
    {
        fprintf(stderr, "UPDATE failed: %s\n", mysql_error(conn));
    }
    else
    {
        printf("Time updated successfully for Machine ID %d\n", machine_id);
    }
    snprintf(query, sizeof(query),
             "SELECT id FROM washer_status WHERE id = %d;", machine_id);

    if (mysql_query(conn, query))
    {
        fprintf(stderr, "SELECT failed: %s\n", mysql_error(conn));
        return;
    }

    MYSQL_RES *result = mysql_store_result(conn);
    if (result == NULL)
    {
        fprintf(stderr, "mysql_store_result() failed\n");
        return;
    }

    MYSQL_ROW row = mysql_fetch_row(result);
    if (row) // id가 존재하면 UPDATE 실행
    {
        snprintf(query, sizeof(query),
                 "UPDATE washer_status SET is_switch = 1 WHERE id = %d;", machine_id);
        if (mysql_query(conn, query))
        {
            fprintf(stderr, "UPDATE failed: %s\n", mysql_error(conn));
        }
        else
        {
            printf("Machine ID %d updated successfully.\n", machine_id);
        }
    }
    else
    {
        printf("Machine ID %d not found in database. No update performed.\n", machine_id);
    }

    mysql_free_result(result);

    // char query[256];
    // int machine_id = atoi(&message[0]); // 'a1' -> 1번 세탁기
    // snprintf(query, sizeof(query),
    //          "UPDATE washer_status SET is_switch = '%d' WHERE phonenumber = '1012345678';", machine_id);
    // if (mysql_query(conn, query))
    // {
    //     fprintf(stderr, "UPDATE failed: %s\n", mysql_error(conn));
    // }
} // 인터럽트 발생 시 DB 수정

// void logAllStatusChanges()
// {
//     char query[] = "SELECT phonenumber, MACHINE_NUMBER, RUNNING_STATUS FROM washer_status;";
//     // 쿼리 실행
//     if (mysql_query(conn, query))
//     {
//         fprintf(stderr, "SELECT failed: %s\n", mysql_error(conn));
//         return;
//     }
//     MYSQL_RES *result = mysql_store_result(conn);
//     if (result == NULL)
//     {
//         fprintf(stderr, "mysql_store_result() failed: %s\n", mysql_error(conn));
//         return;
//     }
//     MYSQL_ROW row;
//     while ((row = mysql_fetch_row(result))) // 모든 행을 순회
//     {
//         const char *phonenumber = row[0]; // 전화번호
//         int machine_id = atoi(row[1]);    // 기기 번호
//         int status = atoi(row[2]);        // 실행 상태
//         // logStatusChange() 호출하여 로그 테이블에 INSERT
//         logStatusChange(phonenumber, machine_id, status);
//     }
//     mysql_free_result(result); // 결과 메모리 해제
// }
// void logStatusChange(const char *phonenumber, int machine_id, int status)
// {
//     char query[512];
//     snprintf(query, sizeof(query),
//              "INSERT INTO status_log (phonenumber, MACHINE_NUMBER, RUNNING_STATUS) VALUES ('%s', %d, %d);",
//              phonenumber, machine_id, status);
//     if (mysql_query(conn, query))
//     {
//         fprintf(stderr, "INSERT log failed: %s\n", mysql_error(conn));
//     }
// } // 기록 생성 및 조회 안쓸수도있음

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
            // logAllStatusChanges();
        }
    }

    mysql_close(conn);
    sp_close(port);
    sp_free_port(port);
    return 0;
}
