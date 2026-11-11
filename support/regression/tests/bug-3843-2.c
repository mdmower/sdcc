/* bug-3843-2.c
   mcs51 assembler error "Branching Range Exceeded". The peephole optimizer
   replaces the ljmp instructions of a jump table by sjmp when every case
   label is in range. It allowed a label before the table the distance that
   it allows a label after the table, and more for each later entry of the
   table, although later entries are farther from a label before the table.
 */

#include <testfwk.h>

#ifdef __SDCC_mcs51
#define NOP __asm nop __endasm
#else
#define NOP
#endif

#define NOP10 NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP
#define NOP50 NOP10; NOP10; NOP10; NOP10; NOP10

/* The label of case 6 is the top of the loop. With 110 bytes of nop between
   the top of the loop and the table, an sjmp in the last entry of the table
   cannot reach that label. */
static unsigned char
loop (unsigned char x)
{
  unsigned char r = 0;

  while (1)
    {
      x >>= 1;
      NOP50;
      NOP50;
      NOP10;
      switch (x)
        {
        case 0: r += 1; break;
        case 1: r += 2; break;
        case 2: r += 4; break;
        case 3: r += 8; break;
        case 4: r += 16; break;
        case 5: r += 32; break;
        case 6: continue;
        }
      break;
    }
  return r;
}

void
testBug (void)
{
  ASSERT (loop (0) == 1);
  ASSERT (loop (2) == 2);
  ASSERT (loop (5) == 4);
  ASSERT (loop (7) == 8);
  ASSERT (loop (9) == 16);
  ASSERT (loop (11) == 32);
  ASSERT (loop (12) == 8); /* case 6, then case 3 */
  ASSERT (loop (13) == 8);
  ASSERT (loop (14) == 0);
  ASSERT (loop (255) == 0);
}
