CC=gcc

CFLAGS=  -lbluetooth 
CFLAGS_GLIB= `pkg-config --cflags glib-2.0` 
LDLIBS = `pkg-config --libs glib-2.0`
#-lbluetooth -lglib-2.0 -Wall 
#-I/usr/include/glib-2.0/ 
SRCDIR=.

csrc = $(wildcard src/shared/*.c)\
       btgatt-server.c\
       uuid.c

csrc_client = $(wildcard src/shared/*.c)\
	      btgatt-client.c\
	      uuid.c


obj := $(csrc:.c=.o)
obj_client := $(csrc_client:.c=.o)

LDFLAGS =   -lpng -lz -lm
#-lGL -lglut -lpng -lz -lm

server : $(obj)
	$(CC) $(LDLIBS) $(CFLAG_GLIB) -o $@ $^ $(CFLAGS) 
	#$(CC) $(CFLAGS) $(LDLIBS) -o $@ $^ $(LDFLAGS)

client : $(obj_client)
	$(CC) $(LDLIBS) -o $@ $^ $(CFLAGS)

clean : 
	rm *.o
