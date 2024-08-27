#ifndef __CONSTANTS__
#define __CONSTANTS__

#define boundary_address (2 * (2 + '&'))
#define current_lineno_address (2 * (2 + '#'))
#define linebuffer_address 0x88
#define memorylimit_address (2 * (2 + '*'))
#define numberbuffer_address 0x82
#define programtop_address (2 * (2 + '='))
#define random_address (2 * (2 + '\''))
#define remainder_address (2 * (2 + '%'))
#define saved_lineno_address (2 * (2 + '!'))

#define BS ('H' - '@')
#define CR '\r'
#define LF '\n'
#define CANCEL ('U' - '@')
#define SPACE ' '
#define BREAK ('C' - '@')

#endif
