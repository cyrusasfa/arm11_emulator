#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "symboltable.h"
#include "tables.h"
#include "utility.h"
#include "multiply_a.h"

uint32_t mul(char *instr, Map *symbol_table, int address) {
  uint32_t result = 0; 
  const int rd = look_up(&registers, strtok(instr, ","));
  const int rm = look_up(&registers, strtok(NULL, ","));
  const int rs = look_up(&registers, strtok(NULL, "\n"));
  return multiply(rd, rm, rs, result);
}

uint32_t mla(char *instr, Map *symbol_table, int address) {
  uint32_t result = 0;
  const int bit_A = 21;
  const int rn_end = 15; // rn end bit
  const int r_length = 4; // register length 
  const int rd = look_up(&registers, strtok(instr, ","));
  const int rm = look_up(&registers, strtok(NULL, ","));
  const int rs = look_up(&registers, strtok(NULL, ","));
  const int rn = look_up(&registers, strtok(NULL, "\n"));

  result = set_bit(result, bit_A); // set bit A
  result = set_field(result, rn, rn_end, r_length); // set Rn field 
  return multiply(rd, rm, rs, result);
}

uint32_t multiply(int rd, int rm, int rs, uint32_t machineCode) {
  // S-bit already 0 and A-bit will be 0 for "mul"
  const int cond_value = 14;
  const int rd_end = 19; // rd end bit
  const int rs_end = 11; // rs end bit
  const int rm_end = 3; // rm end bit
  const int r_length = 4; // register length 

  // set opcode to 1110
  machineCode = set_field(machineCode, cond_value, COND_END, COND_LENGTH);

  machineCode = set_field(machineCode, rd, rd_end, r_length); // set Rd field
  machineCode = set_field(machineCode, rs, rs_end, r_length); // set Rs field
  machineCode = set_field(machineCode, rm, rm_end, r_length); // set Rm field
  machineCode = set_field(machineCode, 9, 7, 4); // set bit field 4-7 to 1001
  return machineCode;
}
