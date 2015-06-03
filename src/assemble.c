#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define MAX_LENGTH (512)
#define MAX_OPERANDS (3)

struct Instruction {
  int numOperands;
  char* label;
  char* opcode;
  char * operands[MAX_OPERANDS];
};

void write_to_output(FILE *dst, int x);

void tokeniseLine(struct Instruction*  parsedinput, FILE* src);
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
  const int linenum = getnumlines(src);
  struct Instruction instructions[linenum];      
  tokeniseLine(instructions, src);
  
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
  fseek(src, 0, SEEK_SET);
  return num;
}

void tokeniseLine(struct Instruction* instructions, FILE* src) {
  char line[MAX_LENGTH];
  for (int i = 0; fgets(line, MAX_LENGTH, src); i++) {
    instructions[i].numOperands = 0;
    if (strchr(line, ':')) {
      instructions[i].label = strtok(line, ":");
      printf("%s\n", instructions[i].label);
    }
    else {
      instructions[i].opcode = strtok(line, " ");
      printf("%s\n", instructions[i].opcode);  
      char* nextOperand = strtok(NULL, ",");
      while(nextOperand!= NULL) {
        instructions[i].operands[instructions[i].numOperands]
         = nextOperand;
        nextOperand = strtok(NULL, ",");
        instructions[i].numOperands ++;
      }
      for (int j = 0; j < instructions[i].numOperands; j++) {
          printf("%s\n", instructions[i].operands[j]);
      }
    }
  }
}

void tokenise(struct Tokenised *parsedinput, FILE* src) {
  char line[MAX_LENGTH];
  for (int i = 0; fgets(line, MAX_LENGTH, src); i++) {
    parsedinput[i].label = strtok(line, ":");
    printf("%s\n", parsedinput[i].label);
  }
}
