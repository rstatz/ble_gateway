CC=gcc
CFLAGS=-lbluetooth -Wall
SRCDIR=.

server : $(SRCDIR)/btgatt-server.o
	$(CC) -o server $(SRCDIR)/btgatt-server.o $(CFLAGS)

mainloop.o : mainloop.c mainloop.h mainloop-notify.h mainloop-notify.c
    $(CC) $(CFLAGS) -c -o $@ $<

mainloop-notify.o : mainloop-notify.c mainloop.h
    $(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm $(SRCDIR)/*.o
	rm lescan