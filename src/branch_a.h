#ifndef BRANCH_A_H
#define BRANCH_A_H

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "symboltable.h"
#include "tables.h"
#include "utility.h"

uint32_t beq(char *instr, Map *symbol_table, int address);
uint32_t bne(char *instr, Map *symbol_table, int address);
uint32_t bge(char *instr, Map *symbol_table, int address);
uint32_t blt(char *instr, Map *symbol_table, int address);
uint32_t bgt(char *instr, Map *symbol_table, int address);
uint32_t ble(char *instr, Map *symbol_table, int address);
uint32_t b(char *instr, Map *symbol_table, int address);

uint32_t branch(char* expr, Map* symbol_table, int address, uint32_t machineCode);

#endif
