SHELL  = /bin/sh
CC     = gcc
CFLAGS = -g
RM     = rm -f

default: all

all: pangolin

pangolin: main.c 
	$(CC) $(CFLAGS) -o pangolin main.c

clean:
	$(RM) pangolin