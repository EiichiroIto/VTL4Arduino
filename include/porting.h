#ifndef __PORTING__
#define __PORTING__

#ifdef __cplusplus
extern "C" {
#endif

  extern uint8_t *core;
  uint16_t address_for(uint8_t ascii);
  uint8_t get_asciivalue();
  void put_asciivalue(uint8_t ascii);
  void put_message(const char *str);

#ifdef __cplusplus
};
#endif

#endif
