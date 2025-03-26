#include <stdio.h>
#include <mysql.h>

#define HOST "localhost"
#define USER "your_username"
#define PASSWORD "your_password"
#define DATABASE "your_database"

int main() {
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    int id;

    conn = mysql_init(NULL);
    if (conn == NULL) {
        printf("MySQL 초기화 실패\n");
        return 1;
    }

    if (mysql_real_connect(conn, HOST, USER, PASSWORD, DATABASE, 0, NULL, 0) == NULL) {
        printf("MySQL 연결 실패\n");
        mysql_close(conn);
        return 1;
    }

    if (mysql_query(conn, "SELECT id, washer1, washer2, washer3, washer4 FROM washer") != 0) {
        printf("쿼리 실행 실패\n");
        mysql_close(conn);
        return 1;
    }

    res = mysql_use_result(conn);
    while ((row = mysql_fetch_row(res)) != NULL) {
        id = atoi(row[0]);
        printf("ID: %d\n", id);

        if (atoi(row[1])) {
            printf("세탁기 1번 작동\n");
        }
        if (atoi(row[2])) {
            printf("세탁기 2번 작동\n");
        }
        if (atoi(row[3])) {
            printf("세탁기 3번 작동\n");
        }
        if (atoi(row[4])) {
            printf("세탁기 4번 작동\n");
        }
    }

    mysql_free_result(res);
    mysql_close(conn);

    return 0;
}