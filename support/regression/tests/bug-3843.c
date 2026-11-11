/* bug-3843.c
   mcs51 assembler error "Branching Range Exceeded". The peephole optimizer
   counted no bytes for an instruction when a comment followed the mnemonic
   without a space, as in the inline assembler line "nop; nop; nop;". It then
   used a short jump over a distance that was out of range.
 */

#include <testfwk.h>

#ifdef __SDCC_mcs51
/* The assembler reads everything after the first semicolon as a comment,
   so this is one nop. */
#define NOP __asm nop; nop; nop; __endasm
#else
#define NOP
#endif

#define NOP10 NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP
#define NOP70 NOP10; NOP10; NOP10; NOP10; NOP10; NOP10; NOP10

/* 140 bytes of nop are between each jump and its label. */

static unsigned char
backward (unsigned char n)
{
  unsigned char r = 0;

  do
    {
      r += 2;
      NOP70;
      NOP70;
    }
  while (--n);
  return r;
}

static unsigned char
forward (unsigned char c)
{
  unsigned char r = 1;

  if (c)
    {
      NOP70;
      NOP70;
      r = 2;
    }
  return r;
}

void
testBug (void)
{
  ASSERT (backward (3) == 6);
  ASSERT (forward (0) == 1);
  ASSERT (forward (1) == 2);
}
