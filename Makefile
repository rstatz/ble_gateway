CC=gcc
CFLAGS=-lbluetooth -Wall
SRCDIR=./src

client: $(SRCDIR)/client.o
	$(CC) -o client $(SRCDIR)/client.o $(CFLAGS)

lescan: $(SRCDIR)/lescan.o
	$(CC) -o lescan $(SRCDIR)/lescan.o $(CFLAGS)

clean:
	rm $(SRCDIR)/*.o
	rm lescan
