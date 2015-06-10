#ifndef GPIO_H
#define GPIO_H

#define control_bits 3

void gpio_run (uint32_t memory_out, uint32_t memory_clear, uint32_t memory_set, uint8_t gpio_pin);

void clear_pin(uint32_t memory_clear, uint8_t bit);

void set_pin(uint32_t memory_set, uint8_t bit);

int checkGPIO(uint32_t memory);

bool check_address(int address);

void GPIO_access (uint32_t memory);

int GPIO_bits_value (uint32_t memory, uint8_t gpio_pin);

int GPIO_starting_bit (uint8_t gpio_pin);

#endif

