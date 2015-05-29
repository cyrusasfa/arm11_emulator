#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#define MEM_SIZE (64*1024)
#define NUM_REGS (17)
#define REG_SIZE (32)

#define r0 (registers[0])
#define r1 (registers[1])
#define r2 (registers[2])
#define r3 (registers[3])
#define r4 (registers[4])
#define r5 (registers[5])
#define r6 (registers[6])
#define r7 (registers[7])
#define r8 (registers[8])
#define r9 (registers[9])
#define r10 (registers[10])
#define r11 (registers[11])
#define r12 (registers[12])
#define r13 (registers[13])
#define r14 (registers[14])
#define pc  (registers[15])
#define cpsr (registers[16])

uint8_t *memory; //Array to represent the memory
uint32_t registers[NUM_REGS]; // represents the registers
bool carryout;
uint32_t instlgth = 32;

void initilize_memories();

int load_memory(const char *filename);

uint32_t fetch(int address); 

void decode_execute(uint32_t fetched);

bool equal(void);

bool not_equal(void);

bool greater_equal(void);

bool less(void);

bool greater(void);

bool less_equal(void);

bool always(void);

bool condition(int cond_code);

void  data_processing(int);

bool read_bit(int instruction, int index);

int extract_bits(int instruction, int start, int length);

int rotate_right(int x, int y);

int lsl(int reg, int shiftvalue);

int lsr(int reg, int shiftvalue);

int asr(int reg, int shiftvalue);

int ror(int reg, int shiftvalue);

void branch(uint32_t fetched);

void output_machine_state(void);

// Main function starts here MAIN
int main(int argc, char **argv) {
  assert (argc == 2);
  char name[70] = "../../arm11_1415_testsuite/test_cases/";
  const char *fileName = strcat(name, argv[1]);
  
  initilize_memories();
  //Set all memory locations and registers to 0 
  load_memory(fileName);
  //Parse binary file and upload contents into memory
  
  //uint32_t fetched;
  //void *decoded = NULL;
  
  //while () {
    // execute(decoded);
    // decoded = decode(fetched);
    // fetched = fetch(pc);
    // pc += 4;    
  //}

  free(memory); 
  output_machine_state();
  return EXIT_SUCCESS; 
}

void initilize_memories() {
  memory = (uint8_t *) malloc(MEM_SIZE * sizeof(uint8_t));
  
  for (int i = 0; i < MEM_SIZE; i++) {
    memory[i] = 0;
  }
  //Set all memory locations to 0
  
  for (int i = 0; i < NUM_REGS; i++) {
    registers[i] = 0; 
  }
  //Set all registers to 0
}

int load_memory(const char *fileName) {

  FILE *program;
  program = fopen(fileName, "rb");
  
  assert (program);
  int ret = fread(memory, 1, MEM_SIZE, program);

  fclose(program);
  return ret;
}

uint32_t fetch(int address) {
  uint32_t ret = 0;
  for(int i = 3; i > 0; --i) {
    ret += memory[address+i];
    ret = ret << 8;
  }
  return ret += memory[address];
}

void decode_execute(uint32_t fetched) {
  if (!fetched) {
    return; 
  }
  
  if (read_bit(fetched,27) == 1) {
    branch(fetched);
  } else if (read_bit(fetched,26) == 1) {
    //single_data_transfer(fetched);
  } else if (read_bit(fetched,25) == 0 && read_bit(fetched, 4) == 1 &&
  read_bit(fetched, 7) ==1) {
    //multiply(fetched);
  } else {
    data_processing(fetched);
  }
  return;
}

bool equal(void) {
  return read_bit(cpsr, 30) == 1;
}

bool not_equal(void) {
  return !equal();
}

bool greater_equal(void) {
  return (read_bit(cpsr,31) == read_bit(cpsr, 28));
}

bool less(void) {
  return (read_bit(cpsr,31) != read_bit(cpsr, 28));
}

bool greater(void) {
  return (not_equal() && greater_equal());
}

bool less_equal(void) {
  return (equal() && less());
}

