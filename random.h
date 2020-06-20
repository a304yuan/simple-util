#ifndef SIMPLE_RANDOM_H
#define SIMPLE_RANDOM_H

#include <stdint.h>

extern uint32_t xorshift32(uint32_t * state);
extern uint64_t xorshift64(uint64_t * state);

#endif