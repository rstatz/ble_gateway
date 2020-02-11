CC=gcc
CFLAGS=-lbluetooth -Wall
SRCDIR=./src

lescan: $(SRCDIR)/lescan.o
	$(CC) -o lescan $(SRCDIR)/lescan.o $(CFLAGS)

clean:
	rm $(SRCDIR)/*.o
	rm lescan
