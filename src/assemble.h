#ifndef ASSEMBLE_H
#define ASSEMBLE_H

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "symboltable.h"

extern FILE* dst;

void write_to_output(uint32_t x);

Map *create_label_table(FILE *src);

void tokenise_and_assemble(char *  instruction, Map* table, int address);

#endif  
