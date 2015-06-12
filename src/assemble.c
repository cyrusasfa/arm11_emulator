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

uint32_t lsl(char *instruction, Map* symbol_table);

uint32_t ldr(char *instruction, Map* symbol_table);

uint32_t str(char *instruction, Map* symbol_table);

uint32_t single_data_transfer(char *instruction, uint32_t machineCode);

uint32_t set_address(char *instruction, uint32_t machineCode);

int main(int argc, char **argv) {
  assert(argc == 3);
  
  const char *srcname = argv[1];
  const char* dstname = argv[2];
  
  FILE *src;
  FILE *dst;
  
  src = fopen(srcname, "r");
  dst = fopen(dstname, "wb");
  
  assert (src != NULL);
  assert (dst != NULL);
  
  //first pass. break the instructions and create symbol table

  Map * label_table = create_label_table(src); //create symbol table

  char instruction[MAX_LENGTH];

  for (int i = 0; fgets(instruction, MAX_LENGTH, src); i++) {
    if (strchr(instruction, ':')) {
     continue;
    }
   
    else {
      tokenise_and_assemble(instruction, label_table, dst);
    }
  }
  
  free(label_table);    
  
  fclose(src);
  fclose(dst);
  
  return EXIT_SUCCESS;
}

//writes an int (instruction) to the binary file;
void write_to_output(FILE *dst, uint32_t x) {
  fwrite(&x, sizeof(x), 1, dst);
}

//breaks an instruction into it's different parts and creates symbol table
void tokenise_and_assemble(char *instruction, Map* table, FILE *dst) {
  
  
  uint32_t (*op_ptrs[23])(char *, Map*);
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
  //op_ptrs[10] = &mul;
  //op_ptrs[11] = &mla;
  op_ptrs[12] = &ldr;
  op_ptrs[13] = &str;
  //op_ptrs[14] = &beq;
  //op_ptrs[15] = &bne;
  //op_ptrs[16] = &bge;
  //op_ptrs[17] = &blt;
  //op_ptrs[18] = &bgt; 
  //op_ptrs[19] = &ble;
  //op_ptrs[20] = &b;
  op_ptrs[21] = &lsl;
  op_ptrs[22] = &andeq;
  

 
  char *mnemonic = strtok(instruction, " ");
  instruction = strtok(NULL, "\0");
  remove_spaces(instruction);
  
  
  uint32_t machinecode = op_ptrs[look_up(&mnemonic_table, mnemonic)](instruction, table);
  write_to_output(dst, machinecode);
}

Map *create_label_table(FILE *src) {
  
  Map * result = malloc(sizeof(Map));
  init_table(result);
  
  char line[MAX_LENGTH];
  
  for (int i = 0; fgets(line, MAX_LENGTH, src); i++) {
    
    if (strchr(line, ':')) {
     
      insert(result, strtok(line, ":"), (i - result->size) * 4);
    }
  }
  fseek(src, 0, SEEK_SET);
  return result;
}

uint32_t lsl(char *instruction, Map* symbol_table) {
  
  char * new = (char *) malloc(MAX_LENGTH * sizeof(char));
  if (new == NULL) {
      perror("malloc problem in lsl");
      exit(EXIT_FAILURE);
  }
  
  char * Rn = strtok(instruction, ",");
  instruction = strtok(NULL, "\n");
  strcpy(new, Rn);
  strcat(new, ",");
  strcat(new, Rn);
  strcat(new, ",lsl");
  strcat(new, instruction);
  strcat(new, "\n");
  uint32_t result = mov(new, symbol_table);
  free(new);
  return result;
}

uint32_t ldr(char *instruction, Map* symbol_table) {
  //sets the bits specific to ldr
  uint32_t result = 0;
  set_bit(result, 20); //the L bit is set
  
  result = single_data_transfer(instruction, result);
  return result;
}

uint32_t str(char *instruction, Map* symbol_table) {
  //sets the bits specific to str
  uint32_t result = 0;
  clear_bit(result, 20); //the L bit is cleared
  
  result = single_data_transfer(instruction, result);
  return result;
}

uint32_t single_data_transfer(char *instruction, uint32_t machineCode) {
  //sets the generic bits in all single data transfer instructions
  machineCode = set_field(machineCode, 14, 31, 4); // cond is 1110
  machineCode = set_field(machineCode, 1, 27, 2); // bits 27-26 are 01s
  machineCode = set_address(instruction, machineCode);
  return machineCode;
}

uint32_t set_address(char *instruction, uint32_t machineCode) {
  //sets the bits representing address
    char *Rd = strtok(instruction, ","); 
    instruction = strtok(NULL, "\0");
  if (*instruction == '=') { // constant case
    int value;
    instruction++;
    
    if (*instruction == '0') { //value is in hex
      value = (int) strtol(instruction, NULL, 0);
    } 
    
    else { // value is decimal
      value = (int) strtol(instruction, NULL, 10);
    }
    
    if (value < 256) {
      char * new = (char *) malloc(MAX_LENGTH * sizeof(char));
      if (new == NULL) {
        perror("malloc problem in lsl");
        exit(EXIT_FAILURE);
      }
      strcpy(new, Rd);
      strcat(new, ",#");
      char val[3];
      sprintf(val, "%d\n", value);
      strcat(new, val);
      uint32_t result = mov(new, NULL);
      return result;
    }
    else {
    //TODO IMPLEMENT THIS CASE
    }
  }
  
  else {
    isntruction++;
    
}
