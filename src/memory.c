#include "memory.h"

uint8_t *core = 0;

uint8_t get_byte(uint16_t address)
{
  return core[address];
}

uint16_t get_word(uint16_t address)
{
  return (uint16_t) core[address] + ((uint16_t) core[address+1] * 256);
}

void set_byte(uint16_t address, uint8_t value)
{
  core[address] = value;
}

void set_word(uint16_t address, uint16_t value)
{
  core[address] = value & 0xFF;
  core[address+1] = value >> 8;
}

uint8_t copy_byte(uint16_t src, uint16_t dst)
{
  uint8_t c = core[src];
  core[dst] = c;
  return c;
}

uint16_t count_until(uint16_t address, uint8_t value)
{
  uint16_t count = 0;

  while (core[address++] != value) {
    count ++;
  }
  return count;
}
