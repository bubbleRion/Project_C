#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "database.h"

void input_user_info(MYSQL *con) {
    int id;
    char userid[51];
    char dormitoryroom[51];
    char name[51];
    char password[51];
    char phonenumber[51];
    char email[51];
    char buffer[100];

    printf("\n");
    printf("===== 사용자 정보 입력 =====\n\n");
    
    printf("ID: ");
    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        id = atoi(buffer);
    }
    
    printf("사용자 ID: ");
    if (fgets(userid, sizeof(userid), stdin) != NULL) {
        userid[strcspn(userid, "\n")] = 0;
    }
    
    printf("기숙사 호실: ");
    if (fgets(dormitoryroom, sizeof(dormitoryroom), stdin) != NULL) {
        dormitoryroom[strcspn(dormitoryroom, "\n")] = 0;
    }

    printf("이름: ");
    if (fgets(name, sizeof(name), stdin) != NULL) {
        name[strcspn(name, "\n")] = 0;
    }
    
    printf("비밀번호: ");
    if (fgets(password, sizeof(password), stdin) != NULL) {
        password[strcspn(password, "\n")] = 0;
    }
    
    printf("전화번호: ");
    if (fgets(phonenumber, sizeof(phonenumber), stdin) != NULL) {
        phonenumber[strcspn(phonenumber, "\n")] = 0;
    }
    
    printf("이메일: ");
    if (fgets(email, sizeof(email), stdin) != NULL) {
        email[strcspn(email, "\n")] = 0;
    }
    
    if (add_user(con, id, userid, dormitoryroom, name, password, phonenumber, email)) {
        printf("\n사용자 정보가 성공적으로 추가되었습니다.\n");
    } else {
        printf("\n사용자 정보 추가에 실패했습니다.\n");
    }
}

void search_user_info(MYSQL *con) {
    int id;
    
    printf("조회할 사용자 ID: ");
    scanf("%d", &id);
    
    if (!get_user(con, id)) {
        printf("사용자 정보 조회에 실패했습니다.\n");
    }
}

void update_user_info(MYSQL *con) {
    int id;
    char userid[51];
    char dormitoryroom[51];
    char name[51];
    char password[51];
    char phonenumber[51];
    char email[51];
    
    printf("수정할 사용자 ID: ");
    scanf("%d", &id);
    getchar();
    
    printf("\n새 사용자 ID: ");
    fgets(userid, 50, stdin);
    userid[strcspn(userid, "\n")] = 0;
    
    printf("\n기숙사 호실: ");
    fgets(dormitoryroom, 50, stdin);
    dormitoryroom[strcspn(dormitoryroom, "\n")] = 0;
    
    printf("\n새 이름: ");
    fgets(name, 50, stdin);
    name[strcspn(name, "\n")] = 0;
    
    printf("\n새 비밀번호: ");
    fgets(password, 50, stdin);
    password[strcspn(password, "\n")] = 0;
    
    printf("\n새 전화번호: ");
    fgets(phonenumber, 50, stdin);
    phonenumber[strcspn(phonenumber, "\n")] = 0;
    
    printf("\n새 이메일: ");
    fgets(email, 50, stdin);
    email[strcspn(email, "\n")] = 0;
    
    if (update_user(con, id, userid, dormitoryroom, name, password, phonenumber, email)) {
        printf("사용자 정보가 성공적으로 수정되었습니다.\n");
    } else {
        printf("사용자 정보 수정에 실패했습니다.\n");
    }
}

void delete_user_info(MYSQL *con) {
    int id;
    
    printf("삭제할 사용자 ID: ");
    scanf("%d", &id);
    
    if (delete_user(con, id)) {
        printf("사용자 정보가 성공적으로 삭제되었습니다.\n");
    } else {
        printf("사용자 정보 삭제에 실패했습니다.\n");
    }
}

int main() {
    MYSQL *con = connect_to_db();
    
    if (con == NULL) {
        return 1;
    }
    
    int choice;
    
    while (1) {
        printf("\n메뉴:\n");
        printf("1. 사용자 정보 입력\n");
        printf("2. 사용자 정보 조회\n");
        printf("3. 사용자 정보 수정\n");
        printf("4. 사용자 정보 삭제\n");
        printf("5. 종료\n");
        printf("선택: ");
        scanf("%d", &choice);
        getchar(); // 버퍼 비우기
        
        switch (choice) {
            case 1:
                input_user_info(con);
                break;
            case 2:
                search_user_info(con);
                break;
            case 3:
                update_user_info(con);
                break;
            case 4:
                delete_user_info(con);
                break;
            case 5:
                printf("프로그램을 종료합니다.\n");
                mysql_close(con);
                return 0;
            default:
                printf("잘못된 선택입니다.\n");
                break;
        }
    }
    
    return 0;
}