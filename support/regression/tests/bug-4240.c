/* bug-4240.c
   Loop reversal turned a counting-up loop into a counting-down one although
   the body used the loop variable: isConformingBody() only looked for the
   variable as a direct operand of the operators it lists, so a use through
   a cast or as the condition of an if statement went unnoticed.
 */

#include <testfwk.h>

#include <stdint.h>

static uint8_t seq, k, n, m;

static uint8_t
get (void)
{
  return seq++;
}

static uint8_t
withCall (void)
{
  static uint8_t i;
  uint8_t bad = 0;

  for (i = 0; i < 15; i++)
    if (get () != (uint8_t)i)
      bad++;
  return bad;
}

static void
withCast (void)
{
  static uint8_t i;

  for (i = 0; i < 15; i++)
    if ((uint8_t)i == k)
      k++;
}

static void
asCondition (void)
{
  uint8_t i;

  for (i = 0; i < 10; i++)
    if (i)
      n++;
}

static void
castToWider (void)
{
  uint8_t i;

  for (i = 0; i < 10; i++)
    if ((unsigned int)i == m)
      m += 2;
}

void
testBug (void)
{
  seq = 0;
  k = 0;
  n = 0;
  m = 0;

  ASSERT (withCall () == 0);

  withCast ();
  ASSERT (k == 15);

  asCondition ();
  ASSERT (n == 9);

  castToWider ();
  ASSERT (m == 10);
}
