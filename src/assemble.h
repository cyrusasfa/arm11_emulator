#ifndef ASSEMBLE_H
#define ASSEMBLE_H

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "symboltable.h"

#define MAX_OPERANDS (3)
#define INSTRUCTION_LENGTH (32)
#define INSTRUCTION_LENGTH (32)
#define COND_LENGTH (4)
#define COND_END (31)
#define PC_DIFF 8

void write_to_output(uint32_t x);

Map *create_label_table(FILE *src);

void tokenise_and_assemble(char *  instruction, Map* table, int address);

uint32_t mul(char *instr, Map *symbol_table, int address);

uint32_t mla(char *instr, Map *symbol_table, int address);

uint32_t multiply(int rd, int rm, int rs, uint32_t machineCode);

uint32_t ldr(char *instr, Map *symbol_table, int address);
uint32_t str(char *instr, Map *symbol_table, int address);

uint32_t beq(char *instr, Map *symbol_table, int address);
uint32_t bne(char *instr, Map *symbol_table, int address);
uint32_t bge(char *instr, Map *symbol_table, int address);
uint32_t blt(char *instr, Map *symbol_table, int address);
uint32_t bgt(char *instr, Map *symbol_table, int address);
uint32_t ble(char *instr, Map *symbol_table, int address);
uint32_t b(char *instr, Map *symbol_table, int address);

uint32_t branch(char* expr, Map* symbol_table, int address, uint32_t machineCode);

#endif  
