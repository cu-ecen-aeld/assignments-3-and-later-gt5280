  ifdef CROSS_COMPILE
    CC = aarch64-none-linux-gnu-gcc -g -Wall -c -o systemcalls.o
  else
    CC = gcc -o systemcalls
  endif

#CC=aarch64-none-linux-gnu-gcc -g -Wall -c
CFLAGS=-I.
DEPS = systemcalls.h
 
%.c: $(DEPS) 
 
systemcallsmake: systemcalls.c
	$(CC) systemcalls.c 
	
.PHONY: clean

clean:
	rm -f *.o *.bak