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

uint32_t and(char *instruction, Map* symbol_table, int address) {
  
  //sets the bits specific to and
  uint32_t result = 0;
  const int and_opcode = 0; // and opcode is 0000

  result = set_field(result, and_opcode, OPCODE_END, OPCODE_LENGTH);
  
  return set_compute_result(instruction, result);
}

uint32_t eor(char *instruction, Map* symbol_table, int address) {
  
  //sets the bits specific to eor
  
  uint32_t result = 0;
  const int eor_opcode = 1; // eor opcode is 0001
  
  result = set_field(result, eor_opcode, OPCODE_END, OPCODE_LENGTH); 
  
  return set_compute_result(instruction, result);
}

uint32_t sub(char *instruction, Map* symbol_table, int address) {
  
  //sets the bits specific to sub
  
  uint32_t result = 0;
  const int sub_opcode = 2; // sub opcode is 0010
  
  result = set_field(result, sub_opcode, OPCODE_END, OPCODE_LENGTH); 
  
  return set_compute_result(instruction, result);
}

uint32_t rsb(char *instruction, Map* symbol_table, int address) {
  
  //sets the bits specific to rsb
  
  uint32_t result = 0;
  const int rsb_opcode = 3; // rsb opcode is 0011
  
  result = set_field(result, rsb_opcode, OPCODE_END, OPCODE_LENGTH); 
  
  return set_compute_result(instruction, result);
}

uint32_t add(char *instruction, Map* symbol_table, int address) {
  
  //sets the bits specific to add
  
  uint32_t result = 0;
  const int add_opcode = 4; // add opcode is 0100
  
  result = set_field(result, add_opcode, OPCODE_END, OPCODE_LENGTH);
  
  return set_compute_result(instruction, result);
}

uint32_t orr(char *instruction, Map* symbol_table, int address) {
  
  //sets the bits specific to orr
  
  uint32_t result = 0;
  const int orr_opcode = 12; // opcode for sub is 1100
  
  result = set_field(result, orr_opcode, OPCODE_END, OPCODE_LENGTH);
  
  return set_compute_result(instruction, result);
}

uint32_t set_compute_result(char *instruction, uint32_t machineCode) {
  
  // sets the bits generic to dataprocessing instructions which compute results;
  
  const int Rd = look_up(&registers, strtok(instruction, ","));
  const int Rn = look_up(&registers, strtok(NULL, ",")); 
  const int bit_S = 20;
  const int rn_end = 19;
  const int rd_end = 15;
  const int r_length = 4;
  
  instruction = strtok(NULL, "\0");
  
  machineCode = clear_bit(machineCode, bit_S); //all of them update cpsr
  machineCode = set_field(machineCode, Rn, rn_end, r_length); //Rn register
  machineCode = set_field(machineCode, Rd, rd_end, r_length); // Rd register 
  
  return data_processing(instruction, machineCode);
}

uint32_t mov(char *instruction, Map* symbol_table, int address) {
  
  //sets the bits specific to move
  
  uint32_t result = 0;
  const int mov_opcode = 13; // mov opcode is 1101
  const int bit_S = 20;
  const int rn_end = 19;
  const int rd_end = 15;

  const int r_length = 4;
  const int Rd = look_up(&registers, strtok(instruction, ",")); instruction = strtok(NULL, "\0");
  result = clear_bit(result, bit_S); //mov does not update cpsr
  result = set_field(result, 0, rn_end, r_length); //Rn is ignored by mov
  result = set_field(result, Rd, rd_end, r_length); // Rd register goes here
  result = set_field(result, mov_opcode, OPCODE_END, OPCODE_LENGTH);

  return data_processing(instruction, result);
}

uint32_t tst(char *instruction, Map* symbol_table, int address) {
  
  //sets the bits specific to tst
  
  uint32_t result = 0;
  const int tst_opcode = 8; // tst opcode is 1000
  
  result = set_field(result, tst_opcode, OPCODE_END, OPCODE_LENGTH); 
  
  return set_changes_cpsr(instruction, result);
}

uint32_t teq(char *instruction, Map* symbol_table, int address) {
  
  //sets the bits specific to teq
  
  uint32_t result = 0;
  const int teq_opcode = 9; // teq opcode is 1001
  
  result = set_field(result, teq_opcode, OPCODE_END, OPCODE_LENGTH); 
  
  return set_changes_cpsr(instruction, result);
}

