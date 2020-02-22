CC=gcc
CFLAGS= `pkg-config --cflags glib-2.0` -lbluetooth 
LDLIBS = `pkg-config --libs glib-2.0`
#-lbluetooth -lglib-2.0 -Wall 
#-I/usr/include/glib-2.0/ 
SRCDIR=.

csrc = $(wildcard src/shared/*.c)\
       $(wildcard *.c)

obj := $(csrc:.c=.o)

LDFLAGS = -lGL -lglut -lpng -lz -lm

server : $(obj)
	$(CC) $(CFLAGS) $(LDLIBS) -o $@ $^ $(LDFLAGS)
