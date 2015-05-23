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
int registers [17];

int parseInstructions(FILE *program) {

  int ret = fread(memory, 1, MEM_SIZE, program);

  for(int i=0; i < ret; i++) {
    printf("%u\n", memory[i]);
  }

  return ret;
}

int main(int argc, char **argv) {
  printf("%i\n", argc);
  assert (argc == 2);
  const char *fileName = argv[1]; 
  printf("%s\n", fileName);
  
  memory = malloc(MEM_SIZE * sizeof(char));
  
  FILE *program;
  program  = fopen(fileName, "r");
  assert (program);
  parseInstructions(program);
  fclose(program);
  
  free(memory); 
  return EXIT_SUCCESS; 
}
