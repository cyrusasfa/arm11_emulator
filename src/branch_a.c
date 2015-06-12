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
  // const int eq = 0;
  // result = set_field(result, eq, COND_END, COND_LENGTH);
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
  machineCode = set_field(machineCode, 5, 27, 3); // set bits 25-27 to 101


  // need address of current line

  int offset = look_up(symbol_table, instr) - address - PC_DIFF;
  int signed_offset = offset;
  if (signed_offset < 0) {
   signed_offset += 4;
  } 
  int mask = ((1 << 30) - 1) >> 4;
  signed_offset &= mask;
  signed_offset &= mask;
  // offset <<= 25;
  // offset = set_field(offset, offset, 25, 26);
  signed_offset >>= 2;
  machineCode |= signed_offset;
  return machineCode;
}
