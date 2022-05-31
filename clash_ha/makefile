CC = gcc
CFLAGS = -std=c11 -pedantic -Wall -D_XOPEN_SOURCE=700
.PHONY: all clean
all: clash
clean:
	rm clash
clash: clash.c --plist.o
	$(CC) $(CFLAGS) -o clash *.o clash.c
--plist.o:
	$(CC) $(CFLAGS) -c -o plist.o plist.c