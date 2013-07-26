# Makefile for sct OS/2

CC=gcc
CFLAGS=-Wall

all: sct

sct.o: sct.c
cxio.o: cxio.c

%.o: %.c
	$(CC) $(CFLAGS) -c $<

sct: sct.o cxio.o
	$(CC) $(CFLAGS) sct.o cxio.o -o $@.exe
clean:
	del sct.o sct.exe cxio.o
