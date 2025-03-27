#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libserialport.h>
#include <mysql/mysql.h>

// gcc -o serial_to_mysql serial_to_mysql.c -lserialport -lmysqlclient

#define SERIAL_PORT "/dev/ttyUSB0"
#define BAUDRATE 115200

void error_exit(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

void auto_reset_running_status()
{
    MYSQL *conn;
    const char *server = "localhost";
    const char *user = "root";
    const char *password = "0000";
    const char *database = "mydb";

    conn = mysql_init(NULL);
    if (conn == NULL)
    {
        fprintf(stderr, "mysql_init() failed\n");
        return;
    }

    if (mysql_real_connect(conn, server, user, password, database, 3360, NULL, 0) == NULL)
    {
        fprintf(stderr, "mysql_real_connect() failed\n");
        mysql_close(conn);
        return;
    }

    char query[] = "UPDATE washing_machine_status SET RUNNING_STATUS = 0 WHERE TIMESTAMPDIFF(MINUTE, last_updated, NOW()) > 50;"; // 30분 후 자동 종료
    if (mysql_query(conn, query))
    {
        fprintf(stderr, "UPDATE failed: %s\n", mysql_error(conn));
    }

    mysql_close(conn);
}

void check_accept_key_and_notify()
{
    MYSQL *conn;
    const char *server = "localhost";
    const char *user = "root";
    const char *password = "0000";
    const char *database = "mydb";

    conn = mysql_init(NULL);
    if (conn == NULL)
    {
        fprintf(stderr, "mysql_init() failed\n");
        return;
    }

    if (mysql_real_connect(conn, server, user, password, database, 3360, NULL, 0) == NULL)
    {
        fprintf(stderr, "mysql_real_connect() failed\n");
        mysql_close(conn);
        return;
    }

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
        if (accept_key == 1)
        {
            sp_blocking_write(port, "enable", 6, 1000); // ATmega128에 "enable" 전송
        }
        else
        {
            sp_blocking_write(port, "disable", 7, 1000); // ATmega128에 "disable" 전송
        }
    }

    mysql_free_result(result);
    mysql_close(conn);
}

void update_to_database(const char *message)
{
    MYSQL *conn;
    const char *server = "localhost";
    const char *user = "root";
    const char *password = "0000";
    const char *database = "mydb";

    conn = mysql_init(NULL);
    if (conn == NULL)
    {
        fprintf(stderr, "mysql_init() failed\n");
        return;
    }

    if (mysql_real_connect(conn, server, user, password, database, 3360, NULL, 0) == NULL)
    {
        fprintf(stderr, "mysql_real_connect() failed\n");
        mysql_close(conn);
        return;
    }

    char query[256];
    int machine_id = atoi(&message[0]); // 'a1' -> 1번 세탁기
    snprintf(query, sizeof(query),
             "UPDATE washing_machine_status SET MACHINE_NUMBER = '%d', RUNNING_STATUS = 1 WHERE phonenumber = '1012345678';", machine_id);
    if (mysql_query(conn, query))
    {
        fprintf(stderr, "UPDATE failed: %s\n", mysql_error(conn));
    }

    mysql_close(conn);
}

int main()
{
    struct sp_port *port;
    enum sp_return error;

    error = sp_get_port_by_name(SERIAL_PORT, &port);
    if (error != SP_OK)
        error_exit("Failed to get port");

    error = sp_open(port, SP_MODE_READ);
    if (error != SP_OK)
        error_exit("Failed to open port");

    sp_set_baudrate(port, BAUDRATE);
    sp_set_parity(port, SP_PARITY_NONE);
    sp_set_bits(port, 8);
    sp_set_stopbits(port, 1);

    char buffer[256];
    int bytes_read;

    while (1)
    {
        memset(buffer, 0, sizeof(buffer));
        bytes_read = sp_blocking_read(port, buffer, sizeof(buffer) - 1, 1000);

        if (bytes_read > 0)
        {
            buffer[bytes_read] = '\0'; // 문자열 종료
            printf("Received: %s\n", buffer);

            update_to_database(buffer);
            auto_reset_running_status()
        }
    }

    sp_close(port);
    sp_free_port(port);
    return 0;
}
