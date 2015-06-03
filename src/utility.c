#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "utility.h"

//reading start from right to left. begin with index 0
bool read_bit(int number, int index) { 
  return extract_bits(number, index, 1) == 1;
}

// start is the start bit from where we extract the bits
// length is how many bits we want to extract
// instruction is from where we want to extract
unsigned int extract_bits(int number, int start, int length) {       
  if (start > 31 || start < 0) {                           
    printf("%s\n", "take int error");                   
    printf("start = %d, length = %d\n", start, length);
    return 0;
  } else {
      number >>= start;
      int mask = (1 << length) - 1;
      return mask & number;     
  }
}

uint32_t set_bit(int32_t number, int bit) {
  return number |= (1 << bit);
}

uint32_t clear_bit(int32_t number, int bit) {
  return number &= ~(1 << bit);
}

//NULL function so to say
void do_nothing (uint32_t *args, struct machine_state *mach, struct pipeline *pip) {
  return;
}