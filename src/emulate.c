#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

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

char *memory; //Array to represent the memory
int **registers; // represents the registers

void initializeMemories();

int parseInstructions(const char *filename);

char* fetch(int address);

char* decode(const char *fetched);

void execute(const char *decoded);

int main(int argc, char **argv) {
  assert (argc == 2);
  const char *fileName = argv[1]; 
  
  initializeMemories();
  //Set all memory locations and registers to 0 
  parseInstructions(fileName);
  //Parse binary file and upload contents into memory
  
  char *fetched = NULL;
  char *decoded = NULL;
  
  while (strcmp(decoded, "halt") != 0) {
    // execute(decoded);
    // decoded = decode(fetched);
    // fetched = fetch(pc);
    // pc += 4;    
  }
  
  return EXIT_SUCCESS; 
}

void initializeMemories() {
  memory    = (char *) malloc(MEM_SIZE * sizeof(char));
  registers = (int **) malloc(sizeof(int *) * NUM_REGS);
  //Allocate space for 17 reg pointers
  
  for (int i=0; i < MEM_SIZE; i++) {
    memory[i] = 0;
  }
  //Set all memory locations to 0
  
  for (int i=0; i < NUM_REGS; i++) {
    registers[i] = (char *) malloc(sizeof(char) * REG_SIZE); 
  }
  //Allocate 32 "fake" bits per reg
  
  for (int rNum=0; rNum < NUM_REGS; rNum++) {
    for (int bit=0; bit < REG_SIZE; bit++) {
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

char* fetch(int address) {
  char* result;
  memcpy(result, memory[address], 4);
  return result;
}

char* decode(const char *fetched) {
  if (!fetched) {
    return NULL; 
  }   
}

void execute(const char *decoded) {
  if (!decoded) {
    return;
  }
}
