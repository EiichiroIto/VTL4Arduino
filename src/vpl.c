#include "memory.h"
#include "context.h"
#include "status.h"
#include "porting.h"
#include "constants.h"
#include "vpl.h"

Status status;

uint16_t get_boundary()
{
  return get_word(boundary_address);
}

void set_boundary(uint16_t value)
{
  set_word(boundary_address, value);
}

uint16_t get_current_lineno()
{
  return get_word(current_lineno_address);
}

void set_current_lineno(uint16_t value)
{
  set_word(current_lineno_address, value);
}

uint16_t get_memorylimit()
{
  return get_word(memorylimit_address);
}

void set_memorylimit(uint16_t value)
{
  set_word(memorylimit_address, value);
}

uint16_t get_programtop()
{
  return get_word(programtop_address);
}

void set_programtop(uint16_t value)
{
  set_word(programtop_address, value);
}

uint16_t get_variable(uint8_t ascii)
{
  uint16_t address = address_for(ascii);
  return get_word(address);
}

void set_variable(uint8_t ascii, uint16_t value)
{
  uint16_t address = address_for(ascii);
  set_word(address, value);
}

/* input/output */

void put_crlf()
{
  put_asciivalue(CR);
  put_asciivalue(LF);
}

void put_until(uint8_t ascii, Context *context)
{
  while (1) {
    uint8_t c = get_byte_context(context);
    if (c == ascii) {
      break;
    }
    put_asciivalue(c);
    inc_context(context);
  }
  inc_context(context);
}

void put_number(uint16_t value)
{
  Context new_context;
  uint16_t position = numberbuffer_address + 5;
  set_byte(position, 0);
  do {
    uint8_t c = (value % 10) + '0';
    value /= 10;
    position --;
    set_byte(position, c);
  } while (value);
  new_context.position = position;
  put_until(0, &new_context);
}

void get_line(uint16_t address)
{
  uint16_t position = 0;
  while (1) {
    uint8_t c = get_asciivalue();
    if (c == BS) {
      if (position > 0) {
        position --;
        put_asciivalue(c);
      }
    }
    if (c == CR) {
      set_byte(address + position, 0);
      put_crlf();
      return;
    }
    if (c == CANCEL || position > 74) {
      put_crlf();
      position = 0;
    }
    if (c >= SPACE) {
      set_byte(address + position, c);
      put_asciivalue(c);
      position ++;
    }
  }
}

/* line access */

uint16_t next_line(uint16_t address)
{
  uint8_t c = 0;
  address += 2;
  do {
    c = get_byte(address ++);
  } while (c);
  return address;
}

uint16_t find_line()
{
  uint16_t boundary = get_boundary();
  uint16_t current_lineno = get_current_lineno();
  uint16_t position = get_programtop();
  while (position != boundary) {
    uint16_t lineno = get_word(position);
    if (lineno >= current_lineno) {
      return position;
    }
    position = next_line(position);
  }
  return 0;
}

uint16_t delete_lineno(uint16_t lineno)
{
  uint16_t cur, src, dst, boundary, tmp;

  set_current_lineno(lineno);
  cur = find_line();
  if (cur == 0) {
    return 0;
  }
  tmp = get_word(cur);
  if (tmp != lineno) {
    return cur;
  }
  src = next_line(cur);
	boundary = get_boundary();
  dst = cur;
  while (src != boundary) {
    copy_byte(src, dst);
    src ++;
    dst ++;
  }
  set_boundary(dst);
	return cur;
}

void entry_line(uint16_t src, uint16_t dst, uint16_t lineno)
{
  uint8_t c = 0;

  set_word(dst, lineno);
  dst += 2;
  do {
    c = copy_byte(src, dst);
    src ++;
    dst ++;
  } while (c);
}

int insert(uint16_t count, uint16_t address)
{
  uint16_t boundary = get_boundary();
  uint16_t limit = get_memorylimit();
  if (boundary + count >= limit) {
    return 0;
  }
  uint16_t src = boundary;
  boundary += count;
  set_boundary(boundary);
  while (src != address) {
    boundary --;
    src --;
    copy_byte(src, boundary);
  }
	return 1;
}

void entry_lineno(uint16_t address, uint16_t lineno)
{
  uint16_t count;
  uint16_t cur = delete_lineno(lineno);
  if (cur == 0) {
    cur = get_boundary();
  }
  if (get_byte(address) == 0) {
    return;
  }
  count = 3 + count_until(address, 0);
  if (!insert(count, cur)) {
    return;
  }
  entry_line(address, cur, lineno);
}

void set_randomseed(uint16_t value)
{
  set_word(random_address, value);
}

void update_random(uint16_t value)
{
  uint16_t tmp = get_word(random_address);
  tmp += value;
  tmp = ((tmp << 8) & 0xFF00) + ((tmp >> 8) & 0xFF);
  set_word(random_address, tmp);
}

/* parsing */

int decimal_context(Context *context, uint16_t *value)
{
  uint16_t number = 0;
  uint8_t c = get_byte_context(context);
  if (!is_digit(c)) {
    return 0;
  }
  while (is_digit(c)) {
    number *= 10;
    number += c - '0';
    inc_context(context);
    c = get_byte_context(context);
  }
  *value = number;
  return 1;
}

extern uint16_t factor_context(Context *context);
extern uint16_t term_context(uint16_t value, Context *context);

