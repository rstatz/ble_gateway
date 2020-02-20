#include "mysqldb.h"
#include "timer_setup.h"

/* TBD might need globals for data buffer and bluetooth tag list */
timer_t post_timer;
timer_t poll_timer;

void timer_handler(int sig, siginfo_t *si, void *uc) {
	timer_t *tidp;
	tidp = si->si_value.sival_ptr;
	if(*tidp == poll_timer)
		printf("poll tags\n");
	else if(*tidp == post_timer)
		printf("post data\n");
}

int main(int argc, char *argv[]) {
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	int poll_rate = 200;
	int post_rate = 300;
	char realtime = 'F';
	/* TDB 
	 * char * data_buffer[];
	 */

	/* Connect to database */
	if ((conn = connect_db()) == NULL)
		exit(1);
	
	printf("Connected!\n");
	
	/* pull tag list from azure */
       	if (mysql_query(conn, "SHOW TABLES;")) {
       		fprintf(stderr, "mysql: %s\n", mysql_error(conn));
		exit(1);
       	}
		
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
	
	make_timer(&poll_timer, poll_rate, timer_handler);

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
		make_timer(&post_timer, post_rate, timer_handler);
	}

       	/* close connection */
       	mysql_free_result(res);
       	mysql_close(conn);

	while(1)
		pause();
}
