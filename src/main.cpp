#include <Arduino.h>
#include "porting.h"
#include "constants.h"
#include "vpl.h"
#include "config.h"

#define is_break(c) (((c) == BREAK)||((c)==27))

extern int allocate_memory(uint16_t size);

void setup()
{
  Serial.begin(115200);
  Serial.print("VTL for Arduino\n");
  allocate_memory(memorysize);
  setup_execution(default_programtop, memorysize);
  set_randomseed(default_randomseed);
}

void loop()
{
  if (Serial.available()) {
    uint8_t c = Serial.read();
    if (is_break(c)) {
      break_execution();
    }
  }
  do_onecycle();
  delay(1);
}

uint8_t memory[memorysize];

int allocate_memory(uint16_t size)
{
  core = memory;
  return size;
}

uint8_t get_asciivalue()
{
  while (!Serial.available()) {
  }
  uint8_t c = Serial.read();
  if (is_break(c)) {
    break_execution();
    c = CR;
  }
  return c;
}

void put_asciivalue(uint8_t ascii)
{
  Serial.write(ascii);
}
