  ifdef aarch64
    CC = aarch64-none-linux-gnu-gcc -g -Wall -c
  else
    CC = gcc
  endif

#CC=aarch64-none-linux-gnu-gcc -g -Wall -c
CFLAGS=-I.
 
writermake: writer.c
	$(CC) -o writer.o writer.c 