/* bug-4157.c
   A compound literal of struct type whose first initializer is a constant
   was not returned from a function: the return statement took the literal
   for a constant expression and replaced it by the value of that initializer.
 */

#include <testfwk.h>

#ifdef __SDCC
#pragma std_c23
#elif !defined (__STDC_VERSION__) || __STDC_VERSION__ < 202311L
#define SKIP_C23
#endif

#if !defined(__SDCC_hc08) && !defined(__SDCC_s08) && !defined(SDCC_MOS) // struct return not yet supported
struct s
{
  unsigned char a, b;
};

struct t
{
  int i;
  unsigned char c;
};

volatile unsigned char src = 2;

unsigned char
f (void)
{
  return src;
}

struct s
constant_first (void)
{
  return (struct s){1, f ()};
}

struct s
call_first (void)
{
  return (struct s){f (), 1};
}

struct s
constants (void)
{
  return (struct s){3, 4};
}

struct t
wider (unsigned char c)
{
  if (c)
    return (struct t){0x1234, 0x56};
  return (struct t){0x789a, c};
}

#ifndef SKIP_C23
constexpr struct t ct = {0x2468, 0x7b};

struct t
object (void)
{
  return ct;
}
#endif
#endif

void
testBug (void)
{
#if !defined(__SDCC_hc08) && !defined(__SDCC_s08) && !defined(SDCC_MOS) // struct return not yet supported
  struct s r = {0, 0};
  struct t w;

  r = constant_first ();
  ASSERT (r.a == 1);
  ASSERT (r.b == 2);

  r = call_first ();
  ASSERT (r.a == 2);
  ASSERT (r.b == 1);

  r = constants ();
  ASSERT (r.a == 3);
  ASSERT (r.b == 4);

  w = wider (1);
  ASSERT (w.i == 0x1234);
  ASSERT (w.c == 0x56);
  w = wider (0);
  ASSERT (w.i == 0x789a);
  ASSERT (w.c == 0);

#ifndef SKIP_C23
  w = object ();
  ASSERT (w.i == 0x2468);
  ASSERT (w.c == 0x7b);
#endif
#endif
}
