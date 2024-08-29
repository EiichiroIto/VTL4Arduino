#include <avr/pgmspace.h>
#include "porting.h"

const unsigned char AddressTable[96] PROGMEM = {
   68,  70,  72,  74,  76,  78,  80,  82,  84,  86,  88,  90,  92,  94,  96,  98,
  100, 102, 104, 106, 108, 110, 112, 114, 116, 118, 120, 122, 124, 126, 128, 130,
    4,   6,   8,  10,  12,  14,  16,  18,  20,  22,  24,  26,  28,  30,  32,  34, 
   36,  38,  40,  42,  44,  46,  48,  50,  52,  54,  56,  58,  60,  62,  64,  66,
  208,   6,   8,  10,  12,  14,  16,  18,  20,  22,  24,  26,  28,  30,  32,  34,
   36,  38,  40,  42,  44,  46,  48,  50,  52,  54,  56, 208, 208, 208, 208, 208
};

uint16_t address_for(uint8_t ascii)
{
  if (ascii < 32 || ascii >= 128) {
    return 208;
  }
  uint16_t address = pgm_read_byte_near(AddressTable + ascii - 32);
  return address;
}
