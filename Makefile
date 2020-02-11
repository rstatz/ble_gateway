CC=gcc
CFLAGS=-lbluetooth
SRCDIR=./src

lescan: $(SRCDIR)/lescan.o
	$(CC) -o lescan $(SRCDIR)/lescan.o $(CFLAGS)

clean:
	rm $(SRCDIR)/*.o
	rm lescan
