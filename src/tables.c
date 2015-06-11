#include <stdio.h>
#include "tables.h"
#include "symboltable.h"
// File for different constant tables.

char *register_names[16] = {
  "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7",
  "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15"
};

int register_numbers[16] = {
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
};

char *mnemonics_strings[23] = {
   "add", "sub", "rsb", "and", "eor", "orr", "mov", "tst", "teq", "cmp",
  "mul", "mla",
  "ldr", "str",
  "beq", "bne", "bge", "blt", "bgt", "ble", "b",
  "lsl", "andeq"
};

int mnemonic_ints[23] = {
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
  19, 20, 21, 22 
};


char *shift_names[4] = {
  "lsl", "lsr", "asr", "ror"
};

int shift_ints[4] = {
  0, 1, 2, 3
};



// Set table of register name to number mappings
Map registers = {register_names, register_numbers, 16, 16};

// Set table of mnemonic to int mappings
Map mnemonic_table = {mnemonics_strings, mnemonic_ints, 23, 23};

Map shift_table = {shift_names, shift_ints, 4, 4};