uint32_t cmp(char *instruction, Map* symbol_table, int address) {
  
  //sets the bits specific to tst
  
  uint32_t result = 0;
  const int cmp_opcode = 10; // cmp opcode is 1010
  
  result = set_field(result, cmp_opcode, OPCODE_END, OPCODE_LENGTH);
  
  return set_changes_cpsr(instruction, result);
}

uint32_t andeq(char *instruction, Map* symbol_table, int address) {
  return 0;
}

uint32_t set_changes_cpsr(char  *instruction, uint32_t machineCode) {
  
  // sets the generic bits of dataprocessing instructions that don't compute a
  // result but still update cpsr
  
  const int Rn = look_up(&registers, strtok(instruction, ",")); 
  const int rn_end = 19;
  const int rd_end = 15;
  const int r_length = 4;

  instruction = strtok(NULL, "\0");
  
  machineCode = set_bit(machineCode, 20); //they all update cpsr
  machineCode = set_field(machineCode, Rn, rn_end, r_length); //Rn register
  machineCode = set_field(machineCode, 0, rd_end, r_length); //Rd is ignored
  
  return data_processing(instruction, machineCode);
}

uint32_t data_processing(char  *instruction, uint32_t machineCode) {
  
  //sets the bits that are generic in all data processing instructions
  
  const int cond = 14; // cond is 1110;

  machineCode = set_field(machineCode, cond, COND_END, COND_LENGTH);
  machineCode = set_field(machineCode, 0, 27, 2);// bits 27,26 are always 0
  
  return set_operand2(instruction, machineCode);// sets operand 2
}

uint32_t set_operand2(char  *instruction, uint32_t machineCode) {
  
  char *operand2 = strtok(instruction, "\n"); 
  const int rotate_end = 11;
  const int rotate_length = 4;
    
  if (*operand2 ==  '#') {
    
    machineCode = set_bit(machineCode, 25); // set I bit (immediate value)
    int value;
    operand2++;
    if (*operand2 == '0') { //value is in hex
      value = (int) strtol(operand2, NULL, 0);
    }
    
    else { // value is decimal
    
      value = (int) strtol(operand2, NULL, 10);
    }
    
    if (value < 256) { //immediate value fits in 8 bits
    
      machineCode = set_field(machineCode, value, 7, 8);//put value in bits 7-0
      machineCode = set_field(machineCode, 0, rotate_end, rotate_length); 
      
    }
    
    else { // value does not fit in 8 bits
    
      int temp;
    
      for (temp = 0; value % 2 != 1; temp++) {
        value >>= 1;
      }
      
      if (temp % 2 != 0) {
        temp--;
        value <<= 1;
      }
      
      int rotation = (INSTRUCTION_LENGTH - temp) / 2;
      machineCode = set_field(machineCode, value, 7, 8);
      machineCode = set_field(machineCode, rotation, rotate_end, rotate_length);
    }
    
  }
  else {// this case is optional shifted register
      
    int reg = look_up(&registers, strtok(operand2, ","));
    machineCode = set_field(machineCode, reg, 3, 4);
    
    operand2 = strtok(NULL, "\n"); 
    
    if (operand2 == NULL) {
        return machineCode;
    }
    char shift[3];
    for (int i = 0; i < 3; i++) {
      shift[i] = *operand2;
      operand2++;
    }
    printf("%s\n", shift);
    int shift_code = look_up(&shift_table, shift);
    machineCode = set_field(machineCode, shift_code, 6, 2);
    
    operand2 = strtok(operand2, "\n");
    
    if (*operand2 ==  '#') {


      int value;
      operand2++;
      
      if (*operand2 == '0') { //value is in hex
        value = (int) strtol(operand2, NULL, 0);
      } 
    
      else { // value is decimal
    
        value = (int) strtol(operand2, NULL, 10);
      }
          machineCode = clear_bit(machineCode, 4);
          machineCode = set_field(machineCode, value, 11, 5);
    }
    
    else {

      reg = look_up(&registers, strtok(operand2, "\n"));
      machineCode = set_bit(machineCode, 4);
      machineCode = clear_bit(machineCode, 7);
      machineCode = set_field(machineCode, reg, 11, 4);
    }
  }

    return machineCode;

}

uint32_t lsl_assemble(char *instruction, Map* symbol_table, int address) {
  
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


