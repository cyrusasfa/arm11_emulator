#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define MAX_LENGTH (512)
#define MAX_OPERANDS (3)

struct Instruction {
   _Bool isLabel;
   int numOperands;
   char *label;
   char *mnemonic;
   char *operands[MAX_OPERANDS];
};

void write_to_output(FILE *dst, int x);

void tokeniseLine(struct Instruction*  parsedinput, FILE* src);

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
  
  //first pass. break the instructions and create symbol table
  struct Instruction instructions[getnumlines(src)];      
  tokeniseLine(instructions, src);
  
  char line[MAX_LENGTH];
  for (int i = 0; fgets(line, MAX_LENGTH, src); i++) {
    if (!instructions[i].isLabel) {
    
    }
  }
      
  
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

//breaks an instruction into it's different parts and creates symbol table
void tokeniseLine(struct Instruction* instructions, FILE* src) {
  char line[MAX_LENGTH];
  for (int i = 0; fgets(line, MAX_LENGTH, src); i++) {
    instructions[i].numOperands = 0;
    if (strchr(line, ':')) {
      instructions[i].isLabel = true;
      instructions[i].label = strtok(line, ":");
    }
    else {
      instructions[i].isLabel = false;
      instructions[i].mnemonic = strtok(line, " ");
      char* nextOperand = strtok(NULL, ",");
      while(nextOperand!= NULL) {
        instructions[i].operands[instructions[i].numOperands]
         = nextOperand;
        nextOperand = strtok(NULL, ",");
        instructions[i].numOperands ++;
      }
    }
  }
}
