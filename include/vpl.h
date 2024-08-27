#ifndef __VPL__
#define __VPL__

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

  void do_onecycle();
  void setup_execution(uint16_t program_top, uint16_t memory_size);
  void break_execution();
  void raise_error(const char *str);
  void set_randomseed(uint16_t value);

#ifdef __cplusplus
};
#endif

#endif
