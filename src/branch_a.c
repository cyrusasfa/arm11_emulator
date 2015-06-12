#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "symboltable.h"
#include "tables.h"
#include "utility.h"
#include "branch_a.h"

uint32_t beq(char *instr, Map *symbol_table, int address) {
  uint32_t result = 0;
  const int eq = 0;
  result = set_field(result, eq, COND_END, COND_LENGTH); // eq cond is 0
  return branch(instr, symbol_table, address, result);
}

uint32_t bne(char *instr, Map *symbol_table, int address) {
  uint32_t result = 0;
  const int ne = 1;
  result = set_field(result, ne, COND_END, COND_LENGTH);
  return branch(instr, symbol_table, address, result);
}

uint32_t bge(char *instr, Map *symbol_table, int address) {
  uint32_t result = 0;
  const int ge = 10;
  result = set_field(result, ge, COND_END, COND_LENGTH);
  return branch(instr, symbol_table, address, result);
}

uint32_t blt(char *instr, Map *symbol_table, int address) {
  uint32_t result = 0;
  const int lt = 11;
  result = set_field(result, lt, COND_END, COND_LENGTH);
  return branch(instr, symbol_table, address, result);
}

uint32_t bgt(char *instr, Map *symbol_table, int address) {
  uint32_t result = 0;
  const int gt = 12;
  result = set_field(result, gt, COND_END, COND_LENGTH);
  return branch(instr, symbol_table, address, result);
}

uint32_t ble(char *instr, Map *symbol_table, int address) {
  uint32_t result = 0;
  const int le = 13;
  result = set_field(result, le, COND_END, COND_LENGTH);
  return branch(instr, symbol_table, address, result);
}

uint32_t b(char *instr, Map *symbol_table, int address) {
  uint32_t result = 0;
  const int al = 14;
  result = set_field(result, al, COND_END, COND_LENGTH);
  return branch(instr, symbol_table, address, result);
}

uint32_t branch(char *instr, Map *symbol_table, int address, uint32_t machineCode) {
  instr = strtok(instr, "\n");
  const int bits_25_27 = 5; // bits 25-27 are 101
  const int bit_27 = 27;
  const int bits_25_27_lgth = 3;
  machineCode = set_field(machineCode, bits_25_27, bit_27, bits_25_27_lgth); 


  // need address of current line
  int signed_offset = look_up(symbol_table, instr) - address - PC_DIFF;
  if (signed_offset < 0) {
    signed_offset += 4;
  } 
  int mask = ((1 << 30) - 1) >> 4; // mask for extracting 26-bits
  signed_offset &= mask;
  signed_offset >>= 2; // shift offset right two before setting field
  machineCode |= signed_offset; // set first 24 bits to offset
  return machineCode;
}
