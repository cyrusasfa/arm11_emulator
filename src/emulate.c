#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <stdint.h>

#define MEM_SIZE (64*1024)
#define NUM_REGS (17)
#define REG_SIZE (32)

#define r0 (registers[0])
#define r1 (registers[1])
#define r2 (registers[2])
#define r3 (registers[3])
#define r4 (registers[4])
#define r5 (registers[5])
#define r6 (registers[6])
#define r7 (registers[7])
#define r8 (registers[8])
#define r9 (registers[9])
#define r10 (registers[10])
#define r11 (registers[11])
#define r12 (registers[12])
#define r13 (registers[13])
#define r14 (registers[14])
#define pc  (registers[15])
#define cpsr (registers[16])

uint8_t *memory; //Array to represent the memory
uint32_t registers[NUM_REGS]; // represents the registers

void initializeMemories();

int loadMemory(const char *filename);

uint32_t fetch(int address); 

void* decode(uint8_t fetched[4]);

void execute(void *decoded);

int main(int argc, char **argv) {
  assert (argc == 2);
  const char *fileName = argv[1]; 
  
  initializeMemories();
    //Set all memory locations and registers to 0 
  loadMemory(fileName);
    //Parse binary file and upload contents into memory
  
  uint32_t fetched;
  //void *decoded = NULL;
  
  //while () {
    // execute(decoded);
    // decoded = decode(fetched);
    // fetched = fetch(pc);
    // pc += 4;    
  //}

  fetched = fetch(0);
  printf("%u", fetched);
  
  free(memory); 
    
  return EXIT_SUCCESS; 
}

void initializeMemories() {
  memory    = (uint8_t *) malloc(MEM_SIZE * sizeof(uint8_t));
  
  for (int i=0; i < MEM_SIZE; i++) {
    memory[i] = 0;
  }
  //Set all memory locations to 0
  
  for (int i=0; i < NUM_REGS; i++) {
    registers[i] = 0; 
  }
  //Set all registers to 0
}

int loadMemory(const char *fileName) {

  FILE *program;
  program  = fopen(fileName, "rb");
  
  assert (program);
  int ret = fread(memory, 1, MEM_SIZE, program);

  fclose(program);
  return ret;
}

uint32_t fetch(int address) {
  uint32_t ret = 0;
  for(int i=3; i > 0; --i) {
    ret += memory[address+i];
    ret = ret << 8;
  }
  return ret += memory[address];
}

void* decode(uint8_t fetched[4]) {
  if (!fetched) {
    return NULL; 
  }
  return NULL; 
}

void execute(void *decoded) {
  if (!decoded) {
    return;
  }
}
