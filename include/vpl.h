#ifndef __VPL__
#define __VPL__

#include "common.h"

enum Error {None, ZERODIV, OUTOFMEMORY};

#ifdef __cplusplus
extern "C" {
#endif

  void do_onecycle();
  void setup_execution(uint16_t program_top, uint16_t memory_size);
  void break_execution();
  void raise_error(enum Error error);
  void set_randomseed(uint16_t value);

#ifdef __cplusplus
};
#endif

#endif
