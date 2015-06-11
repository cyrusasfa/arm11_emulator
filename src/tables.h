#ifndef TABLES_H
#define TABLES_H

#include "symboltable.h"
#include <stdlib.h>

extern char *register_names[16];
extern int register_numbers[16];
extern char *shift_names[4];

extern Map registers;
extern Map mnemonic_table;
extern Map shift_table;

#endif
