CC=gcc
CFLAGS= -lmysqlclient -lpthread -lz -lm -lrt -lbluetooth -Wall -I/usr/include/mysql -I.  
CFLAGS_GLIB= `pkg-config --cflags glib-2.0` 
LDLIBS = `pkg-config --libs glib-2.0`

SRCDIR=./src

SERVERDIR=$(SRCDIR)/leserver
CLIENTDIR=$(SRCDIR)/leclient
SCANDIR=$(SRCDIR)/lescan

CSRC_SHARED = $(wildcard $(SRCDIR)/shared/*.c) \
              $(wildcard $(SRCDIR)/azure/*.c)

CSRC_SERVER = $(SERVERDIR)/leserver.c

CSRC_CLIENT = $(CLIENTDIR)/leclient.c

CSRC_SCAN = $(SCANDIR)/lescan.c

OBJ_SHARED := $(CSRC_SHARED:.c=.o)

OBJ_SERVER_ONLY := $(CSRC_SERVER:.c=.o)
OBJ_SERVER := $(OBJ_SHARED) $(OBJ_SERVER_ONLY)

OBJ_CLIENT_ONLY := $(CSRC_CLIENT:.c=.o)
OBJ_CLIENT := $(OBJ_SHARED) $(OBJ_CLIENT_ONLY)

OBJ_SCAN := $(CSRC_SCAN:.c=.o)

LDFLAGS =   -lpng -lz -lm

#------------------------------

all: server client scan

server : $(OBJ_SERVER)
	$(CC) $(LDLIBS) $(CFLAGS_GLIB) -o $@ $^ $(CFLAGS)

client : $(OBJ_CLIENT)
	$(CC) $(LDLIBS) $(CFLAGS_GLIB) -o $@ $^ $(CFLAGS)

scan: $(OBJ_SCAN)
	$(CC) -o $@ $^ $(CFLAGS)

%.o:%.c
	$(CC) -c -o $@ $^ $(LDLIBS) $(CFLAGS) $(CFLAGS_GLIB)

clean:
	rm $(OBJ_CLIENT_ONLY) \
		$(OBJ_SERVER_ONLY) \
		$(OBJ_SCAN) \
		$(OBJ_SHARED) \
		server \
		client \
		scan