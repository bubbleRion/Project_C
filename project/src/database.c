#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "database.h"

void finish_with_error(MYSQL *con) {
    fprintf(stderr, "%s\n", mysql_error(con));
    mysql_close(con);
    exit(1);
}

MYSQL* connect_to_db() {
    MYSQL *con = mysql_init(NULL);
    
    if (con == NULL) {
        fprintf(stderr, "mysql_init() failed\n");
        exit(1);
    }
    
    if (mysql_real_connect(con, DB_HOST, DB_USER, DB_PASS, 
                           DB_NAME, DB_PORT, NULL, 0) == NULL) {
        finish_with_error(con);
    }
    
    return con;
}

int add_user(MYSQL *con, const char *userid, const char *dormitoryroom, const char *name, const char *password, const char *phonenumber, const char *email) {
    char query[512];
    
    sprintf(query, "INSERT INTO User (userid, dormitoryroom, name, password, phonenumber, email) VALUES ('%s', '%s', '%s', '%s', '%s', '%s')",
            userid, dormitoryroom, name, password, phonenumber, email);
    
    printf("실행할 쿼리: %s\n", query);  // 디버깅용
    
    if (mysql_query(con, query)) {
        fprintf(stderr, "쿼리 실행 오류: %s\n", mysql_error(con));
        return 0;
    }
    
    printf("영향받은 행 수: %lu\n", (unsigned long)mysql_affected_rows(con));  // 디버깅용
    
    return 1;
}

int update_user_by_userid(MYSQL *con, const char *userid, const char *dormitoryroom, const char *name, const char *password, const char *phonenumber, const char *email) {
    char query[512];
    
    sprintf(query, "UPDATE User SET dormitoryroom = '%s', name = '%s', password = '%s', phonenumber = '%s', email = '%s' WHERE userid = '%s'",
            dormitoryroom, name, password, phonenumber, email, userid);
    
    printf("실행할 쿼리: %s\n", query);
    
    if (mysql_query(con, query)) {
        fprintf(stderr, "쿼리 실행 오류: %s\n", mysql_error(con));
        return 0;
    }
    
    printf("영향받은 행 수: %lu\n", (unsigned long)mysql_affected_rows(con));
    
    return mysql_affected_rows(con) > 0;
}

int delete_user(MYSQL *con, int id) {
    char query[512];
    
    sprintf(query, "DELETE FROM User WHERE id = %d", id);
    
    if (mysql_query(con, query)) {
        fprintf(stderr, "%s\n", mysql_error(con));
        return 0;
    }
    
    return 1;
}
int get_user_by_userid(MYSQL *con, const char *userid) {
    char query[512];
    MYSQL_RES *result;
    MYSQL_ROW row;
    
    sprintf(query, "SELECT * FROM User WHERE userid = '%s'", userid);
    
    printf("실행할 쿼리: %s\n", query);  // 디버깅용
    
    if (mysql_query(con, query)) {
        fprintf(stderr, "쿼리 실행 오류: %s\n", mysql_error(con));
        return 0;
    }
    
    result = mysql_store_result(con);
    
    if (result == NULL) {
        fprintf(stderr, "결과 가져오기 오류: %s\n", mysql_error(con));
        return 0;
    }
    
    // 결과 행 수 확인
    int num_rows = mysql_num_rows(result);
    if (num_rows == 0) {
        printf("해당 사용자 ID의 정보가 없습니다.\n");
        mysql_free_result(result);
        return 0;
    }
    
    printf("사용자 정보:\n");
    
    while ((row = mysql_fetch_row(result))) {
        printf("ID: %s\n", row[0]);
        printf("사용자 ID: %s\n", row[1]);
        printf("기숙사 호실: %s\n", row[2]);
        printf("이름: %s\n", row[3]);
        printf("비밀번호: %s\n", row[4]);
        printf("전화번호: %s\n", row[5]);
        printf("이메일: %s\n", row[6]);
    }
    
    mysql_free_result(result);
    
    return 1;
}
int get_user(MYSQL *con, int id) {
    char query[512];
    MYSQL_RES *result;
    MYSQL_ROW row;
    
    sprintf(query, "SELECT * FROM User WHERE id = %d", id);
    
    printf("실행할 쿼리: %s\n", query);  // 디버깅용
    
    if (mysql_query(con, query)) {
        fprintf(stderr, "쿼리 실행 오류: %s\n", mysql_error(con));
        return 0;
    }
    
    result = mysql_store_result(con);
    
    if (result == NULL) {
        fprintf(stderr, "결과 가져오기 오류: %s\n", mysql_error(con));
        return 0;
    }
    
    // 결과 행 수 확인
    int num_rows = mysql_num_rows(result);
    if (num_rows == 0) {
        printf("해당 ID의 사용자 정보가 없습니다.\n");
        mysql_free_result(result);
        return 0;
    }
    
    printf("사용자 정보:\n");
    
    while ((row = mysql_fetch_row(result))) {
        printf("ID: %s\n", row[0]);
        printf("사용자 ID: %s\n", row[1]);
        printf("기숙사 호실: %s\n", row[2]);
        printf("이름: %s\n", row[3]);
        printf("비밀번호: %s\n", row[4]);
        printf("전화번호: %s\n", row[5]);
        printf("이메일: %s\n", row[6]);
    }
    
    mysql_free_result(result);
    
    return 1;
}