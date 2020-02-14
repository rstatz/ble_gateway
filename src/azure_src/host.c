#include "mysqldb.h"
#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <string.h>

/* TBD might need globals for data buffer and bluetooth tag list */

void post_handler(int sig) {
	/* TBD post data */
	printf("post caught signal %d\n", sig);
}
void poll_handler(int sig) {
	printf("poll caught signal %d\n", sig);
	/* TBD bluetooth poll stuff */	
}

int init_timer(int seconds, void (*handler)(int)) {
	struct itimerval *it_val = (struct itimerval*)malloc(sizeof(struct itimerval));
	struct sigaction *sa = (struct sigaction*)malloc(sizeof(struct sigaction));
	
	sa->sa_handler = handler;
	sigemptyset(&sa->sa_mask);
	sa->sa_flags = 0;

	if(sigaction(SIGALRM, sa, NULL) == -1) {
		perror("sigaction");
		exit(1);
	}

	it_val->it_value.tv_sec = seconds;
	it_val->it_value.tv_usec = (seconds * 1000000) % 1000000;
	it_val->it_interval = it_val->it_value;

	if(setitimer(ITIMER_REAL, it_val, NULL) == -1) {
		perror("setitimer");
		exit(1);
	}
	return 0;
}

int main(int argc, char *argv[]) {
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	void (*post)(int) = post_handler;
	void (*poll)(int) = poll_handler;
	int poll_rate = 200;
	int post_rate = 300;
	char realtime = 'F';
	/* TDB 
	 * char * data_buffer[];
	 */

	//test_query();
	printf("here\n");
	/* Connect to database */
	if ((conn = connect_db()) == NULL)
		exit(1);
	
	printf("Connected!\n");
	
	/* pull tag list from azure */
       	if (mysql_query(conn, "SHOW TABLES;")) {
       		fprintf(stderr, "mysql: %s\n", mysql_error(conn));
		exit(1);
       	}
	printf("wtf\n");	
       	res = mysql_use_result(conn);
	/* TBD format tags for use */
       	while ((row = mysql_fetch_row(res)) != NULL)
	       printf("%s \n", row[0]);
	
       	/* pull refresh rate from azure */
	if (mysql_query(conn, "SELECT pollrate FROM config;")) {
		fprintf(stderr, "mysql: %s\n", mysql_error(conn));
		exit(1);
	}

	res = mysql_use_result(conn);
       	/* create poll timer */

	/* TBD how is res stored *
	 * hours, mins, seconds ??
	 */
	
	while((row = mysql_fetch_row(res)) != NULL) {
		poll_rate = atoi(row[0]); //refresh every 5 mins
		printf("poll rate: %d \n", poll_rate);
	}
	
	/* TBD
	 * refresh_rate = row[0]; 
	 * init_timer(refresh_rate,poll);
	 * */
	init_timer(poll_rate, poll);

       	/* check if realtime configured */
	if (mysql_query(conn, "SELECT realtime FROM config;")) {
		fprintf(stderr, "mysql: %s\n", mysql_error(conn));
		exit(1);
	}

	res = mysql_use_result(conn);

	/* TBD get realtime from res */
	while((row = mysql_fetch_row(res)) != NULL) {
 		realtime = *row[0];
		printf("realtime: %c\n", realtime);
	}
	
       	/* create post timer */
	if(realtime == '0') {
		/*TBD get post rate */
			/* pull refresh rate from azure */
		if (mysql_query(conn, "SELECT postrate FROM config;")) {
			fprintf(stderr, "mysql: %s\n", mysql_error(conn));
			exit(1);
		}

		res = mysql_use_result(conn);
       		/* create poll timer */
	
		if((row = mysql_fetch_row(res)) != NULL) {
			post_rate = atoi(row[0]); //refresh every 5 mins
			printf("post rate: %d \n", post_rate);
		}	
		init_timer(post_rate, post);
	}

       	/* close connection */
       	mysql_free_result(res);
       	mysql_close(conn);

	while(1)
		pause();
}
