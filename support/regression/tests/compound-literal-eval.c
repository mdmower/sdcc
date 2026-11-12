/* compound-literal-eval.c
   The object of a compound literal in a function body gets its value each
   time the literal is evaluated, and only then.
 */

#include <testfwk.h>

#ifdef __SDCC
#pragma std_c99
#endif

struct s
{
  char a;
  char b;
};

static unsigned char calls;

static int
count (int v)
{
  calls++;
  return v;
}

static int
deref (const int *p)
{
  return *p;
}

static char
sum (const struct s *p)
{
  return p->a + p->b;
}

/* Under a case label, where the start of the block is never executed. */
static int
underCase (unsigned char op, int x)
{
  int y = 0;

  switch (op)
    {
    case 0:
      y = x ? 4 : (int){3};
      break;
    case 1:
      y = (int){x};
      break;
    }
  return y;
}

/* The initializer names a variable that changes after the start of the block. */
static int
afterChange (int x)
{
  x = x + 1;
  return (int){x};
}

/* The literal is evaluated several times, in a loop body that is not a block. */
static int
inLoop (void)
{
  int s = 0;
  unsigned char i;

  for (i = 0; i < 3; i++)
    s += (int){i + 1};
  return s;
}

/* The object gets its value again each time. */
static int
eachTime (void)
{
  int s = 0;
  unsigned char i;

  for (i = 0; i < 3; i++)
    s += ++(int){10};
  return s;
}

/* Reached by a goto past the start of the block. */
static int
afterGoto (int x)
{
  int y;

  goto skip;
  y = 9;
skip:
  y = x ? 4 : (int){3};
  return y;
}

/* The address, a member, and an element of the literal. */
static int
asLvalue (char x)
{
  int r;

  x++;
  r = deref (&(int){x});
  r += (struct s){x, 2}.a;
  r += (char[]){1, x, 3}[1];
  r += sum (&(struct s){x, x});
  return r;
}

/* The initializer is evaluated only when the literal is. */
static int
conditional (unsigned char c)
{
  int r;

  calls = 0;
  r = c ? (int){count (5)} : 7;
  if (c && (int){count (1)})
    r++;
  r += c || (int){count (1)};
  return r;
}

/* In the controlling expressions of loops. */
static unsigned char
inControl (unsigned char n)
{
  unsigned char i = 0;
  unsigned char r = 0;

  while ((unsigned char){i} < n)
    i++;
  r += i;
  for (i = 0; i < (unsigned char){n}; i += (unsigned char){r})
    r++;
  do
    r++;
  while ((unsigned char){r} < 12);
  return r;
}

/* After the left operand of a comma operator. */
static int
afterComma (int x)
{
  int y;

  y = (x = x + 5, (int){x});
  return y;
}

static struct s
retStruct (char x)
{
  x++;
  return (struct s){x, x + 1};
}

/* A literal in the initializer of another one. */
static int
nested (int x)
{
  x++;
  return (int){(int){x} + 1};
}

static inline int
inl (int x)
{
  x++;
  return (int){x};
}

void
testCase (void)
{
  ASSERT (underCase (0, 0) == 3);
  ASSERT (underCase (0, 1) == 4);
  ASSERT (underCase (1, 7) == 7);
  ASSERT (afterGoto (0) == 3);
}

void
testPoint (void)
{
  ASSERT (afterChange (1) == 2);
  ASSERT (inLoop () == 6);
  ASSERT (eachTime () == 33);
  ASSERT (afterComma (1) == 6);
  ASSERT (nested (1) == 3);
  ASSERT (inl (1) == 2);
  ASSERT (inl (5) + inl (7) == 14);
}

void
testLvalue (void)
{
  struct s v;

  ASSERT (asLvalue (4) == 25);
  v = retStruct (2);
  ASSERT (v.a == 3);
  ASSERT (v.b == 4);
}

void
testEvaluated (void)
{
  ASSERT (conditional (0) == 8);
  ASSERT (calls == 1);
  ASSERT (conditional (1) == 7);
  ASSERT (calls == 2);
  ASSERT (inControl (3) == 12);
}
