#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "utility.h"
#include "gpio.h"
#include <unistd.h>

void gpio_run (uint32_t memory_out, uint32_t memory_clear, uint32_t memory_set, 
																uint8_t gpio_pin) {
	
	int starting_bit = GPIO_starting_bit(gpio_pin);
  if(GPIO_bits_value(memory_out, gpio_pin) != 1) {

		set_pin(memory_out, starting_bit);
		clear_pin(memory_out, starting_bit + 1);
		clear_pin(memory_out, starting_bit + 2);
	}
	for (int i = 0; i < 10; ++i){
		clear_pin(memory_clear, gpio_pin);
		sleep(1000);
		set_pin(memory_set, gpio_pin);
		sleep(1000);
	}
}

int GPIO_bits_value (uint32_t memory, uint8_t gpio_pin) {	
	return extract_bits(memory, GPIO_starting_bit(gpio_pin), control_bits);
}

int GPIO_starting_bit (uint8_t gpio_pin) {
	int aux = gpio_pin % 10; // turn the pin to 0 -> 9
	return control_bits * aux; // starting bit of the bulk we need
}

void GPIO_access (uint32_t memory) {
	switch (checkGPIO(memory)) {
		case (0) :
			printf("%s\n","One GPIO pin from 0 to 9 has been accessed");
			break;
		case (1) :
			printf("%s\n","One GPIO pin from 10 to 19 has been accessed");
			break;
		case (2) :
			printf("%s\n","One GPIO pin from 0 to 9 has been accessed");
			break;
		case (-1) :
			printf("%s\n","Erorr");
	}
}
int checkGPIO(uint32_t memory) {
	if (memory == 538968064) { //0x20200000
		return 0;
	} else if(memory == 538968068) { //0x20200004
		return 1;
	} else if(memory == 538968072) { //0x20200008
      return 2;
	} else {
	  return -1;
	}
}

bool check_address(int address) {
	if (address >= 64 * 1024) {
		printf("Error: Out of bounds memory access at address %d\n", address);
		return false;
	} else {
		return true;
	}
}


void clear_pin(uint32_t memory_clear, uint8_t bit) {
	memory_clear = clear_bit(memory_clear, (uint8_t)bit);
}

void set_pin(uint32_t memory_set, uint8_t bit) {
	memory_set = set_bit(memory_set, (uint8_t)bit);
}