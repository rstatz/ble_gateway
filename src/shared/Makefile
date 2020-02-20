CC=gcc
CFLAGS=-lbluetooth -Wall
SRCDIR=.
OBJS = btgatt-server.o mainloop.o mainloop-notify.o

#lescan: $(SRCDIR)/lescan.o
#	$(CC) -o lescan $(SRCDIR)/lescan.o $(CFLAGS)

server: $(SRCDIR)/$(OBJS)
	$(CC) -o server $(OBJS) $(CFLAGS)

mainloop.o : mainloop.c
	$(CC) mainloop.c -o $(CFLAGS)

mainloop-notify.o : mainloop-notify.c 
	$(CC) mainloop-notify.c -o $(CFLAGS)
clean:
	rm $(SRCDIR)/*.o
	rm lescan
