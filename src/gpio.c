#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "utility.h"
#include "gpio.h"
#include <unistd.h>

#define F_PIN_S (0x20200000)
#define F_PIN_M (0x20200004)
#define F_PIN_E (0x20200008)
#define SET_P (0x2020001c)
#define CLR_P (0x20200028)

void GPIO_access (struct machine_state *mach, uint32_t* args ) {
	switch (args[1]) {
		case (F_PIN_S) :
			printf("%s\n","One GPIO pin from 0 to 9 has been accessed");
      break;
		case (F_PIN_M) :
			printf("%s\n","One GPIO pin from 10 to 19 has been accessed");
      break;
		case (F_PIN_E) :
			printf("%s\n","One GPIO pin from 20 to 29 has been accessed");
      break;
		default :
			printf("%s\n","Error");
	}  
  mach->registers[args[0]] = args[1];
}

void GPIO_write(struct machine_state *mach, uint32_t *args) {
  switch (args[1]) {
		case (F_PIN_S) :
			printf("%s\n","One GPIO pin from 0 to 9 has been accessed");
      break;
		case (F_PIN_M) :
			printf("%s\n","One GPIO pin from 10 to 19 has been accessed");
      break;
		case (F_PIN_E) :
			printf("%s\n","One GPIO pin from 20 to 29 has been accessed");
      break;
		case (SET_P) :
			printf("PIN ON\n");
	    break;
    case (CLR_P) :
      printf("PIN OFF\n");
      break;
    default :
      printf("ERROR\n");
  }  
}

bool check_address(uint32_t address) {
  return
  (address >= F_PIN_S && address <= F_PIN_E) || address == CLR_P || address == SET_P; 
}
