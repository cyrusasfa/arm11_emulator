#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "utility.h"
#include "data_processing.h"
#include "machine.h"
#include "branch.h"




void branch (struct machine_state *mach, uint32_t fetched) {
  int offset = extract_bits(fetched, 0, 24);
  offset = offset << 2;
  int32_t move = offset;
  mach -> registers[15] += move;
}