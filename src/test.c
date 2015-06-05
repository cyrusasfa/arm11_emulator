#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "machine.h"
#include "execution_cycle.h"
#include "data_processing.h"

#define MEM_SIZE (64*1024)
#define NUM_REGS (17)
#define REG_SIZE (32)

#define pc  (Machine.registers[15])
#define cpsr (Machine.registers[16])

void initialize_memories(struct machine_state *machine) {
  machine->memory = (int8_t*) malloc(MEM_SIZE * sizeof(int8_t));
  
  //Set all memory locations to 0
  for(int i=0; i < MEM_SIZE; i++) {
    machine->memory[i] = 0; 
  }

  //Set all registers to 0
  for(int i=0; i < NUM_REGS; i++) {
    machine->registers[i] = 0;
  }
}

void load_memory(const char *file_name, struct machine_state *machine) {
  FILE *program;
  program = fopen(file_name, "rb");

  assert(program);
  fread(machine->memory, 1, MEM_SIZE, program);

  fclose(program);
}

void output_machine_state(struct machine_state Machine) {
  printf("Registers:\n");
  for(int i=0; i<13; i++) {
    printf("$%-3d: %10d (%0#10x)\n", i, 
      Machine.registers[i], Machine.registers[i]);
  }
  
  printf("PC  : %10d (%0#10x)\n", pc, pc);
  printf("CPSR: %10d (%0#10x)\n", cpsr, cpsr);

  printf("Non-zero memory:\n");

  for(uint32_t i=0; i< MEM_SIZE; i+=4) {
    uint32_t fetched = 0;
    int j;
    for(j=0; j<3; j++) {
      fetched += Machine.memory[i+j];
      fetched = fetched << 8;
    }
    fetched += Machine.memory[i+j];
    if (fetched != 0) { 
      printf("%0#10x: %0#10x\n", i, fetched);
    }
  }
}

int main(int argc, char **argv) {
  assert(argc == 2);
  const char *file_name = *(argv+1);
  
  struct machine_state Machine;
  struct pipeline Pipeline;
  struct pipeline *pip_ptr = &Pipeline;
  //Set all memory locations and registers to 0
  initialize_memories(&Machine);
  
  //Program load in memory using a  little endian format
  load_memory(file_name, &Machine); 
  
  while (Pipeline.decoded != &halt) {
     (pip_ptr->decoded) (Pipeline.decoded_args, &Machine);
     decode(Pipeline.fetched, &Pipeline, &Machine);
     fetch(pc, &Pipeline, Machine);
     pc += 4;
  }

  free(Machine.memory);
  output_machine_state(Machine);

  return EXIT_SUCCESS;
}
