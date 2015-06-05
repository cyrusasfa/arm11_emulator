CC      = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_BSD_SOURCE -std=c99 -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

all: assemble emulate

assemble: assemble.o

emulate:  emulate.o 
	$(CC) emulate.o execution_cycle.o utility.o data_processing.o multiply.o single_data_transfer.o branch.o -o emulate

emulate.o: machine.h execution_cycle.o emulate.c
	$(CC) $(CFLAGS) emulate.c -c -o emulate.o  

execution_cycle.o : machine.h execution_cycle.c execution_cycle.h data_processing.o multiply.o branch.o single_data_transfer.o utility.o
	$(CC) $(CFLAGS) execution_cycle.c -c -o execution_cycle.o

utility.o : utility.h utility.c
	$(CC) $(CFLAGS) utility.c -c -o utility.o
  
data_processing.o : machine.h utility.o data_processing.h data_processing.c
	$(CC) $(CFLAGS) data_processing.c -c -o data_processing.o

multiply.o : utility.o machine.h multiply.c multiply.h
	$(CC) $(CFLAGS) multiply.c -c -o multiply.o

single_data_transfer.o : utility.o machine.h single_data_transfer.h single_data_transfer.c
	$(CC) $(CFLAGS) single_data_transfer.c -c -o single_data_transfer.o

branch.o : utility.o utility.c
	$(CC) $(CFLAGS) branch.c -c -o branch.o

gpio.o : utility.o machine.h multiply.c multiply.h
	$(CC) $(CFLAGS) gpio.c -c -o gpio.o

clean:
	rm -rf $(wildcard *.o)
	rm -rf assemble
	rm -rf emulate
