CC      = gcc
CFLAGS  = -Wall -O2  -g -D_POSIX_SOURCE -D_BSD_SOURCE -std=c99 -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

all: assemble emulate

assemble: assemble.o
	$(CC) assemble.o utility.o tables.o symboltable.o single_data_transfer_a.o data_processing_a.o branch_a.o multiply_a.o -o assemble

assemble.o: symboltable.o branch_a.o multiply_a.o tables.o data_processing_a.o single_data_transfer_a.o utility.o
		$(CC) $(CFLAGS) assemble.c -c -o assemble.o
	
tables.o : symboltable.h symboltable.c tables.c tables.h
	$(CC) $(CFLAGS) tables.c -c -o tables.o

symboltable.o : symboltable.h
	$(CC) $(CFLAGS) symboltable.c -c -o symboltable.o

single_data_transfer_a.o : utility.o tables.o symboltable.o data_processing_a.o single_data_transfer_a.c single_data_transfer_a.h
	$(CC) $(CFLAGS) single_data_transfer_a.c -c -o single_data_transfer_a.o

data_processing_a.o : utility.o symboltable.o tables.o data_processing_a.c data_processing_a.h assemble.h
	$(CC) $(CFLAGS) data_processing_a.c -c -o data_processing_a.o

branch_a.o : utility.o symboltable.o tables.o branch_a.c branch_a.h
	$(CC) $(CFLAGS) branch_a.c -c -o branch_a.o

multiply_a.o : utility.o symboltable.o tables.o multiply_a.c multiply_a.h
	$(CC) $(CFLAGS) multiply_a.c -c -o multiply_a.o


emulate:  emulate.o 
	$(CC) emulate.o execution_cycle.o gpio.o utility.o data_processing.o multiply.o single_data_transfer.o branch.o -o emulate

emulate.o: gpio.o machine.h execution_cycle.o emulate.c
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

branch.o : utility.o branch.c
	$(CC) $(CFLAGS) branch.c -c -o branch.o

gpio.o : utility.o gpio.h gpio.c
	$(CC) $(CFLAGS) gpio.c -c -o gpio.o




clean:
	rm -rf $(wildcard *.o)
	rm -rf assemble
	rm -rf emulate
