#ifndef MYSQLDB_H
#define MYSQLDB_H
#include <mysql.h>
#define SERVER "reach-db.mysql.database.azure.com"
#define USER "myadmin@reach-db"
#define PASSWORD "Reach123"
#define DATABASE "reachdb"


MYSQL* connect_db();
int test_query(); 
#endif