uint16_t expression_context(Context *context)
{
  uint8_t c;
  uint16_t value = factor_context(context);
  while ((c = get_byte_context(context)) != 0 && c != ')') {
    value = term_context(value, context);
  }
  inc_context(context);
  return value;
}

void variable_context(Context *context, uint8_t *pc, uint16_t *paddress)
{
	uint8_t c = get_byte_context(context);
  inc_context(context);
  if (c == ':') {
    uint16_t value = expression_context(context);
    *paddress = get_boundary() + (value * 2);
  } else {
    *paddress = address_for(c);
  }
  *pc = c;
}

uint16_t factor_context(Context *context)
{
  uint8_t c = get_byte_context(context);
  if (c == 0) {
    return 0;
  }
  uint16_t tmp;
  if (decimal_context(context, &tmp)) {
    return tmp;
  }
  c = get_byte_context(context);
  inc_context(context);
  if (c == '?') {
    tmp = linebuffer_address;
    get_line(tmp);
    Context new_context;
    new_context.position = tmp;
    return expression_context(&new_context);
  }
  if (c == '$') {
    return get_asciivalue();
  }
  if (c == '(') {
    return expression_context(context);
  }
  dec_context(context);
  variable_context(context, &c, &tmp);
  return get_word(tmp);
}

uint16_t term_context(uint16_t value1, Context *context)
{
  uint8_t c = get_byte_context(context);
  inc_context(context);
  uint16_t value2 = factor_context(context);
  if (c == '*') {
    return value1 * value2;
  }
  if (c == '+') {
    return value1 + value2;
  }
  if (c == '-') {
    return value1 - value2;
  }
  if (c == '/') {
    if (value2 == 0) {
      raise_error("ZERO DIVIDE");
      return 0;
    }
    set_word(remainder_address, value1 % value2);
    return value1 / value2;
  }
  if (c == '=') {
    return value1 == value2 ? 1 : 0;
  }
  if (c == '>') {
    return value1 >= value2 ? 1 : 0;
  }
  return value1 < value2 ? 1 : 0;
}

void statement_context(Context *context)
{
  uint8_t c, c2;
  uint16_t address, value;

  if (context->position == 0) {
    context->position = status.top_of_line + 3;
  }
  variable_context(context, &c, &address);
  inc_context(context);
	c2 = get_byte_context(context);
  if (c2 == '"') {
    inc_context(context);
    put_until('"', context);
  	c2 = get_byte_context(context);
    if (c2 == ';') {
      inc_context(context);
    } else {
      put_crlf();
    }
    return;
  }
	value = expression_context(context);
  if (c == '$') {
    put_asciivalue(value & 0xFF);
    return;
  }
  if (c == '?') {
    put_number(value);
    return;
  }
  set_word(address, value);
  update_random(value);
#ifdef DEBUG
  put_message("address=");
  put_number(address);
  put_message("value=");
  put_number(value);
  put_message("\n");
#endif
}

/* execution */

void setup_execution(uint16_t program_top, uint16_t memory_size)
{
  set_memorylimit(memory_size);
  set_programtop(program_top);
  set_boundary(program_top);
  status.state = Prompted;
}

void break_execution()
{
  put_crlf();
  put_message("BREAK");
  put_crlf();
  status.state = Prompted;
}

void raise_error(const char *str)
{
  put_message("ERROR: ");
  put_message(str);
  put_message("\n");
  status.state = Prompted;
}

int execute_once()
{
  uint16_t lineno, r2;
  Context new_contex;

  new_contex.position = status.position;
  statement_context(&new_contex);
  lineno = get_current_lineno();
	r2 = get_word(status.top_of_line);
  if (lineno == 0 || lineno == r2) {
    if (status.top_of_line == linebuffer_address) {
      return 1;
    }
    status.top_of_line = next_line(status.top_of_line);
    if (status.top_of_line >= get_boundary()) {
      return 1;
    }
  } else {
    lineno = get_word(status.top_of_line);
    set_word(saved_lineno_address, lineno + 1);
    status.top_of_line = find_line();
    if (status.top_of_line == 0) {
      return 1;
    }
  }
  status.position = 0;
  return 0;
}

void do_list()
{
  Context new_context;
  new_context.position = get_programtop();
  uint16_t boundary = get_boundary();
  while (new_context.position < boundary) {
    uint16_t lineno = get_word(new_context.position);
    put_number(lineno);
    inc_context(&new_context);
    inc_context(&new_context);
    put_until(0, &new_context);
    put_crlf();
  }
}

void do_onecycle()
{
  uint16_t position, lineno;
  Context new_context;
  if (status.state == Prompted) {
    //setup_();
    position = linebuffer_address + 2;
    get_line(position);
    new_context.position = position;
    if (decimal_context(&new_context, &lineno)) {
      if (lineno == 0) {
        do_list();
      } else {
        entry_lineno(new_context.position, lineno);
      }
      return;
    }
    status.position = position;
    position = linebuffer_address;
    set_word(position, 0);
    set_current_lineno(0);
    status.state = Stepping;
    status.top_of_line = position;
  }
  if (status.state == Stepping) {
    if (execute_once()) {
      put_crlf();
      put_asciivalue('O');
      put_asciivalue('K');
      put_crlf();
      status.state = Prompted;
    } else {
      lineno = get_word(status.top_of_line);
      set_current_lineno(lineno);
    }
    return;
  }
  raise_error("do_onecycle");
}
