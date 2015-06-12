#ifndef MULTIPLY_A_H
#define MULTIPLY_A_H

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "symboltable.h"

uint32_t mul(char *instr, Map *symbol_table, int address);

uint32_t mla(char *instr, Map *symbol_table, int address);

uint32_t multiply(int rd, int rm, int rs, uint32_t machineCode);

#endif
