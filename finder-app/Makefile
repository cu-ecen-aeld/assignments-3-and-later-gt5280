  ifdef CROSS_COMPILE
    CC = aarch64-none-linux-gnu-gcc -g -Wall -c -o writer.o
  else
    CC = gcc -o writer
  endif

#CC=aarch64-none-linux-gnu-gcc -g -Wall -c
CFLAGS=-I.
 
writermake: writer.c
	$(CC) writer.c 
	
.PHONY: clean

clean:
	rm -f *.o *.bak