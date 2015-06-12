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
  
  result = set_field(result, 0, 24, 4); // opcode for and is 0000
  
  return set_compute_result(instruction, result);
}

uint32_t eor(char *instruction, Map* symbol_table, int address) {
  
  //sets the bits specific to eor
  
  uint32_t result = 0;
  
  result = set_field(result, 1, 24, 4); // opcode for eor is 0001
  
  return set_compute_result(instruction, result);
}

uint32_t sub(char *instruction, Map* symbol_table, int address) {
  
  //sets the bits specific to sub
  
  uint32_t result = 0;
  
  result = set_field(result, 2, 24, 4); // opcode for sub is 0010
  
  return set_compute_result(instruction, result);
}

uint32_t rsb(char *instruction, Map* symbol_table, int address) {
  
  //sets the bits specific to rsb
  
  uint32_t result = 0;
  
  result = set_field(result, 3, 24, 4); // opcode for rsb is 0011
  
  return set_compute_result(instruction, result);
}

uint32_t add(char *instruction, Map* symbol_table, int address) {
  
  //sets the bits specific to add
  
  uint32_t result = 0;
  
  result = set_field(result, 4, 24, 4); // opcode for add is 0100
  
  return set_compute_result(instruction, result);
}

uint32_t orr(char *instruction, Map* symbol_table, int address) {
  
  //sets the bits specific to orr
  
  uint32_t result = 0;
  
  result = set_field(result, 12, 24, 4); // opcode for sub is 1100
  
  return set_compute_result(instruction, result);
}

uint32_t set_compute_result(char *instruction, uint32_t machineCode) {
  
  // sets the bits generic to dataprocessing instructions which compute results;
  
  const int Rd = look_up(&registers, strtok(instruction, ","));
  const int Rn = look_up(&registers, strtok(NULL, ",")); 
  
  instruction = strtok(NULL, "\0");
  
  machineCode = clear_bit(machineCode,20); //all of them update cpsr
  machineCode = set_field(machineCode, Rn, 19, 4); //Rn register goes here
  machineCode = set_field(machineCode, Rd, 15, 4); // Rd register goes here
  
  return data_processing(instruction, machineCode);
}

uint32_t mov(char *instruction, Map* symbol_table, int address) {
  
  //sets the bits specific to move
  
  uint32_t result = 0;
  
  const int Rd = look_up(&registers, strtok(instruction, ",")); 
  
  instruction = strtok(NULL, "\0");
  result = set_field(result, 13, 24, 4); // opcode for mov is 1101
  result = clear_bit(result,20); //mov does not update cpsr
  result = set_field(result, 0, 19, 4); //Rn is ignored by mov
  result = set_field(result, Rd, 15, 4); // Rd register goes here
  
  return data_processing(instruction, result);
}

uint32_t tst(char *instruction, Map* symbol_table, int address) {
  
  //sets the bits specific to tst
  
  uint32_t result = 0;
  
  result = set_field(result, 8, 24, 4); // opcode for tst is 1000
  
  return set_changes_cpsr(instruction, result);
}

uint32_t teq(char *instruction, Map* symbol_table, int address) {
  
  //sets the bits specific to teq
  
  uint32_t result = 0;
  
  result = set_field(result, 9, 24, 4); // opcode for teq is 1001
  
  return set_changes_cpsr(instruction, result);
}

uint32_t cmp(char *instruction, Map* symbol_table, int address) {
  
  //sets the bits specific to tst
  
  uint32_t result = 0;
  
  result = set_field(result, 10, 24, 4); // opcode for cmp is 1010
  
  return set_changes_cpsr(instruction, result);
}

uint32_t andeq(char *instruction, Map* symbol_table, int address) {
  return 0;
}

uint32_t set_changes_cpsr(char  *instruction, uint32_t machineCode) {
  
  // sets the generic bits of dataprocessing instructions that don't compute a
  // result but still update cpsr
  
  const int Rn = look_up(&registers, strtok(instruction, ",")); 
  
  instruction = strtok(NULL, "\0");
  
  machineCode = set_bit(machineCode, 20); //they all update cpsr
  machineCode = set_field(machineCode, Rn, 19, 4); //Rn register goes here
  machineCode = set_field(machineCode, 0, 15, 4); //Rd is ignored
  
  return data_processing(instruction, machineCode);
}

uint32_t data_processing(char  *instruction, uint32_t machineCode) {
  
  //sets the bits that are generic in all data processing instructions
  
  machineCode = set_field(machineCode, 14, 31, 4);// cond is always 1110
  machineCode = set_field(machineCode, 0, 27, 2);// bits 27,26 are always 0
  
  return set_operand2(instruction, machineCode);// sets operand 2
}

uint32_t set_operand2(char  *instruction, uint32_t machineCode) {
  
  char *operand2 = strtok(instruction, "\n"); 
    
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
    
      machineCode = set_field(machineCode, value, 7, 8);//put value in bits 7..0
      machineCode = set_field(machineCode, 0, 11, 4); //rotate bits are 0
      
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
      machineCode = set_field(machineCode, rotation, 11, 4);
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

