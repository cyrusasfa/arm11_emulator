#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "symboltable.h"
#include "tables.h"
#include "utility.h"
#include "assemble.h"
#include "data_processing_a.h"
#include "multiply_a.h"
#include "single_data_transfer_a.h"
#include "branch_a.h"


FILE* dst;

int main(int argc, char **argv) {
  assert(argc == 3);
  
  const char *srcname = argv[1];
  const char* dstname = argv[2];
  
  FILE *src;
  
  src = fopen(srcname, "r");
  dst = fopen(dstname, "w+b");
  
  assert (src != NULL);
  assert (dst != NULL);
  
  //first pass. break the instructions and create symbol table

  Map * label_table = create_label_table(src); //create symbol table

  char instruction[MAX_LENGTH];
    
  int emptylines = 0;
  for (int i = 0; fgets(instruction, MAX_LENGTH, src); i++) {
    if (strchr(instruction, ':')) {
     continue;
    }
    else if (* instruction == '\n') {
      emptylines++;
    }
    else {
      tokenise_and_assemble(instruction, label_table, (i + 2 - label_table->size - emptylines) * 4);
    }
  }
  
  free(label_table);    
  
  fclose(src);
  fclose(dst);
  
  return EXIT_SUCCESS;
}

//writes an int (instruction) to the binary file;
void write_to_output( uint32_t x) {
  fwrite(&x, sizeof(x), 1, dst);
}

//breaks an instruction into it's different parts and creates symbol table
void tokenise_and_assemble(char *instruction, Map* table, int address) {
  
  
  uint32_t (*op_ptrs[NUM_MNEMONICS])(char *, Map*, int);
  op_ptrs[0]  = &add; 
  op_ptrs[1]  = &sub; 
  op_ptrs[2]  = &rsb;
  op_ptrs[3]  = &and;
  op_ptrs[4]  = &eor;
  op_ptrs[5]  = &orr;
  op_ptrs[6]  = &mov;
  op_ptrs[7]  = &tst;
  op_ptrs[8]  = &teq;
  op_ptrs[9]  = &cmp;
  op_ptrs[10] = &mul;
  op_ptrs[11] = &mla;
  op_ptrs[12] = &ldr;
  op_ptrs[13] = &str;
  op_ptrs[14] = &beq;
  op_ptrs[15] = &bne;
  op_ptrs[16] = &bge;
  op_ptrs[17] = &blt;
  op_ptrs[18] = &bgt; 
  op_ptrs[19] = &ble;
  op_ptrs[20] = &b;
  op_ptrs[21] = &lsl_assemble;
  op_ptrs[22] = &andeq;
  


  char *mnemonic = strtok(instruction, " ");
  instruction = strtok(NULL, "\0");
  remove_spaces(instruction);
  uint32_t machinecode = op_ptrs[look_up(&mnemonic_table, mnemonic)]
       (instruction, table,address);
  write_to_output(machinecode);
}

Map *create_label_table(FILE *src) {
  
  Map *result = malloc(sizeof(Map));
  init_table(result);
  char line[MAX_LENGTH];   
  int i;
  int emptylines = 0;
  for (i = 0; fgets(line, MAX_LENGTH, src); i++) {
    if(*line == '\n') {
        emptylines++;
    }
    if (strchr(line, ':')) {
     
      insert(result, strtok(line, ":"), (i - result->size - emptylines) * 4);
      
    }

  }
  insert(result, "num_lines", i - emptylines - result->size);
  fseek(src, 0, SEEK_SET);
  return result;
}

