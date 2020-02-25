CC=gcc

CFLAGS=  -lbluetooth -Wall 
CFLAGS_GLIB= `pkg-config --cflags glib-2.0` 
LDLIBS = `pkg-config --libs glib-2.0`

SRCDIR=./src
LIBDIR=./lib #todo remove

INCLUDES= -I $(LIBDIR) -I .

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
	$(CC) $(LDLIBS) $(CFLAGS_GLIB) -o $@ $^ $(CFLAGS) $(INCLUDES)

client : $(OBJ_CLIENT)
	$(CC) $(LDLIBS) $(CFLAGS_GLIB) -o $@ $^ $(CFLAGS) $(INCLUDES)

scan: $(OBJ_SCAN)
	$(CC) -o $@ $^ $(CFLAGS)

%.o:%.c
	$(CC) -c -o $@ $^ $(LDLIBS) $(CFLAGS) $(CFLAGS_GLIB) $(INCLUDES)

clean:
	rm $(OBJ_CLIENT_ONLY) \
		$(OBJ_SERVER_ONLY) \
		$(OBJ_SCAN) \
		server \
		client \
		scan

deepclean:
	rm $(OBJ_SHARED) \
		$(OBJ_CLIENT_ONLY) \
		$(OBJ_SERVER_ONLY) \
		$(OBJ_SCAN)
