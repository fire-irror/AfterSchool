#include <stdio.h>
#include <mysql/mysql.h> // libmysql:x64-windows
#include "db_config.h" // mysql 계정정보

int main(void) {
    MYSQL mysql;
    mysql_init(&mysql);
    // mysql 연결시도, 실패시 NULL 반환
    MYSQL* connection = mysql_real_connect(&mysql, DB_HOST, DB_USER, DB_PASS, DB_NAME, 3306, NULL, 0);

    if (connection == NULL)
        printf("DB 연결 실패\n");
    else
        printf("DB 연결 성공\n");

    int query_result;
    // INSERT
    char insert_q[50] = "INSERT INTO korea VALUES('TOKYO', 10000)";

    // Query 실행
    query_result = mysql_query(connection, insert_q);
    if (query_result == 0)
        printf("%s 성공\n", insert_q);
    else
        printf("%s 실패\n", insert_q);

    mysql_close(&mysql); // mysql 종료
}