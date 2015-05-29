#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

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
_Bool carryout;
_Bool flagS;
uint32_t instlgth = 32;

void initializeMemories();

int loadMemory(const char *filename);

uint32_t fetch(int address); 

void decodeAndExecute(uint32_t fetched);

void execute(void *decoded);

void dataprocessing(int);

_Bool read_bit(int instruction, int index);

int extractbits(int instruction, int start, int length);

int rotatateright(int x, int y);

int lsl(int reg, int shiftvalue);

int lsr(int reg, int shiftvalue);

int asr(int reg, int shiftvalue);

int ror(int reg, int shiftvalue);




int main(int argc, char **argv) {
  assert (argc == 2);
  const char *fileName = argv[1]; 
  
  initializeMemories();
    //Set all memory locations and registers to 0 
  loadMemory(fileName);
    //Parse binary file and upload contents into memory
  
  uint32_t fetched;
  //void *decoded = NULL;
  
  //while () {
    // execute(decoded);
    // decoded = decode(fetched);
    // fetched = fetch(pc);
    // pc += 4;    
  //}

  fetched = fetch(0);
  printf("%u", fetched);
  
  free(memory); 
    
  return EXIT_SUCCESS; 
}

void initializeMemories() {
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

int loadMemory(const char *fileName) {

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

void decodeAndExecute(uint32_t fetched) {
  if (!fetched) {
    return; 
  }
  return; 
}

// Assume operand 2 has been processed to give the value
// Need to do the operand 2 processing
uint32_t and(uint32_t rn, uint32_t value, uint32_t rd) {
  rd = rn & value;
  // if (flagS && carryout) {
  //   setC();
  // }
  return rd;
}

uint32_t eor(uint32_t rn, uint32_t value, uint32_t rd) {
  rd = rn ^ value;
  return rd;
}

uint32_t sub(uint32_t rn, uint32_t value, uint32_t rd) {
  rd = rn - value;
  return rd;
}

uint32_t rsb(uint32_t rn, uint32_t value, uint32_t rd) {
  rd = value - rn;
  return rd;
}

uint32_t add(uint32_t rn, uint32_t value, uint32_t rd) {
  rd = rn + value;
  return rd;
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
  rd = value;
  return rd;
}

void dataprocessing(int instruction){
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
  _Bool flagS       = false;

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
  uint32_t rd = registers[extractbits(instruction, rd_strt, 4)];

  if (read_bit(instruction, bitS)) {
    flagS = true;
    printf("Set flag S\n");
  }
  if (read_bit(instruction, bitI)) { // I = 1 if operand 2 is an immediat value
    printf("I = 1\n");
    unsigned int imm    = extractbits(instruction, immstrt, immlgth);
    unsigned int rotate = extractbits(instruction, rotstrt, rotlngth);
    rotate              <<= 2;
    o2                  = rotatateright(imm, rotate);
    // o2 when I = 1
  } else { // I = 0  we shift register
    printf("I = 0\n");
    int rm         = extractbits(instruction, rmstrt, rmlngth);
    int valrm      = registers[rm]; // value of register
    int shifttype  = extractbits(instruction, 5, 2);//4 possible shift codes
    int shiftvalue = 0;
    if (read_bit(instruction, operand2bit4)) { // if bit 4 is 1
      int rs = extractbits(instruction, 8, 4);
      int valrs = registers[rs]; // value of the register
      int bottombyte = extractbits(valrs, 24, 8); // last 8 bits of valrs
      shiftvalue = bottombyte;
    } else { // if bit 4 is 0
      shiftvalue = extractbits(instruction, 7, 5);
    }
    switch (shifttype) {
      case (0) :
        o2 = lsl(valrm, shiftvalue);
        break;
      case (1) :
        o2 = lsr(valrm, shiftvalue);
        break;
      case (2) :
        o2 = asr(valrm, shiftvalue);
        break;
      case (3) :
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
    (*op_ptrs[opcode])(rn, value, rd);
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
    _Bool lastbit = read_bit(reg, instlgth - 1);
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
  return rotatateright(reg, shiftvalue);
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

//reading start from right to left. begin with index 0
_Bool read_bit(int instruction, int index) { 
  return extractbits(instruction, index, 1) == 1;
}

// start is the start bit from where we extract the bits
// length is how many bits we want to extract
// instructionm is from where we want to extract
int extractbits(int instruction, int start, int length) {       
  if (start > 31 || start < 0) {                           
    printf("%s\n", "take int error");                   
    printf("start = %d, length = %d\n", start, length);
    return 0;
  } else {
    instruction >>= start;
    int mask = (1 << length) - 1;
    return mask & instruction;     
  }
}

int rotatateright(int x, int y) {
  if (y != 0) {
    //carryout = read_bit(x, y-1);
    int firstYbits = x << (32 - y);
    int lastbits = x >> y;
    return (firstYbits | lastbits);
  } else {
   return x;
  }
}

