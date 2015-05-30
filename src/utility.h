#ifndef UTILITY_H
#define UTILITY_H

bool read_bit(int instruction, int index);

int extract_bits(int instruction, int start, int length);

int32_t set_bit(int32_t number, int bit);

int32_t reset_bit(int32_t number, int bit);

#endif
