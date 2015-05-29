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
uint32_t instlgth = 32;

void initializeMemories();

int loadMemory(const char *filename);

uint32_t fetch(int address); 

void decodeAndExecute(uint32_t fetched);

void execute(void *decoded);

void dataprocessing(int);

_Bool readbit(int instruction, int index);

int extractbits(int instruction, int start, int length);

int rotateright(int x, int y);

int lsl(int reg, int shiftvalue);

int lsr(int reg, int shiftvalue);

int asr(int reg, int shiftvalue);

int ror(int reg, int shiftvalue);




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


void dataprocessing(int instruction){
  int o2;
  int bitS          = 20;
  int bitI          = 25;
  int immstrt       = 0;
  int immlgth       = 8;
  int rotstrt       = 8;
  int rotlngth      = 4;
  int rmstrt        = 0;
  int rmlngth       = 4;
  int operand2bit4  = 4;
  _Bool flagS       = false;
  carryout          = false;
  if (readbit(instruction, bitS)) {
    flagS = true;
    // should update CPSR!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    printf("Set flag S\n");
  }
  if (readbit(instruction, bitI)) { // I = 1 if operand 2 is an immediat value
    printf("I = 1\n");
    unsigned int imm    = extractbits(instruction, immstrt, immlgth);
    unsigned int rotate = extractbits(instruction, rotstrt, rotlngth);
    rotate              <<= 2;
    o2                  = rotateright(imm, rotate);

    // o2 when I = 1
  } else { // I = 0  we shift register
        printf("I = 0\n");
        int rm         = extractbits(instruction, rmstrt, rmlngth);
        int valrm      = registers[rm]; // value of register
        int shifttype  = extractbits(instruction, 5, 2);//4 possible shift codes
        int shiftvalue = 0;
        if (readbit(instruction, operand2bit4)) { // if bit 4 is 1
            int rs = extractbits(instruction, 8, 4);
            int valrs = registers[rs]; // value of the register
            int bottombyte = extractbits(valrs, 24, 8); // last 8 bits of valrs
            shiftvalue = bottombyte;
        } else { // if bit 4 is 0
            shiftvalue = extractbits(instruction, 7, 5);
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
        }
        // o2 when I = 0
        printf("o2 = %d\n", o2); 
    }
}






//reading start from right to left. begin with index 0
_Bool readbit(int instruction, int index) { 
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

int rotateright(int x, int y) {
   if (y != 0) {
     carryout       = readbit(x, y - 1);
     int firstYbits = x << (32 - y);
     int lastbits   = x >> y;
     return (firstYbits | lastbits);
   } else {
    return x;
   }   
}

_Bool checkCond (int instruction) {

}


// void single_data_transfer(uint32_t instruction) {
//   const int bit_I = 25;
//   const int bit_P = 24;
//   const int bit_U = 23;
//   const int bit_L = 20;
//   const int rn_strt = 16;
//   const int rd_strt = 12;
//   const int offset_strt = 0;

//   if (read_bit(instruction, bit_I)) { // I = 1 so offset is shifted reg
//     // STMTS
//     //
//     //
//     //
//     //
//     //
//     //
//     //
//     //
//     //
//     //
//     //
//     //
//     //
//     //
//     //
//     //
//     //
//     //
//     //
//     //
//     //
//     //
//   } else { // I = 0 offset is immediate
//     if (read_bit(instruction, bit_P)) {
//       // offset added/subtracted before transfer
//       if (read_bit(instruction, bit_U)) {
//         // add to base reg
//         if (read_bit(instruction, bit_L)) {
//           // load from memory
//         } else {
//           // store in memory
//         }
//       } else {
//         // subtract from base reg
//         if (read_bit(instruction, bit_L)) {
//           // load from memory
//         } else {
//           // store in memory
//         }
//       }

//     } else {
//       // offset added/subtracted after transfer
//       if (read_bit(instruction, bit_U)) {
//         // add to base reg
//       } else {
//         // subtract from base reg
//       }
//     }
//   }
// }


