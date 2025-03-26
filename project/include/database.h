#ifndef DATABASE_H
#define DATABASE_H

#include <mysql.h>

#define DB_HOST "localhost"
#define DB_USER "bangme"
#define DB_PASS "djwls123"
#define DB_NAME "project"
#define DB_PORT 3306

void finish_with_error(MYSQL *con);

MYSQL* connect_to_db();

int add_user(MYSQL *con, int id, const char *dormitoryroom, const char *name, const char *password, const char *phonenumber);

int get_user(MYSQL *con, int id);

int update_user(MYSQL *con, int id, const char *dormitoryroom, const char *name, const char *password, const char *phonenumber);

int delete_user(MYSQL *con, int id);

#endif