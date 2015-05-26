#include <stdio.h>

struct registers {
	int r_0;  // general purpose
	int r_1;  // general purpose
	int r_2;  // general purpose
	int r_3;  // general purpose
	int r_4;  // general purpose
	int r_5;  // general purpose
	int r_6;  // general purpose
	int r_7;  // general purpose
	int r_8;  // general purpose
	int r_9;  // general purpose
	int r_10; // general purpose
	int r_11; // general purpose
	int r_12; // general purpose
	int r_13; // SP
	int r_14; // LR
	int r_15; // PC
	int r_16; // CPSR
};

char memory[64 * 1024]; // 1 char = 1 byte 
int registers[17]; 

void dataprocessing(int);

int main(){


	return 0;
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

bool readbit(int instruction, int index) { //reading start from right to left. begin with index 0
	return takeint(instrc, index, 1) == 1;
}

int takeint(int instruction, int start, int length) {      // start is the start bit from where we extract the bits 
  if (start > 31 || start < 0) {                           // length is how many bits we want to extract
	  printf("%s\n", "take int error");                    // instructionm is from where we want to extract
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