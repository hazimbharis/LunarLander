CC = gcc
CFLAGS = -lncurses
SOURCES = lunarlander.c

all: lunar run clean

lunar: 
	$(CC) $(SOURCES) $(CFLAGS) -o lunar

run:
	    ./lunar

clean:
	    rm lunar