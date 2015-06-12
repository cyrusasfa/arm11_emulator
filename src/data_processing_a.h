#ifndef DATA_PROCESSING_A_H
#define DATA_PROCESSING_A_H

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "symboltable.h"

uint32_t and(char *instruction, Map* symbol_table, int address);

uint32_t eor(char *instruction, Map* symbol_table, int address);

uint32_t sub(char *instruction, Map* symbol_table, int address);

uint32_t rsb(char *instruction, Map* symbol_table, int address);

uint32_t add(char *instruction, Map* symbol_table, int address);

uint32_t orr(char *instruction, Map* symbol_table, int address);

uint32_t set_compute_result(char  *instruction, uint32_t machineCode);

uint32_t mov(char *instruction, Map* symbol_table, int address);

uint32_t tst(char *instruction, Map* symbol_table, int address);

uint32_t teq(char *instruction, Map* symbol_table, int address);

uint32_t cmp(char *instruction, Map* symbol_table, int address);

uint32_t andeq(char *instruction, Map* symbol_table, int address);

uint32_t set_changes_cpsr(char  *instruction, uint32_t machineCode);

uint32_t data_processing(char  * instruction, uint32_t machineCode);

uint32_t set_operand2(char  *instruction, uint32_t machineCode);

#endif