bool always(void) {
  return true;
}

bool condition(int cond_code) {
  bool (*check_cpsr[15])(void);
 
  check_cpsr[0] = &equal;
  check_cpsr[1] = &not_equal;
  check_cpsr[10] = &greater_equal;
  check_cpsr[11] = &less;
  check_cpsr[12] = &greater;
  check_cpsr[13] = &less_equal;
  check_cpsr[14] = &always;
  
  return (*check_cpsr[cond_code])();
}

uint32_t set_bit(uint32_t number, int bit) {
	int ret = number;
	if (!read_bit(number, bit)) {
		ret += pow (2, bit);
	}
	return ret;
}

uint32_t reset_bit(uint32_t number, int bit) {
	int ret = number;
	if (read_bit(number, bit)) {
		ret -= pow (2, bit);
	}
	return ret;
}

uint32_t set_bit_z(uint32_t number, uint32_t res) {
	uint32_t ret = number;
	if (res == 0) {
		ret = set_bit(number, 30);
	} else {
		ret = reset_bit(number,30);
	}
	
	return ret;
}


// Assume operand 2 has been processed to give the value
// Need to do the operand 2 processing
uint32_t and(uint32_t rn, uint32_t value, uint32_t rd) {
  registers[rd] = rn & value;
  uint32_t ret = cpsr;
}

uint32_t eor(uint32_t rn, uint32_t value, uint32_t rd) {
  registers[rd] = rn ^ value;
  return rd;
}

uint32_t sub(uint32_t rn, uint32_t value, uint32_t rd) {
  registers[rd] = rn - value;
  return rd;
}

uint32_t rsb(uint32_t rn, uint32_t value, uint32_t rd) {
  registers[rd] = value - rn;
  return rd;
}

