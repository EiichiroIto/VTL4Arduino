#ifndef __MEMORY__
#define __MEMORY__

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

extern uint8_t *core;

uint8_t get_byte(uint16_t address);
uint16_t get_word(uint16_t address);
void set_byte(uint16_t address, uint8_t value);
void set_word(uint16_t address, uint16_t value);
uint8_t copy_byte(uint16_t src, uint16_t dst);
uint16_t count_until(uint16_t address, uint8_t value);

#ifdef __cplusplus
};
#endif
#endif
