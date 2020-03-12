#include "mysqldb.h"
#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>

MYSQL *connect_db() {
    MYSQL *conn = (MYSQL*)malloc(sizeof(MYSQL));
    char *srv = SERVER;
    char *usr = USER;
    char *psswrd = PASSWORD;
    char *db = DATABASE;

    conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, srv,
                            usr, psswrd, db, 0, NULL, 0)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return NULL;
    }
    return conn;
}
