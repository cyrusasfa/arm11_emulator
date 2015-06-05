#ifndef SINGLEDATATRANSFER_H
#define SINGLEDATATRANSFER_H

void decode_data_trans(struct machine_state *mach, uint32_t instruction);
// Helpers
uint32_t load_mem_word(uint32_t start);
void store_mem_word(uint32_t word, uint32_t start);

#endif