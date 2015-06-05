#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "symboltable.h"
#include "tables.h"
#include "utility.h"

#define MAX_LENGTH (512)
#define MAX_OPERANDS (3)

struct Instruction {
   _Bool isLabel;
   int numOperands;
   char *label;
   char *mnemonic;
   char *operands[MAX_OPERANDS];
};

void write_to_output(FILE *dst, uint32_t x);

void tokenise_lines(struct Instruction*  parsedinput, Map* table, FILE* src);

int get_num_lines(FILE* src);

uint32_t and(struct Instruction *instruction);

uint32_t eor(struct Instruction *instruction);

uint32_t sub(struct Instruction *instruction);

uint32_t rsb(struct Instruction *instruction);

uint32_t add(struct Instruction *instruction);

uint32_t orr(struct Instruction *instruction);

uint32_t set_compute_result(struct Instruction *instruction, uint32_t machineCode);

uint32_t mov(struct Instruction *instruction);

uint32_t tst(struct Instruction *instruction);

uint32_t teq(struct Instruction *instruction);

uint32_t cmp(struct Instruction *instruction);

uint32_t set_changes_cpsr(struct Instruction *instruction, uint32_t machineCode);

uint32_t data_processing(struct Instruction * instruction, uint32_t machineCode);

uint32_t set_operand2(struct Instruction *instruction, uint32_t machineCode);

int main(int argc, char **argv) {
  assert(argc == 3);
  
  const char *srcname = argv[1];
  const char* dstname = argv[2];
  
  FILE *src;
  FILE *dst;
  
  src = fopen(srcname, "r");
  dst = fopen(dstname, "wb");
  
  assert (src != NULL);
  assert (dst != NULL);
  
  //first pass. break the instructions and create symbol table
  struct Instruction instructions[get_num_lines(src)];
  Map* table;
  init_table(table);
  tokenise_lines(instructions, table, src);
  
  fclose(src);
  fclose(dst);
  
  return EXIT_SUCCESS;
}

//writes an int (instruction) to the binary file;
void write_to_output(FILE *dst, uint32_t x) {
  fwrite(&x, sizeof(x), 1, dst);
}

int get_num_lines(FILE* src) {
  int num = 0;
  char str[MAX_LENGTH];
  while (fgets(str, MAX_LENGTH, src)) {
    num++;
  }
  fseek(src, 0, SEEK_SET);
  return num;
}

//breaks an instruction into it's different parts and creates symbol table
void tokenise_lines(struct Instruction* instructions, Map* table, FILE* src) {
  int numLabels = 0;
  char line[MAX_LENGTH];
  for (int i = 0; fgets(line, MAX_LENGTH, src); i++) {
    instructions[i].numOperands = 0;
    if (strchr(line, ':')) {
      instructions[i].isLabel = true;
      instructions[i].label = strtok(line, ":");
      insert(table, instructions[i].label, (i - numLabels) * 4);
      numLabels++;
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

uint32_t and(struct Instruction *instruction) {
  //sets the bits specific to and
  uint32_t result = 0;
  result = set_field(result, 0, 24, 4); // opcode for and is 0000
  return set_compute_result(instruction, result);
}

uint32_t eor(struct Instruction *instruction) {
  //sets the bits specific to eor
  uint32_t result = 0;
  result = set_field(result, 1, 24, 4); // opcode for eor is 0001
  return set_compute_result(instruction, result);
}

uint32_t sub(struct Instruction *instruction) {
  //sets the bits specific to sub
  uint32_t result = 0;
  result = set_field(result, 2, 24, 4); // opcode for sub is 0010
  return set_compute_result(instruction, result);
}

uint32_t rsb(struct Instruction *instruction) {
  //sets the bits specific to rsb
  uint32_t result = 0;
  result = set_field(result, 3, 24, 4); // opcode for rsb is 0011
  return set_compute_result(instruction, result);
}

uint32_t add(struct Instruction *instruction) {
  //sets the bits specific to add
  uint32_t result = 0;
  result = set_field(result, 4, 24, 4); // opcode for add is 0100
  return set_compute_result(instruction, result);
}

uint32_t orr(struct Instruction *instruction) {
  //sets the bits specific to orr
  uint32_t result = 0;
  result = set_field(result, 12, 24, 4); // opcode for sub is 1100
  return set_compute_result(instruction, result);
}

uint32_t set_compute_result(struct Instruction *instruction, uint32_t machineCode) {
  // sets the bits generic to dataprocessing instructions which compute results;
  machineCode = set_bit(machineCode,20); //all of them update cpsr
  int regnum = look_up(&registers,instruction->operands[0]);
  machineCode = set_field(machineCode, regnum, 19, 4); //Rn register goes here
  regnum = look_up(&registers,instruction->operands[1]);
  machineCode = set_field(machineCode, regnum, 15, 4); // Rd register goes here
  return data_processing(instruction, machineCode);
}

uint32_t mov(struct Instruction *instruction) {
  //sets the bits specific to move
  uint32_t result = 0;
  result = set_field(result, 13, 24, 4); // opcode for mov is 1101
  result = clear_bit(result,20); //mov does not update cpsr
  result = set_field(result, 0, 19, 4); //Rn is ignored by mov
  int regnum = look_up(&registers,instruction->operands[0]);
  result = set_field(result, regnum, 15, 4); // Rd register goes here
  return data_processing(instruction, result);
}

uint32_t tst(struct Instruction *instruction) {
  //sets the bits specific to tst
  uint32_t result = 0;
  result = set_field(result, 8, 24, 4); // opcode for tst is 1000
  return set_changes_cpsr(instruction, result);
}

uint32_t teq(struct Instruction *instruction) {
  //sets the bits specific to teq
  uint32_t result = 0;
  result = set_field(result, 9, 24, 4); // opcode for teq is 1001
  return set_changes_cpsr(instruction, result);
}

uint32_t cmp(struct Instruction *instruction) {
  //sets the bits specific to tst
  uint32_t result = 0;
  result = set_field(result, 10, 24, 4); // opcode for cmp is 1010
  return set_changes_cpsr(instruction, result);
}

uint32_t set_changes_cpsr(struct Instruction *instruction, uint32_t machineCode) {
  // sets the generic bits of dataprocessing instructions that don't compute a
  // result but still update cpsr
  machineCode = set_bit(machineCode, 20); //they all update cpsr
  int regnum = look_up(&registers,instruction->operands[1]);
  machineCode = set_field(machineCode, regnum, 19, 4); //Rn register goes here
  machineCode = set_field(machineCode, 0, 15, 4); //Rd is ignored
  return data_processing(instruction, machineCode);
}

uint32_t data_processing(struct Instruction *instruction, uint32_t machineCode) {
  //sets the bits that are generic in all data processing instructions
  machineCode = set_field(machineCode, 14, 31, 4);// cond is always 1110
  machineCode = set_field(machineCode, 0, 27, 2);// bits 27,26 are always 0
  return set_operand2(instruction, machineCode);// sets operand 2
}

uint32_t set_operand2(struct Instruction *instruction, uint32_t machineCode) {
  //sets data processing operand2 and the I bit
}
