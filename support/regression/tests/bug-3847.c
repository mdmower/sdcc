/* bug-3847.c
   A function could return a struct or union only from a variable. Returning
   the value of a call directly gave an internal error in the code generator
   (z80) or silently left the caller's object unwritten (mcs51).
 */

#include <testfwk.h>

#if !defined(__SDCC_hc08) && !defined(__SDCC_s08) && !defined(SDCC_MOS) // struct return not yet supported
struct X
{
  int k;
};

struct s
{
  unsigned char a;
  int b;
  unsigned char c;
};

union u
{
  unsigned char a;
  long b;
};

struct X
fx (int i)
{
  struct X r;
  r.k = i + 1;
  return r;
}

struct X
f (void)
{
  return fx (1);
}

struct s
mk (unsigned char x)
{
  struct s r;
  r.a = x;
  r.b = 0x1200 + x;
  r.c = x ^ 0xff;
  return r;
}

struct s
mk7 (void)
{
  return mk (7);
}

struct s
fwd (unsigned char x)
{
  return mk (x);
}

struct s
chain (unsigned char x)
{
  return fwd (x + 1);
}

struct s
nested (unsigned char x)
{
  return mk (mk (x).a + 2);
}

struct s
two (unsigned char c)
{
  if (c)
    return mk (0x21);
  return mk (0x22);
}

struct s
viaPointer (struct s (*fp) (void))
{
  return fp ();
}

union u
mku (long x)
{
  union u r;
  r.b = x;
  return r;
}

union u
fwdu (long x)
{
  return mku (x + 1);
}
#endif

void
testBug (void)
{
#if !defined(__SDCC_hc08) && !defined(__SDCC_s08) && !defined(SDCC_MOS) // struct return not yet supported
  struct X x;
  struct s r;
  union u v;

  x.k = 0;
  x = f ();
  ASSERT (x.k == 2);

  r = mk7 ();
  ASSERT (r.a == 7);
  ASSERT (r.b == 0x1207);
  ASSERT (r.c == 0xf8);

  r = fwd (0x31);
  ASSERT (r.a == 0x31);
  ASSERT (r.b == 0x1231);
  ASSERT (r.c == 0xce);

  r = chain (0x40);
  ASSERT (r.a == 0x41);
  ASSERT (r.b == 0x1241);
  ASSERT (r.c == 0xbe);

  r = nested (0x50);
  ASSERT (r.a == 0x52);
  ASSERT (r.b == 0x1252);
  ASSERT (r.c == 0xad);

  r = two (1);
  ASSERT (r.a == 0x21);
  ASSERT (r.b == 0x1221);
  r = two (0);
  ASSERT (r.a == 0x22);
  ASSERT (r.b == 0x1222);

  r = viaPointer (mk7);
  ASSERT (r.a == 7);
  ASSERT (r.b == 0x1207);
  ASSERT (r.c == 0xf8);

  ASSERT (fwd (0x11).b == 0x1211);

  v = fwdu (0x12345677);
  ASSERT (v.b == 0x12345678);
#endif
}
