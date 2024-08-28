#include "context.h"
#include "memory.h"

uint8_t get_byte_context(Context *context)
{
  return get_byte(context->position);
}

void set_context(uint16_t position, Context *context)
{
  context->position = position;
}

void inc_context(Context *context)
{
  context->position ++;
}

void dec_context(Context *context)
{
  context->position --;
}
