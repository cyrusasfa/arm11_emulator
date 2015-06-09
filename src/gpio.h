#ifndef GPIO_H
#define GPIO_H

void gpio_run (uint32_t memory_out, uint32_t memory_clear, uint32_t memory_set, int gpio_pin);

void clear_pin(uint32_t memory_clear, int bit);

void set_pin(uint32_t memory_set, int bit);

#endif

