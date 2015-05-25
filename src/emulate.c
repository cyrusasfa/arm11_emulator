#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#define MEM_SIZE (64*1024)
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

char *memory; //Array to represent the memory
char **registers;

void initializeMemories() {
  memory    = (char *) malloc(MEM_SIZE * sizeof(char));
  registers = (char **) malloc(sizeof(char *) * 17);
  //Allocate space for 17 reg pointers
  
  for (int i=0; i < MEM_SIZE; i++) {
    memory[i] = 0;
  }
  //Set all memory locations to 0
  
  for (int i=0; i < 17; i++) {
    registers[i] = (char *) malloc(sizeof(char) * 32); 
  }
  //Allocate 32 "fake" bits per reg
  
  for (int rNum=0; rNum < 17; rNum++) {
    for (int bit=0; bit < 32; bit++) {
      registers[rNum][bit] = 0;
    }
  }
  //Set all the registers to 0
}

int parseInstructions(const char *fileName) {

  FILE *program;
  program  = fopen(fileName, "rb");
  assert (program);
  int ret = fread(memory, 1, MEM_SIZE, program);

  for(int i=0; i < ret; i++) {
    printf("%u\n", memory[i]);
  }

  fclose(program);
  return ret;
}

int main(int argc, char **argv) {
  assert (argc == 2);
  const char *fileName = argv[1]; 
  
  initializeMemories();
  //Set all memory locations and registers to 0 
  parseInstructions(fileName);
  //Parse binary file and upload contents into memory
  
  //Free up the memory used by the machine state
  for (int rNum=0; rNum < 17; rNum++) {
    free(registers[rNum]); 
  }
  free(registers);
  free(memory);
  
  return EXIT_SUCCESS; 
}
