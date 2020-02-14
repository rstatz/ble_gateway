CC=gcc
CFLAGS = -L/usr/lib/arm-linux-gnueabihf -lmysqlclient -lpthread -lz -lm -lrt -latomic -lssl -lcrypto -ldl
LIBS= -I/usr/include/mysql -I. 
host: mysqldb.c host.c
	$(CC) -o host host.c mysqldb.c $(CFLAGS) $(LIBS)

clean:
	rm host
