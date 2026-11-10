/* bug-4158.c
   Returning the same struct variable from two return statements gave wrong
   values from the second one for mcs51 with --model-large: the operand of the
   first return was never freed, so the second return reused its stale pointer
   state instead of loading the address again.
 */

#include <testfwk.h>

#if !defined(__SDCC_hc08) && !defined(__SDCC_s08) && !defined(SDCC_MOS) // struct return not yet supported
struct s
{
  unsigned char a, b;
};

struct s
mk (unsigned char x)
{
  struct s v;
  v.a = x;
  v.b = x + 1;
  return v;
}

struct s
twice (unsigned char c, unsigned char x)
{
  struct s r = mk (x);
  if (c)
    return r;
  return r;
}

struct s
thrice (unsigned char c, unsigned char x)
{
  struct s r = mk (x);
  if (c == 0)
    return r;
  if (c == 1)
    return r;
  return r;
}
#endif

void
testBug (void)
{
#if !defined(__SDCC_hc08) && !defined(__SDCC_s08) && !defined(SDCC_MOS) // struct return not yet supported
  struct s r;

  r = twice (1, 0x10);
  ASSERT (r.a == 0x10);
  ASSERT (r.b == 0x11);
  r = twice (0, 0x20);
  ASSERT (r.a == 0x20);
  ASSERT (r.b == 0x21);

  r = thrice (0, 0x30);
  ASSERT (r.a == 0x30);
  ASSERT (r.b == 0x31);
  r = thrice (1, 0x40);
  ASSERT (r.a == 0x40);
  ASSERT (r.b == 0x41);
  r = thrice (2, 0x50);
  ASSERT (r.a == 0x50);
  ASSERT (r.b == 0x51);
#endif
}
