#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "machine.h"
#include "execution_cycle.h"

#define MEM_SIZE (64*1024)
#define NUM_REGS (17)
#define REG_SIZE (32)

#define pc  (mach_ptr->registers[15])

void initialize_memories(struct machine_state *machine) {
  
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

void output_machine_state(struct machine_state *Machine) {
  printf("Registers:\n");
  for(int i=0; i<13; i++) {
    printf("$%-3d: %10d (%0#10x)\n", i, 
      Machine->registers[i], Machine->registers[i]);
  }
  
  printf("PC  : %10d (%0#10x)\n", Machine->registers[15], Machine->registers[15]);
  printf("CPSR: %10d (%0#10x)\n", Machine->registers[16], Machine->registers[16]);

  printf("Non-zero memory:\n");

  for(uint32_t i=0; i< MEM_SIZE; i+=4) {
    uint32_t fetched = 0;
    int j;
    for(j=0; j<3; j++) {
      fetched += Machine->memory[i+j];
      fetched = fetched << 8;
    }
    fetched += Machine->memory[i+j];
    if (fetched != 0) { 
      printf("%0#10x: %0#10x\n", i, fetched);
    }
  }
}

//NULL function so to say
void do_nothing (uint32_t *args, struct machine_state *mach) {
  return;
}

int main(int argc, char **argv) {
  assert(argc == 2);
  const char *file_name = *(argv+1);
  
  struct pipeline *pip_ptr = malloc(sizeof(struct pipeline));
  struct machine_state *mach_ptr = malloc(sizeof(struct machine_state));
  
  //Set all memory locations and registers to 0
  mach_ptr->registers = (uint32_t*) malloc(NUM_REGS * sizeof(uint32_t));  
  mach_ptr->memory = (uint8_t*) malloc(MEM_SIZE * sizeof(uint8_t));
  
  initialize_memories(mach_ptr);
  
  //Program load in memory using a  little endian format
  load_memory(file_name, mach_ptr);
  
  //Initializing the pipeline 
  //pip_ptr->decoded_args = (uint32_t*) malloc(6 * sizeof(uint32_t));
  //pip_ptr->decoded_args = 0;
  pip_ptr->fetched = (uint32_t*) malloc(sizeof(uint32_t));
  pip_ptr->decoded = &do_nothing;
  pip_ptr->halt = 0;
  int cycle = 1;
  do {
    printf("Block here %i \n", cycle);
    if (cycle == 3) {
      printf("%u, args[2]\n", pip_ptr->decoded_args[2]);
    }
    pip_ptr->decoded (pip_ptr->decoded_args, mach_ptr);
    decode(cycle, pip_ptr->fetched, pip_ptr, mach_ptr); 
    if(cycle == 2) {
      printf("%u, before fetch args[2]\n", pip_ptr->decoded_args[2]);
    }
    fetch(pc, pip_ptr, mach_ptr);
    printf("%u fetched\n", *(pip_ptr->fetched));
    if(cycle == 2) {
      printf("%u, after fetched args[2]\n", pip_ptr->decoded_args[2]);
    }
    pc += 4;
    cycle ++;
    output_machine_state(mach_ptr);
  } while (pip_ptr->halt == 0); 
  
  free(mach_ptr->memory);
  free(mach_ptr->registers);
  if (pip_ptr->decoded_args) {
    free(pip_ptr->decoded_args);
  }
  free(pip_ptr->fetched);
  free(pip_ptr);
  free(mach_ptr);
  output_machine_state(mach_ptr);

  return EXIT_SUCCESS;
}
