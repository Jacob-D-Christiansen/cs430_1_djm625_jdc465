CC=gcc
CFLAGS=-Wall
RM=rm -f

all: testPattern

testPattern: ppmrw.c
	$(CC) $(CFLAGS) ppmrw.c -o ppmrw
    
clean:
	$(RM) ppmrw *~
