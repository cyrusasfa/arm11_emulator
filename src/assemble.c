#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define MAX_LENGTH (512)
struct Tokenised {
   char* label;
   char* opcode;
   char* operands[];
};

void writeToOutput(FILE *dst, int x);

void tokenise(struct Tokenised *parsedinput, FILE* src);

int getnumlines(FILE* src);

int main(int argc, char **argv) {
  assert(argc == 3);
  
  const char *srcname = argv[1];
  const char* dstname = argv[2];
  
  FILE *src;
  FILE * dst;
  
  src = fopen(srcname, "r");
  dst = fopen(dstname, "wb");
  
  assert (src != NULL);
  assert (dst != NULL);
  
  struct Tokenised parsedinput[getnumlines(src)];      
  tokenise(parsedinput, src);
    
      
  
  fclose(src);
  fclose(dst);
  
  return EXIT_SUCCESS;
}

//writes an int (instruction) to the binary file;
void writeToOutput(FILE *dst, int x) {
  fwrite(&x, sizeof(x), 1, dst);
}

int getnumlines(FILE* src) {
  int num = 0;
  char str[MAX_LENGTH];
  while (fgets(str, MAX_LENGTH, src)) {
    num++;
  }
  return num;
}
void tokenise(struct Tokenised *parsedinput, FILE* src) {
  char line[MAX_LENGTH];
  for (int i = 0; fgets(line, MAX_LENGTH, src); i++) {
    parsedinput[i].label = strtok(line, ":");
    printf("%s\n", parsedinput[i].label);
  }
}
