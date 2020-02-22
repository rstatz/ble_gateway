CC=gcc

CFLAGS=  -lbluetooth 
#CFLAGS= `pkg-config --cflags glib-2.0` -lbluetooth 
LDLIBS = `pkg-config --libs glib-2.0`
#-lbluetooth -lglib-2.0 -Wall 
#-I/usr/include/glib-2.0/ 
SRCDIR=.

csrc = $(wildcard src/shared/*.c)\
       $(wildcard *.c)

obj := $(csrc:.c=.o)

LDFLAGS =   -lpng -lz -lm
#-lGL -lglut -lpng -lz -lm

server : $(obj)
	$(CC) $(LDLIBS) -o $@ $^ $(CFLAGS) 
	#$(CC) $(CFLAGS) $(LDLIBS) -o $@ $^ $(LDFLAGS)
