/* jump-range.c
   mcs51 assembler error "Branching Range Exceeded". The peephole optimizer
   replaced a 2-byte sjmp to a label at which an ljmp is by a 3-byte ljmp to
   the target of that ljmp. It had accepted a short jump across such places
   for the smaller code, and the jump was then out of range.
 */

#include <testfwk.h>

__data unsigned char last;
__data volatile unsigned char pad;
unsigned char calls;

static void
fn (void)
{
  calls++;
}

#define CASE(n) case n: last = n; break;
#define CALL8 fn (); fn (); fn (); fn (); fn (); fn (); fn (); fn ();

/* Each break jumps to the return statement, and the return statement is a
   jump to the end of the function. The 40 calls put the end of the function
   out of the range of a short jump. The parameter on the stack gives the
   function more than a ret instruction at its end. The jump at the end of
   the loop goes back across the breaks. The loop is about 120 bytes long on
   mcs51, and PAD adds 3 bytes per statement. */
#define FIND(name, PAD) \
static void \
name (unsigned char x, unsigned char other) __reentrant \
{ \
  do \
    { \
      switch (x) \
        { \
        default: \
          x++; \
          PAD \
          continue; \
        CASE (3) CASE (13) CASE (23) CASE (33) CASE (43) CASE (53) \
        CASE (63) CASE (73) CASE (83) CASE (93) CASE (103) \
        } \
      return; \
    } \
  while (x); \
  CALL8 CALL8 CALL8 CALL8 CALL8 \
  last = other; \
}

/* The jump was out of range for 6 consecutive loop lengths, so the three
   loops are 6 bytes apart. */
FIND (find0, )
FIND (find2, pad = 1; pad = 2;)
FIND (find4, pad = 1; pad = 2; pad = 3; pad = 4;)

/* 43 is a case value, 103 is the first case value from 94 upward, and the
   values from 104 to 255 are not case values. */
#define CHECK(find) \
  do \
    { \
      last = 0; \
      calls = 0; \
      find (43, 7); \
      ASSERT (last == 43); \
      find (94, 7); \
      ASSERT (last == 103); \
      ASSERT (calls == 0); \
      find (104, 7); \
      ASSERT (last == 7); \
      ASSERT (calls == 40); \
    } \
  while (0)

void
testBug (void)
{
  CHECK (find0);
  CHECK (find2);
  CHECK (find4);
}
