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
	printf("%s\n", srv);
	if (!mysql_real_connect(conn, srv,
				usr, psswrd, db, 0, NULL, 0)) {
		fprintf(stderr, "%s\n", mysql_error(conn));
		return NULL;
	}
	return conn;
}

int test_query() {
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;

	/* Connect to database */
	if (connect_db(conn) != 0)
		exit(1);

	/* send SQL query */
       if (mysql_query(conn, "SHOW TABLES;")) {
       		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
       }	       

       res = mysql_use_result(conn);

       while ((row = mysql_fetch_row(res)) != NULL)
	       printf("%s \n", row[0]);


       /* close connection */
       mysql_free_result(res);
       mysql_close(conn);
}
