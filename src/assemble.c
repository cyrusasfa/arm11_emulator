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

FILE *dst;

uint32_t lsl(char *instruction, Map* symbol_table, int address);

uint32_t ldr(char *instruction, Map* symbol_table, int address);

uint32_t str(char *instruction, Map* symbol_table, int address);

uint32_t single_data_transfer(char *instruction, uint32_t machineCode, Map *table, int address);

uint32_t set_address(char *instruction, uint32_t machineCode, Map *table, int address);

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

  for (int i = 0; fgets(instruction, MAX_LENGTH, src); i++) {
    if (strchr(instruction, ':')) {
     continue;
    }
   
    else {
      tokenise_and_assemble(instruction, label_table, (i + 2 - label_table->size) * 4);
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
  
  
  uint32_t (*op_ptrs[23])(char *, Map*, int);
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
  op_ptrs[21] = &lsl;
  op_ptrs[22] = &andeq;
  


  char *mnemonic = strtok(instruction, " ");
  instruction = strtok(NULL, "\0");
  remove_spaces(instruction);
  uint32_t machinecode = op_ptrs[look_up(&mnemonic_table, mnemonic)]
       (instruction, table,address);
  write_to_output(machinecode);
}

Map *create_label_table(FILE *src) {
  
  Map * result = malloc(sizeof(Map));
  init_table(result);
  
  char line[MAX_LENGTH];
  int i;
  for (i = 0; fgets(line, MAX_LENGTH, src); i++) {
    
    if (strchr(line, ':')) {
     
      insert(result, strtok(line, ":"), (i - result->size) * 4);
    }
  }
  insert(result, "num_lines", i);
  fseek(src, 0, SEEK_SET);
  return result;
}

uint32_t lsl(char *instruction, Map* symbol_table, int address) {
  
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
  uint32_t result = mov(new, symbol_table, 0);
  free(new);
  return result;
}

uint32_t ldr(char *instruction, Map* symbol_table, int address) {
  //sets the bits specific to ldr
  uint32_t result = 0;
  result = set_bit(result, 20); //the L bit is set
  
  result = single_data_transfer(instruction, result, symbol_table, address);
  return result;
}

uint32_t str(char *instruction, Map* symbol_table, int address) {
  //sets the bits specific to str
  uint32_t result = 0;
  result = clear_bit(result, 20); //the L bit is cleared
  
  result = single_data_transfer(instruction, result, symbol_table, address);
  return result;
}

uint32_t single_data_transfer(char *instruction, uint32_t machineCode, Map *table, int address) {
  //sets the generic bits in all single data transfer instructions
  machineCode = set_field(machineCode, 14, 31, 4); // cond is 1110
  machineCode = set_field(machineCode, 1, 27, 2); // bits 27-26 are 01
  machineCode = set_field(machineCode, 0, 23, 2); // bits 23-22 are 00
  machineCode = set_address(instruction, machineCode, table, address);
  return machineCode;
}

uint32_t set_address(char *instruction, uint32_t machineCode, Map* table, int address) {
    //sets the bits representing address
    char *Rd = strtok(instruction, ","); 
    instruction = strtok(NULL, "\0");
  if (*instruction == '=') { // constant case
    uint32_t value;
    instruction++;
    
    if (*instruction == '0') { //value is in hex
      value = (uint32_t) strtol(instruction, NULL, 0);
    } 
    
    else { // value is decimal
      value = (uint32_t) strtol(instruction, NULL, 10);
    }
    
    if (value < 256) {
      char * new = (char *) malloc(MAX_LENGTH * sizeof(char));
      if (new == NULL) {
        perror("malloc problem in lsl");
        exit(EXIT_FAILURE);
      }
      strcpy(new, Rd);
      strcat(new, ",#");
      char val[4];
      sprintf(val, "%d\n", value);
      strcat(new, val);
      uint32_t result = mov(new, NULL, 0);
      return result;

    }
    else {
      int pos = ftell(dst);
      fseek(dst, look_up(table, "num_lines") * 4, SEEK_SET);
      int adress_val = ftell(dst);
      write_to_output(value);
      fseek(dst, pos, SEEK_SET);
      int offset = (adress_val - (table->size - 2) * 4) - address - PC_DIFF ;
      machineCode = set_field(machineCode, 15, 19, 4); //base reg is PC and goes here
      machineCode = set_bit(machineCode, 24); // P bit is set
      machineCode = clear_bit(machineCode, 25); // I bit is cleared
      machineCode = set_bit(machineCode, 23); // U bit is set
      machineCode = set_field(machineCode, offset, 11, 12);
      insert(table, "num_lines", look_up(table, "num_lines") + 1);
    }
  }
  
  else {
    instruction++;
    char *temp = strtok(instruction, "]");
    instruction = strtok(NULL, "\n");
    if (instruction == NULL) { // this is the pre index
      const int Rn = look_up(&registers, strtok(temp, ","));
      machineCode = set_field(machineCode, Rn, 19, 4); //base reg goes here
      machineCode = set_bit(machineCode, 24); // P bit is set
      machineCode = clear_bit(machineCode, 25); // I bit is cleared
      machineCode = set_bit(machineCode, 23); // U bit is set
      int offset = 0;
      temp = strtok(NULL, "\n");
      if (temp != NULL) {
        temp++;
        if (*temp == '-') {
            machineCode = clear_bit(machineCode,23);// U bit is cleared
            temp++;
        }
        else if (*temp == '+') {
            temp++;
        }
        
        if (*temp == '0') { //value is in hex
          offset = (int) strtol(temp, NULL, 0);
        } 
    
        else { // value is decimal
         offset = (int) strtol(temp, NULL, 10);
        }
      }
      machineCode = set_field(machineCode, offset, 11, 12); //offset in 11-0
    }
    else // this is post index
    {
      printf("temp is %s\n", temp);
      printf("instruction is %s\n", instruction);
      const int Rn = look_up(&registers, temp);
      machineCode = set_field(machineCode, Rn, 19, 4); // base reg here
      machineCode = clear_bit(machineCode, 24) ; // P bit is 0
      machineCode = set_bit(machineCode, 23); //U bit is set
      machineCode = clear_bit(machineCode, 25); // I bit is cleared
      int offset;
      instruction++;
        if (*temp == '0') { //value is in hex
          offset = (int) strtol(temp, NULL, 0);
        } 
    
        else { // value is decimal
         offset = (int) strtol(temp, NULL, 10);
        }
    }
  } 
  machineCode = set_field(machineCode, look_up(&registers, Rd), 15, 4);
  return machineCode;
}

uint32_t mul(char *instr, Map *symbol_table, int address) {
  uint32_t result = 0; 
  const int rd = look_up(&registers, strtok(instr, ","));
  const int rm = look_up(&registers, strtok(NULL, ","));
  const int rs = look_up(&registers, strtok(NULL, "\n"));
  return multiply(rd, rm, rs, result);
}

uint32_t mla(char *instr, Map *symbol_table, int address) {
  uint32_t result = 0;
  const int bit_A = 21;
  const int rn_end = 15; // rn end bit
  const int r_length = 4; // register length 
  const int rd = look_up(&registers, strtok(instr, ","));
  const int rm = look_up(&registers, strtok(NULL, ","));
  const int rs = look_up(&registers, strtok(NULL, ","));
  const int rn = look_up(&registers, strtok(NULL, "\n"));

  result = set_bit(result, bit_A); // set bit A
  result = set_field(result, rn, rn_end, r_length); // set Rn field 
  return multiply(rd, rm, rs, result);
}

uint32_t multiply(int rd, int rm, int rs, uint32_t machineCode) {
  // S-bit already 0 and A-bit will be 0 for "mul"
  const int cond_value = 14;
  const int rd_end = 19; // rd end bit
  const int rs_end = 11; // rs end bit
  const int rm_end = 3; // rm end bit
  const int r_length = 4; // register length 

  // set opcode to 1110
  machineCode = set_field(machineCode, cond_value, COND_END, COND_LENGTH);

  machineCode = set_field(machineCode, rd, rd_end, r_length); // set Rd field
  machineCode = set_field(machineCode, rs, rs_end, r_length); // set Rs field
  machineCode = set_field(machineCode, rm, rm_end, r_length); // set Rm field
  machineCode = set_field(machineCode, 9, 7, 4); // set bit field 4-7 to 1001
  return machineCode;
}

uint32_t beq(char *instr, Map *symbol_table, int address) {
  uint32_t result = 0;
  // const int eq = 0;
  // result = set_field(result, eq, COND_END, COND_LENGTH);
  return branch(instr, symbol_table, address, result);
}

uint32_t bne(char *instr, Map *symbol_table, int address) {
  uint32_t result = 0;
  const int ne = 1;
  result = set_field(result, ne, COND_END, COND_LENGTH);
  return branch(instr, symbol_table, address, result);
}

uint32_t bge(char *instr, Map *symbol_table, int address) {
  uint32_t result = 0;
  const int ge = 10;
  result = set_field(result, ge, COND_END, COND_LENGTH);
  return branch(instr, symbol_table, address, result);
}

uint32_t blt(char *instr, Map *symbol_table, int address) {
  uint32_t result = 0;
  const int lt = 11;
  result = set_field(result, lt, COND_END, COND_LENGTH);
  return branch(instr, symbol_table, address, result);
}

uint32_t bgt(char *instr, Map *symbol_table, int address) {
  uint32_t result = 0;
  const int gt = 12;
  result = set_field(result, gt, COND_END, COND_LENGTH);
  return branch(instr, symbol_table, address, result);
}

uint32_t ble(char *instr, Map *symbol_table, int address) {
  uint32_t result = 0;
  const int le = 13;
  result = set_field(result, le, COND_END, COND_LENGTH);
  return branch(instr, symbol_table, address, result);
}

uint32_t b(char *instr, Map *symbol_table, int address) {
  uint32_t result = 0;
  const int al = 14;
  result = set_field(result, al, COND_END, COND_LENGTH);
  return branch(instr, symbol_table, address, result);
}

uint32_t branch(char *instr, Map *symbol_table, int address, uint32_t machineCode) {
  instr = strtok(instr, "\n");
  machineCode = set_field(machineCode, 5, 27, 3); // set bits 25-27 to 101


  // need address of current line

  int offset = look_up(symbol_table, instr) - address - PC_DIFF;
  int signed_offset = offset;
  if (signed_offset < 0) {
   signed_offset += 4;
  } 
  int mask = ((1 << 30) - 1) >> 4;
  signed_offset &= mask;
  signed_offset &= mask;
  // offset <<= 25;
  // offset = set_field(offset, offset, 25, 26);
  signed_offset >>= 2;
  machineCode |= signed_offset;
  return machineCode;
}
