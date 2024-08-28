#ifndef __CONTEXT__
#define __CONTEXT__

#include "common.h"

typedef struct {
  uint16_t position;
} Context;

uint8_t get_byte_context(Context *context);
void set_context(uint16_t position, Context *context);
void inc_context(Context *context);
void dec_context(Context *context);

#endif
