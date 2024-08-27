#ifndef __STATUS__
#define __STATUS__

#include "common.h"

typedef struct {
  enum {Prompted, Stepping} state;
  uint16_t top_of_line;
  uint8_t position;
} Status;

void setup_status(Status *status);
void prompted_status(Status *status);
void stepping_status(Status *status);

#endif
