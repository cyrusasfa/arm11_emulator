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

char memory[64 * 1024]; // 1 char = 1 byte 
int registers[17]; 

void dataprocessing(int);

void dataprocessing(int instruction){
	int o2;
	int bitS     = 20;
	int bitI     = 25;
	int immstrt  = 0;
	int immlgth  = 8;
	int rotstrt  = 8;
	int rotlngth = 4;
  int rmstrt   = 0;
  int rmlngth  = 4;
	bool flagS   = false;
	if (readbit(instruction, bitS)) {
		flagS = true;
		printf("Set flag S");
	}
	if (readbit(instruction, bitI)){ // I = 1 if operand 2 is an immediat value
        printf("I = 1\n");
		o2 = extractbits(instruction)
		unsigned int imm    = extractbits(instruction, imm, immlgth);
		unsigned int rotate = extractbits(instruction, rotstrt, rotlngth);
		rotate <<= 2;
		o2 = rotatateright(imm, rotate);
		printf("o2 = %d\n", o2);

	} else { // I = 0  we shift register
        printf("I = 0\n");
        int rm = extractbits(instruction, rmstrt, rmlngth);
        
    }

}

bool readbit(int instruction, int index) { //reading start from right to left. begin with index 0
	return extractbits(instrc, index, 1) == 1;
}

void decodeAndExecute(uint32_t fetched) {
  if (!fetched) {
    return null;
  }
}

int extractbits(int instruction, int start, int length) {      // start is the start bit from where we extract the bits 
  if (start > 31 || start < 0) {                           // length is how many bits we want to extract
	  printf("%s\n", "extract bits error");                    // instructionm is from where we want to extract
	  printf("start = %d, length = %d\n", start, length);
	  return 0;
  } else {
    instruction >>= start;
    int mask = (1 << length) - 1;
    instruction = mask & instruction;     
  }
}

int rotateright(int x, int y) {
   if (y != 0) {
	   //carryout = readbit(x, y-1);
	   int firstYbits = x << (32 - y);
	   int lastbits = x >> y;
	   return (firstYbits | lastbits);
   } else {
	   return x;
   }
}

// assume operand 2 has been processed and the value converted to 32-bit
void and_imm(uint32_t rn, uint32_t value, uint32_t rd) {
  rd = rn & o2;
}

void and_reg(uint32_t rn, uint32_t value, uint32_t rd) {

}
