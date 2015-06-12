#ifndef SINGLE_DATA_TRANSFER_A_H
#define SINGLE_DATA_TRANSFER_A_H

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "symboltable.h"

uint32_t ldr(char *instruction, Map* symbol_table, int address);

uint32_t str(char *instruction, Map* symbol_table, int address);

uint32_t single_data_transfer(char *instruction, 
   uint32_t machineCode, Map *table, int address);

uint32_t set_address(char *instruction, 
   uint32_t machineCode, Map *table, int address);
   
#endif
