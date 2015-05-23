#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

const int memSize = 64 * 1024; 
char *memory; //Array to represent the memory

int parseInstructions(FILE *program) {

  int ret = fread(memory, 1, memSize , program);

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
  
  memory = malloc(memSize * sizeof(char));
  
  FILE *program;
  program  = fopen(fileName, "r");
  assert (program);
  parseInstructions(program);
  fclose(program);
  
  free(memory); 
  return EXIT_SUCCESS; 
}
