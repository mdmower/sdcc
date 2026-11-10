/* bug-4145.c
   The compiler crashed on expressions combining run-time values with C23
   constexpr objects, or with compound literals that have a constant
   initializer (which are handled the same way): an expression computed from
   such an object inherited the object's internal constexpr marker through
   its type, and was then taken for the object itself when the object's
   initializer was looked up.
 */

#include <testfwk.h>

#ifdef __SDCC
#pragma std_c23
#elif !defined (__STDC_VERSION__) || __STDC_VERSION__ < 202311L
#define SKIP_C23
#endif

static int calls;

static int
count (void)
{
  return ++calls;
}

static int
literalSum (int x)
{
  return x + (int){3};
}

static int
literalDifference (int x)
{
  return (int){3} - x;
}

static int
literalChoice (int x)
{
  return x ? 4 : (int){3};
}

static int
literalNegated (int x)
{
  return -(x + (int){3});
}

static int
literalComma (void)
{
  return (count (), (int){3});
}

static long
literalLong (long x)
{
  return x + (long){70000};
}

#ifndef SKIP_C23
constexpr int gc = 5;

static int
object (unsigned char op, int x)
{
  constexpr int c = 3;
  constexpr int d = 4;

  switch (op)
    {
    case 0:
      return x + c;
    case 1:
      return c + x;
    case 2:
      return c - x;
    case 3:
      return x * c;
    case 4:
      return c / x;
    case 5:
      return x % c;
    case 6:
      return c << x;
    case 7:
      return x | c;
    case 8:
      return x ^ c;
    case 9:
      return (x + c) * 2;
    case 10:
      return -(x + c);
    case 11:
      return !(x + c);
    case 12:
      return x ? 4 : c;
    case 13:
      return x ? c : d;
    case 14:
      return x ? c : x;
    case 15:
      return x + gc;
    case 16:
      return x < c;
    default:
      return (count (), c);
    }
}

static long
longObject (long x)
{
  constexpr long c = 70000;
  return x + c;
}

static char
element (const char *a, int x)
{
  constexpr int c = 3;
  return a[x + c];
}

static int
stored (int x)
{
  constexpr int c = 3;
  int y;

  y = (x + c) + c;
  return y;
}

static int
inferred (int x)
{
  constexpr int c = 3;
  auto y = x + c;
  typeof (x + c) z = x;

  y++;
  z += c;
  return y + z;
}
#endif

void
testBug (void)
{
  ASSERT (literalSum (5) == 8);
  ASSERT (literalDifference (5) == -2);
  ASSERT (literalChoice (5) == 4);
  ASSERT (literalChoice (0) == 3);
  ASSERT (literalNegated (5) == -8);
  calls = 0;
  ASSERT (literalComma () == 3);
  ASSERT (calls == 1);
  ASSERT (literalLong (-1) == 69999);

#ifndef SKIP_C23
  ASSERT (object (0, 5) == 8);
  ASSERT (object (1, -5) == -2);
  ASSERT (object (2, 5) == -2);
  ASSERT (object (3, 5) == 15);
  ASSERT (object (4, 2) == 1);
  ASSERT (object (5, 5) == 2);
  ASSERT (object (6, 4) == 48);
  ASSERT (object (7, 4) == 7);
  ASSERT (object (8, 5) == 6);
  ASSERT (object (9, 5) == 16);
  ASSERT (object (10, 5) == -8);
  ASSERT (object (11, 5) == 0);
  ASSERT (object (11, -3) == 1);
  ASSERT (object (12, 5) == 4);
  ASSERT (object (12, 0) == 3);
  ASSERT (object (13, 5) == 3);
  ASSERT (object (13, 0) == 4);
  ASSERT (object (14, 5) == 3);
  ASSERT (object (14, 0) == 0);
  ASSERT (object (15, 5) == 10);
  ASSERT (object (16, 2) == 1);
  ASSERT (object (16, 3) == 0);
  calls = 0;
  ASSERT (object (17, 5) == 3);
  ASSERT (calls == 1);

  ASSERT (longObject (-1) == 69999);
  ASSERT (element ("abcdef", 1) == 'e');
  ASSERT (stored (5) == 11);
  ASSERT (inferred (5) == 17);
#endif
}
