/* bug-4248.c
   A compound literal whose address is passed to a function in the
   initializer of a declaration was initialized a second time after the call,
   overwriting what the function had stored in it.
 */

#include <testfwk.h>

#ifdef __SDCC
#pragma std_c99
#endif

static unsigned *
init (unsigned *p)
{
  *p = 42;
  return p;
}

static unsigned
test (void)
{
  unsigned *const p = init (&(unsigned){0});

  return *p;
}

void
testBug (void)
{
  ASSERT (test () == 42);
}
