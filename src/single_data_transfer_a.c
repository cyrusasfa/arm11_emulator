#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "symboltable.h"
#include "tables.h"
#include "utility.h"
#include "data_processing_a.h"
#include "single_data_transfer_a.h"
#include "assemble.h"


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
        perror("malloc problem in set_address");
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
      printf("pos%d\n", pos);
      fseek(dst, look_up(table, "num_lines") * 4, SEEK_SET);
      int address_val = ftell(dst);
      printf("adress%d\n", address);
      printf("%d\n", address_val);
      write_to_output(value);
      fseek(dst, pos, SEEK_SET);
      int offset = (address_val - (table->size - 2) * 4) - address - PC_DIFF ;
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
