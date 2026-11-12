/* bug-4150.c
   The declaration in the first clause of a for statement was added to the
   symbol table after the controlling expression and the iteration
   expression had been parsed. The parser looks up the operand of &* and of
   typeof during the parse. In these two expressions it reported the loop
   variable as undefined, or used an object of the same name from an
   enclosing scope in its place. The declaration in the header of an if or
   switch statement (C2y) was added after the expression of the header.
 */

#include <testfwk.h>

#ifdef __SDCC
#pragma std_c2y
#elif !defined (__STDC_VERSION__) || __STDC_VERSION__ < 202311L
#define SKIP_C23
#endif

static unsigned char buf[3];

/* The loop variables of the tests hide these. */
static unsigned char *p = buf + 2;
static long i;
typedef int T;

static inline unsigned char *
next (unsigned char *q)
{
  return q + 1;
}

/* Each loop also ends at n == 10, which bounds it when its condition or
   its iteration expression reads the wrong object. */

void
testCall (void)
{
  unsigned char n = 0;

  for (unsigned char *r = buf; r != buf + 3 && n < 10; r = next (&*r))
    n++;
  ASSERT (n == 3);
}

void
testIteration (void)
{
  unsigned char n = 0;

  for (unsigned char *p = buf; p != buf + 3 && n < 10; p = &*p + 1)
    n++;
  ASSERT (n == 3);
  ASSERT (p == buf + 2);
}

void
testCondition (void)
{
  unsigned char n = 0;

  for (unsigned char *p = buf; &*p != buf + 3 && n < 10; p++)
    n++;
  ASSERT (n == 3);
  ASSERT (p == buf + 2);
}

void
testTypeof (void)
{
#ifndef SKIP_C23
  unsigned char n = 0;

  for (unsigned char i = 0; i < sizeof (typeof (i)) && n < 10; i += (typeof (i))1)
    n++;
  ASSERT (n == 1);
  ASSERT (sizeof (i) == sizeof (long));
#endif
}

void
testTypedefName (void)
{
  unsigned char n = 0;

  /* T is the loop variable, and (T) does not start a cast. */
  for (unsigned char T = 0; (T) < 3 && n < 10; T++)
    n++;
  ASSERT (n == 3);
  ASSERT (sizeof (T) == sizeof (int));
}

void
testSelection (void)
{
#ifdef __SDCC
  unsigned char n = 0;

  if (unsigned char *p = buf; &*p == buf)
    n++;
  switch (unsigned char *p = buf; &*p == buf)
    {
    case 1:
      n++;
    }
  ASSERT (n == 2);
  ASSERT (p == buf + 2);
#endif
}
