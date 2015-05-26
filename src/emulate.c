#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <stdint.h>

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

bool readbit(int instruction, int index);

int takeint(int instruction, int start, int length);

int rotatateright(int x, int y);

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
  memory    = (uint8_t *) malloc(MEM_SIZE * sizeof(uint8_t));
  
  for (int i=0; i < MEM_SIZE; i++) {
    memory[i] = 0;
  }
  //Set all memory locations to 0
  
  for (int i=0; i < NUM_REGS; i++) {
    registers[i] = 0; 
  }
  //Set all registers to 0
}

int loadMemory(const char *fileName) {

  FILE *program;
  program  = fopen(fileName, "rb");
  
  assert (program);
  int ret = fread(memory, 1, MEM_SIZE, program);

  fclose(program);
  return ret;
}

uint32_t fetch(int address) {
  uint32_t ret = 0;
  for(int i=3; i > 0; --i) {
    ret += memory[address+i];
    ret = ret << 8;
  }
  return ret += memory[address];
}

void decodeAndExecute(uint32_t fetched) {
  if (!fetched) {
    return NULL; 
  }
  return NULL; 
}

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
		o2 = takeint(instruction)
		unsigned int imm    = takeint(instruction, imm, immlgth);
		unsigned int rotate = takeint(instruction, rotstrt, rotlngth);
		rotate <<= 2;
		o2 = rotatateright(imm, rotate);
		printf("o2 = %d\n", o2);

	} else { // I = 0  we shift register
        printf("I = 0\n");
        int rm = takeint(instruction, rmstrt, rmlngth);
        
    }

}

//reading start from right to left. begin with index 0
bool readbit(int instruction, int index) { 
	return takeint(instrc, index, 1) == 1;
}

// start is the start bit from where we extract the bits
// length is how many bits we want to extract
// instructionm is from where we want to extract
int takeint(int instruction, int start, int length) {       
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

int rotatateright(int x, int y) {
   if (y != 0) {
	   //carryout = readbit(x, y-1);
	   int firstYbits = x << (32 - y);
	   int lastbits = x >> y;
	   retun (firstYbits | lastbits);
   } else {
	  return x;
   }
}
