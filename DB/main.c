//#include <my_global.h>
#include <mysql/mysql.h>
#include <stdio.h>
#include "db_config.h"

#define CHOP(x) x[strlen(x) - 1] = ' '

int main(void)
{
    MYSQL* connection = NULL, conn;
    MYSQL_RES* sql_result;
    MYSQL_ROW   sql_row;
    int query_stat;

    char tel[12];
    char query[255];
       
    mysql_init(&conn);

    connection = mysql_real_connect(&conn, DB_HOST,
        DB_USER, DB_PASS,
        DB_NAME, 3306,
        (char*)NULL, 0);

    if (connection == NULL)
    {
        fprintf(stderr, "Mysql connection error : %s", mysql_error(&conn));
        return 1;
    }

    //query_stat = mysql_query(connection, "select * from login");
    //if (query_stat != 0)
    //{
    //    fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
    //    return 1;
    //}

    //sql_result = mysql_store_result(connection);

    //printf("ID 비밀번호\n");
    //while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
    //{
    //    printf("%s %s \n", sql_row[0], sql_row[1]);
    //}

    //mysql_free_result(sql_result);

    //printf("Insert Value \n");
    //printf("이름 :");
    //fgets(name, 12, stdin);
    //CHOP(name);

    //printf("비밀번호 :");
    //fgets(address, 80, stdin);
    //CHOP(address);
    ////select 종료

    //insert
    sprintf(query, "INSERT into korea VALUES "
        "('%s', %d)",
        "TOKO", 10000);

    query_stat = mysql_query(connection, query);
    //에러 발생
    if (query_stat != 0)
    {
        fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
        return 1;
    }
    else {
        printf("%s문 성공", query);
    }
    //insert

    mysql_close(connection);
}