#include <stdio.h>

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

void initializeMemories();

int loadMemory(const char *filename);

uint32_t fetch(int address); 

void decodeAndExecute(uint32_t fetched);

void execute(void *decoded);

void dataprocessing(int);

bool read_bit(uint32_t instruction, int index); 

int extract_bits(uint32_t instruction, int start, int length);

int rotate_right(int op, int rotation); 

int main(int argc, char **argv) {
  assert (argc == 2);
  const char *fileName = argv[1]; 
  
  initializeMemories();
    //Set all memory locations and registers to 0 
  loadMemory(fileName);
    //Parse binary file and upload contents into memory
  
  uint32_t fetched;
  //void *decoded = NULL;
  
  while (1) {
    // execute(decoded);
    // decoded = decode(fetched);
    fetched = fetch(pc);
    printf("%u", fetched);
     pc += 4;    
  }

  //printf("%u", fetched);
  
  free(memory); 
    
  return EXIT_SUCCESS; 
}

void dataprocessing(int);

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

}

uint32_t fetch(int address) {
  uint32_t ret = 0;
  for(int i = 3; i > 0; --i) {
    ret += memory[address+i];
    ret = ret << 8;
  }
  return ret += memory[address];
}

// decodes fetched instruction and calls appropriate execution function
void decodeAndExecute(uint32_t fetched) {
  if (!fetched) {
    return null;
  }

  if (read_bit(fetched, 27) == 1) {
    branch(fetched);
  }
  else if (read_bit(fetched, 26) == 1) {
      singledatatransfer(fetched);
  }
  else if (read_bit(fetched, 25) == 0 && readbit(fetched, 4) == 1 
    && read_bit(fetched, 7) == 1) {
      //This works because if bit 25 is 0 in data processing 
      //then either bit 4 or bit 7 will be 0 as well 
    multiply(fetched);
  }
  else {
    dataprocessing(fetched);
  }
}

void data_processing(uint32_t instruction){
  int o2;
  int bitS         = 20;
  int bitI         = 25;
  int immstrt      = 0;
  int immlgth      = 8;
  int rotstrt      = 8;
  int rotlngth     = 4;
  int rmstrt       = 0;
  int rmlngth      = 4;
  int operand2bit4 = 4;
  bool flagS       = false;
  if (read_bit(instruction, bitS)) {
    flagS = true;
    printf("Set flag S");
  }
  if (read_bit(instruction, bitI)){ // I = 1 if operand 2 is an immediat value
        printf("I = 1\n");
    o2                  = extract_bits(instruction)
    unsigned int imm    = extract_bits(instruction, imm, immlgth);
    unsigned int rotate = extract_bits(instruction, rotstrt, rotlngth);
    rotate <<= 2;
    o2                  = rotatateright(imm, rotate);
    printf("o2 = %d\n", o2);

  } else { // I = 0  we shift register
        printf("I = 0\n");
        int rm         = extract_bits(instruction, rmstrt, rmlngth);
        int valrm      = registers[rm]; // value of register
        int shifttype  = extract_bits(instruction, 5, 2);
        int shiftvalue = 0;
        if (read_bit(instruction, operand2bit4)){ // if bit 4 is 1
            int rs = extract_bits(instrc, 8, 4);
            int vrs = reg[rs];
        }

    }

}

//reading start from right to left. begin with index 0
bool read_bit(uint32_t instruction, int index) { 
	return extract_bits(instrc, index, 1) == 1;
}

// start is the start bit from where we extract the bits
// length is how many bits we want to extract from right to left
// instruction is the uint32_t from which we want to extract
int extract_bits(uint32_t instruction, int start, int length) {       
  if (start > 31 || start < 0) {                           
	  printf("%s\n", "take int error");                   
	  printf("start = %d, length = %d\n", start, length);
	  return 0;
  } else {
      instruction >>= start;
      int mask = (1 << length) - 1;
      instruction = mask & instruction;     
  }
}

int rotate_right(int op, int rotation) {
   if (rotation != 0) {
	   //carryout = read_bit(x, y-1);
	   int firstYbits = op << (32 - rotation);
	   int lastbits = op >> rotation;
	   return (firstYbits | lastbits);
   } else {
	   return op;
   }
}

// Assume operand 2 has been processed to give the value
// Need to do the operand 2 processing
void and(uint32_t rn, uint32_t value, uint32_t rd) {
  rd = rn & o2;
}

void eor(uint32_t rn, uint32_t value, uint32_t rd) {
  rd = rn ^ value;
}

void sub(uint32_t rn, uint32_t value, uint32_t rd) {
  rd = rn - value;
}

void rsb(uint32_t rn, uint32_t value, uint32_t rd) {
  rd = value - rn;
}

void add(uint32_t rn, uint32_t value, uint32_t rd) {
  rd = rn + value;
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

void mov(uint32_t rn, uint32_t value, uint32_t rd) {
  rd = value;
}