uint32_t add(uint32_t rn, uint32_t value, uint32_t rd) {
  registers[rd] = rn + value;
  uint32_t ret = cpsr;
  if (value > pow(2 , 31) - rd){
	carryout = true;
	ret = set_bit(ret, 29);
  } else {
	ret = reset_bit(ret, 29);
  }
  
  if(registers[rd] == 0) {
	ret = set_bit(
  }
  return ret;
}

uint32_t tst(uint32_t rn, uint32_t value, uint32_t rd) {
  return (rn & value);
}

uint32_t teq(uint32_t rn, uint32_t value, uint32_t rd) {
  return (rn ^ value);  
}

uint32_t cmp(uint32_t rn, uint32_t value, uint32_t rd) {
  return (rn - value);
}

uint32_t orr(uint32_t rn, uint32_t value, uint32_t rd) {
  return (rn | value);
}

uint32_t mov(uint32_t rn, uint32_t value, uint32_t rd) {
  registers[rd] = value;
  return rd;
}

void data_processing(int instruction){
  
  int o2;
  int bitS          = 20;
  int bitI          = 25;
  int opcode_strt   = 21;
  int rn_strt       = 16;
  int rd_strt       = 12;
  int immstrt       = 0;
  int immlgth       = 8;
  int rotstrt       = 8;
  int rotlngth      = 4;
  int rmstrt        = 0;
  int rmlngth       = 4;
  int operand2bit4  = 4;
  
  uint32_t (*op_ptrs[14])(uint32_t rn, uint32_t value, uint32_t rd);
  op_ptrs[0] = and;
  op_ptrs[1] = eor;
  op_ptrs[2] = sub;
  op_ptrs[3] = rsb;
  op_ptrs[4] = add;
  op_ptrs[8] = tst;
  op_ptrs[9] = teq;
  op_ptrs[10] = cmp;
  op_ptrs[12] = orr;
  op_ptrs[13] = mov;
  // to call a function we do:
  // (*op_ptrs[opcode])(rn, value, rd)
  
  int opcode = extractbits(instruction, opcode_strt, 4);
  uint32_t rn = registers[extractbits(instruction, rn_strt, 4)];
  uint32_t rd = extractbits(instruction, rd_strt, 4);
  
  carryout = false;
  
  if (read_bit(instruction, bitI)) { // I = 1 if operand 2 is an immediat value
    printf("I = 1\n");
    unsigned int imm    = extract_bits(instruction, immstrt, immlgth);
    unsigned int rotate = extract_bits(instruction, rotstrt, rotlngth);
    rotate              <<= 2;
    o2                  = rotate_right(imm, rotate);
    // o2 when I = 1
  } else { // I = 0  we shift register
        printf("I = 0\n");
        int rm         = extract_bits(instruction, rmstrt, rmlngth);
        int valrm      = registers[rm]; // value of register
        int shifttype  = extract_bits(instruction, 5, 2);//4 possible shift codes
        int shiftvalue = 0;
        if (read_bit(instruction, operand2bit4)) { // if bit 4 is 1
            int rs = extract_bits(instruction, 8, 4);
            int valrs = registers[rs]; // value of the register
            int bottombyte = extract_bits(valrs, 24, 8); // last 8 bits of valrs
            shiftvalue = bottombyte;
        } else { // if bit 4 is 0
            shiftvalue = extract_bits(instruction, 7, 5);
        }
        switch (shifttype) {
          case (0) :  // 00
            o2 = lsl(valrm, shiftvalue);
            break;
          case (1) :  // 01
            o2 = lsr(valrm, shiftvalue);
            break;
          case (2) :  // 10
            o2 = asr(valrm, shiftvalue);
            break;
          case (3) :  // 11
            o2 = ror(valrm, shiftvalue);
            break;
          default :
            printf("Error in choosing shift type\n");
            return;
        }
        // o2 when I = 0
        printf("o2 = %d\n", o2); 
    }
    
    uint32_t value = o2;
    uint32_t newCPSR = (*op_ptrs[opcode])(rn, o2, rd);
    
    if (read_bit(instruction, bitS)) {
    cpsr = newCPSR;
  }

}

int lsl(int reg, int shiftvalue) {
  if (shiftvalue != 0) {
    carryout = read_bit(reg, instlgth - shiftvalue);
    return reg << shiftvalue;
  } else {
    return reg;
  }
}

int lsr(int reg, int shiftvalue) {
  if (shiftvalue != 0) {
    carryout = read_bit(reg, shiftvalue - 1);
    return reg >> shiftvalue;
  } else {
    return reg;
  }
}

int asr(int reg, int shiftvalue) {
  if (shiftvalue != 0) {
    bool lastbit = read_bit(reg, instlgth - 1);
    carryout = read_bit(reg, shiftvalue - 1);
    int reglsr = lsr(reg, shiftvalue);
    if (lastbit) {
      int mask = (1 << shiftvalue) - 1;
      return reglsr | mask;
    } else {
      return reglsr;
    }
  } else {
    return reg;
  }
}

int ror(int reg, int shiftvalue) {
  return rotate_right(reg, shiftvalue);
}


//reading start from right to left. begin with index 0
bool read_bit(int instruction, int index) { 
  return extract_bits(instruction, index, 1) == 1;
}

// start is the start bit from where we extract the bits
// length is how many bits we want to extract
// instruction is from where we want to extract
int extract_bits(int instruction, int start, int length) {       
  if (start > 31 || start < 0) {                           
    printf("%s\n", "take int error");                   
    printf("start = %d, length = %d\n", start, length);
    return 0;
  } else {
      instruction >>= start;
      int mask = (1 << length) - 1;
      return mask & instruction;     
  }


int rotate_right(int x, int y) {
  if (y != 0) {
    carryout = read_bit(x, y-1);
    int firstYbits = x << (32 - y);
    int lastbits   = x >> y;
    return (firstYbits | lastbits);
   } else {
    return x;
   }   
}

void branch (uint32_t fetched) {
  const int cond = extract_bits(fetched, 28, 4);
  if (!condition(cond)) {
    printf("\nlol you fag\n");
    return;
  }
  
  int offset = extract_bits(fetched, 0, 24);
  offset = offset << 2;
  int32_t move = offset;
  pc += move;
}

void output_machine_state(void) {
  printf("Registers:\n");
  for(int i=0; i<13; i++) {
    printf("$%-3d: %10d (%0#10x)\n", i, registers[i], registers[i]);
  }
  
  printf("PC  : %10d (%0#10x)\n", pc, pc);
  printf("CPSR: %10d (%0#10x)\n", cpsr, cpsr);

  printf("Non-zero memory:\n");

  for(uint32_t i=0; i< MEM_SIZE; i+=4) {
    uint32_t fetched = 0;
    int j;
    for(j=0; j<3; j++) {
      fetched += memory[i+j];
      fetched = fetched << 8;
    }
    fetched += memory[i+j];
    uint32_t index = 0;
    index = index << 24;
    index += i; 
    if (fetched != 0) { 
      printf("%0#10x: %0#10x\n", index, fetched);
    }
  }

void single_data_transfer(uint32_t instruction) {
  const int bit_I = 25;
  const int bit_P = 24;
  const int bit_U = 23;
  const int bit_L = 20;
  const int rn_strt = 16;
  const int rd_strt = 12;
  const int offset_strt = 0;

  int rn = extractbits(instruction, rn_strt, 4);
  int rd = extractbits(instruction, rd_strt, 4);

  int offset;

  if (read_bit(instruction, bit_I)) { // I = 1 so offset is shifted reg
    printf("I = 1\n");
    int rm         = extractbits(instruction, offset_strt, 4);
    int valrm      = registers[rm]; // value of register
    int shifttype  = extractbits(instruction, 5, 2);//4 possible shift codes
    int shiftvalue = 0;
    if (read_bit(instruction, 4)) { // if bit 4 is 1
      int rs = extractbits(instruction, 8, 4);
      int valrs = registers[rs]; // value of the register
      int bottombyte = extractbits(valrs, 24, 8); // last 8 bits of valrs
      shiftvalue = bottombyte;
    } else { // if bit 4 is 0
      shiftvalue = extractbits(instruction, 7, 5);
    }
    switch (shifttype) {
      case (0) :
        offset = lsl(valrm, shiftvalue);
        break;
      case (1) :
        offset = lsr(valrm, shiftvalue);
        break;
      case (2) :
        offset = asr(valrm, shiftvalue);
        break;
      case (3) :
        offset = ror(valrm, shiftvalue);
        break;
      default :
        printf("Error in choosing shift type\n");
        return;
    }
    printf("offset = %d\n", offset); 
  } else { // I = 0, offset is immediate
    printf("I = 0\n");
    unsigned int imm    = extractbits(instruction, offset_strt, 8);
    unsigned int rotate = extractbits(instruction, 8, 4);
    rotate              <<= 2;
    offset              = rotatateright(imm, rotate);
  }

  if (read_bit(instruction, bit_P)) {
    // offset added/subtracted before transfer
    if (read_bit(instruction, bit_U)) {
      // add to base reg
      offset += registers[rn];
      if (read_bit(instruction, bit_L)) {
        // load from memory
        registers[rd] = memory[offset];
      } else {
        // store in memory
        memory[offset] = registers[rd];
      }
    } else {
      // subtract from base reg
      offset -= registers[rn];
      if (read_bit(instruction, bit_L)) {
        // load from memory
        registers[rd] = memory[offset];
      } else {
        memory[offset] = registers[rd];
      }
    }

  } else {
    // offset added/subtracted after transfer
    if (read_bit(instruction, bit_U)) {
      // add to base reg
      if (read_bit(instruction, bit_L)) {
        // load from memory
        registers[rd] = memory[registers[rn]];
      } else {
        // store in memory
        memory[registers[rn]] = registers[rd];
      }
      registers[rn] += offset;
    } else {
      // subtract from base reg
      if (read_bit(instruction, bit_L)) {
        // load from memory
        registers[rd] = memory[registers[rn]];
      } else {
        memory[registers[rn]] = registers[rd];
      }
      registers[rn] -= offset;
    }    

  }
}
