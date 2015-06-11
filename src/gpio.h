#ifndef GPIO_H
#define GPIO_H

bool check_address(uint32_t address);

void GPIO_access (struct machine_state *mach, uint32_t *args);

void GPIO_write (struct machine_state *mach, uint32_t *args);

#endif

