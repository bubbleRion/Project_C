#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql.h>

void finish_with_error(MYSQL *con) {
    fprintf(stderr, "%s\n", mysql_error(con));
    mysql_close(con);
    exit(1);
}

MYSQL* connect_to_database() {
    MYSQL *con = mysql_init(NULL);
    
    if (con == NULL) {
        fprintf(stderr, "mysql_init() failed\n");
        exit(1);
    }
    
    if (mysql_real_connect(con, "localhost", "bangme", "djwls123", 
            "washer", 0, NULL, 0) == NULL) {
        finish_with_error(con);
    }
    
    return con;
}

void display_all_washers(MYSQL *con) {
    if (mysql_query(con, "SELECT id, waintgnumber, dormitoryroom, date, is_working FROM washer")) {
        finish_with_error(con);
    }
    
    MYSQL_RES *result = mysql_store_result(con);
    
    if (result == NULL) {
        finish_with_error(con);
    }
    
    MYSQL_ROW row;
    
    printf("\n세탁기 상태 목록:\n");
    printf("ID | 대기번호 | 기숙사호실 | 날짜 | 작동상태\n");
    printf("------------------------------------------------\n");
    
    while ((row = mysql_fetch_row(result))) {
        printf("%s | %s | %s | %s | %s\n", 
               row[0], row[1], row[2], row[3], 
               (strcmp(row[4], "1") == 0) ? "작동 중" : "대기 중");
    }
    
    mysql_free_result(result);
}

void change_washer_status(MYSQL *con, int washer_id, int is_working) {
    char query[1024];
    sprintf(query, "UPDATE washer SET is_working = %d WHERE id = %d", is_working, washer_id);
    
    if (mysql_query(con, query)) {
        finish_with_error(con);
    }
    
    printf("세탁기 ID %d의 상태가 %s(으)로 변경되었습니다.\n", 
           washer_id, is_working ? "작동 중" : "대기 중");
}

void add_new_washer(MYSQL *con, const char *waiting_number, const char *dorm_room, const char *date) {
    char query[1024];
    sprintf(query, "INSERT INTO washer(waintgnumber, dormitoryroom, date, is_working) VALUES('%s', '%s', '%s', 0)",
            waiting_number, dorm_room, date);
    
    if (mysql_query(con, query)) {
        finish_with_error(con);
    }
    
    printf("새로운 세탁기가 추가되었습니다. ID: %lu\n", 
           (unsigned long)mysql_insert_id(con));
}

void check_dorm_washers(MYSQL *con, const char *dorm_room) {
    char query[1024];
    sprintf(query, "SELECT id, waintgnumber, dormitoryroom, date, is_working FROM washer WHERE dormitoryroom = '%s'", dorm_room);
    
    if (mysql_query(con, query)) {
        finish_with_error(con);
    }
    
    MYSQL_RES *result = mysql_store_result(con);
    
    if (result == NULL) {
        finish_with_error(con);
    }
    
    MYSQL_ROW row;
    
    printf("\n기숙사 호실 %s의 세탁기 상태:\n", dorm_room);
    printf("ID | 대기번호 | 기숙사호실 | 날짜 | 작동상태\n");
    printf("------------------------------------------------\n");
    
    while ((row = mysql_fetch_row(result))) {
        printf("%s | %s | %s | %s | %s\n", 
               row[0], row[1], row[2], row[3], 
               (strcmp(row[4], "1") == 0) ? "작동 중" : "대기 중");
    }
    
    mysql_free_result(result);
}

int main() {
    MYSQL *con = connect_to_database();
    int choice, washer_id, is_working;
    char waiting_number[50], dorm_room[50], date[50];
    
    while (1) {
        printf("\n세탁기 관리 시스템\n");
        printf("1. 모든 세탁기 상태 보기\n");
        printf("2. 세탁기 작동 시작\n");
        printf("3. 세탁기 작동 중지\n");
        printf("4. 새 세탁기 추가\n");
        printf("5. 특정 기숙사 호실의 세탁기 상태 확인\n");
        printf("0. 종료\n");
        printf("선택: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 0:
                printf("프로그램을 종료합니다.\n");
                mysql_close(con);
                return 0;
            
            case 1:
                display_all_washers(con);
                break;
            
            case 2:
                printf("작동 시작할 세탁기 ID: ");
                scanf("%d", &washer_id);
                change_washer_status(con, washer_id, 1);
                break;
            
            case 3:
                printf("작동 중지할 세탁기 ID: ");
                scanf("%d", &washer_id);
                change_washer_status(con, washer_id, 0);
                break;
            
            case 4:
                printf("대기번호: ");
                scanf("%s", waiting_number);
                printf("기숙사호실: ");
                scanf("%s", dorm_room);
                printf("날짜(YYYY-MM-DD): ");
                scanf("%s", date);
                add_new_washer(con, waiting_number, dorm_room, date);
                break;
            
            case 5:
                printf("확인할 기숙사 호실: ");
                scanf("%s", dorm_room);
                check_dorm_washers(con, dorm_room);
                break;
            
            default:
                printf("잘못된 선택입니다.\n");
                break;
        }
    }
}
