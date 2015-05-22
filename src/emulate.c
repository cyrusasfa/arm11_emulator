#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

void parseInstructions(char *dest, FILE *program) {
  fseek(program, 0, SEEK_END);
  const int programSize = ftell(program);
  fseek(program, 0, SEEK_SET);

  printf("%i\n",programSize);

  dest = malloc( programSize * sizeof(char));

  fread(dest, 4, programSize, program);

  for(int i=0; i < programSize; i++) {
    printf("%u\n", dest[i]);
  }
}

int main(int argc, char **argv) {
  printf("%i\n", argc);
  assert (argc == 2);

  const char *fileName = argv[1]; 
 
  printf("%s\n", fileName);

  FILE *program;
  program  = fopen(fileName, "r");
  assert (program);

  char *opCodes = 0;

  parseInstructions(opCodes, program);
  
  free(opCodes);
  fclose(program);
 
  return EXIT_SUCCESS; 
}
