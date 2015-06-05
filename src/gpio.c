#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "utility.h"
#include "gpio.h"

void gpio_run (uint32_t memory_out, uint32_t memory_clear, uint32_t memory_set, int gpio_pin) {
	if(extrct_bits(memory, 18, 3) != 1) {
		memory_out = set_bit(memory_out, 18);
		memory_out = clear_bit(memory_out, 19);
		memory_out = clear_bit(memory_out, 20);
	}
	for (; i < 10; ++i){
		clear_pin(memory_clear, gpio_pin);
		sleep(1000);
		set_pin(memory_set, gpio_pin);
		sleep(1000);
	}
}

void clear_pin(uint32_t memory_clear, int bit) {
	memory_clear = clear_bit(memory_clear, bit);
}

void set_pin(uint32_t memory_set, int bit) {
	memory_set = set_bit(memory_set, bit);
}