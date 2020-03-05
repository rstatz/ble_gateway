#ifndef MYSQLDB_H
#define MYSQLDB_H
#include <mysql.h>
#define SERVER "reachdb.mysql.database.azure.com"
#define USER "myadmin@reachdb"
#define PASSWORD "Reach123"
#define DATABASE "reachdb"


MYSQL* connect_db();
int test_query();
#endif
