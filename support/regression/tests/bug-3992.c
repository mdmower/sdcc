/* bug-3992.c
   Objects of an enumerated type stayed in scope after the end of their
   block. A declaration of the same name in a later block, after a
   declaration that follows a statement, was rejected as a duplicate symbol.
 */

#include <testfwk.h>

typedef enum {a, b, c, d} Foo_t;
enum tag {p, q, r};

static unsigned char n;

/* The code of the bug report. */
static int
twoLoops (void)
{
  for (Foo_t i = a; i < d; i++)
    n++;

  int x = 0;
  for (Foo_t i = b; i < d; i++)
    x++;

  return x;
}

static int
twoBlocks (void)
{
  int i = 40;

  {
    enum tag i = r;
    n += i;
  }

  int x = i;
  {
    enum tag i = q;
    x += i;
  }
  {
    long i = 100;
    x += i;
  }

  return x + i;
}

void
testBug (void)
{
  n = 0;

  ASSERT (twoLoops () == 2);
  ASSERT (n == 3);

  ASSERT (twoBlocks () == 181);
  ASSERT (n == 5);
}
