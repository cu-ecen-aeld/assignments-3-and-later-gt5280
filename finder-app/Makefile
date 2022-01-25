  ifdef CROSS_COMPILE
    CC = aarch64-none-linux-gnu-gcc -g -Wall -c
  else
    CC = gcc
  endif

#CC=aarch64-none-linux-gnu-gcc -g -Wall -c
CFLAGS=-I.
 
writermake: writer.c
	$(CC) -o writer.o writer.c 
	
.PHONY: clean

clean:
	rm -f *.o *.bak