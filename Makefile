CC=gcc
CFLAGS=-lbluetooth -Wall
SRCDIR=./src

OBJDIR=./bin

BLUEZDIR = ./bluez

BLUEZ_INCLUDES = -I $(BLUEZDIR)

BLUEZSRCS = $(BLUEZDIR)/lib/uuid.c \
			$(BLUEZDIR)/src/shared/mainloop.c

BLUEZOBJS = $(BLUEZSRCS:.c=.o)

LECLIENTDIR = $(SRCDIR)/leclient
LESERVERDIR = $(SRCDIR)/leserver
LESCANDIR = $(SRCDIR)/lescan

all: leclient leserver lescan

leclient: $(BLUEZOBJS)
	$(CC) $(CFLAGS) -o leclient $(LECLIENTDIR)/leclient.c $(BLUEZOBJS) $(BLUEZ_INCLUDES)

leserver: $(OBJDIR)/leserver.o
	$(CC) -o lecserver $(LESERVERDIR)/leserver.o $(CFLAGS) $(BLUEZ_INCLUDES)

lescan: $(OBJDIR)/lescan.o
	$(CC) -o lescan $(LESCANDIR)/lescan.o $(CFLAGS)

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $< $(BLUEZ_INCLUDES)

clean:
	rm $(OBJDIR)/*
	rm $(LESCANDIR)/*.o
	rm lescan
	rm $(LESERVERDIR)/*.o
	rm leserver
	rm $(LECLIENTDIR)/*.o
	rm leclient
