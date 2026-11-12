/* struct-return-funcattr.c
   Functions that return a struct, have a non-struct first parameter, and
   have a function attribute after the parameter list, such as __reentrant
   or __using. For mcs51 these parameters are passed on the stack, and the
   front end must not keep the register parameter mark that it sets before
   the return type is known.
   __critical is not tested here, because __critical functions that return
   a struct return a wrong value for mcs51 and z80 for another reason.
 */

#include <testfwk.h>

#ifdef __SDCC_mcs51
#define USING0 __using (0)
#else
#define USING0
#endif

#if !defined(__SDCC_hc08) && !defined(__SDCC_s08) && !defined(SDCC_MOS) // struct return not yet supported
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

/* Declared here and defined after the calls, so that the calls are compiled
   from the declarations. */
struct s late (unsigned char x) __reentrant;
struct s lateBank (unsigned char) USING0;
struct s (*fp) (unsigned char x) __reentrant;

struct s
reent (unsigned char x) __reentrant
{
  struct s r;
  r.a = x;
  r.b = 0x1200 + x;
  r.c = x ^ 0xff;
  return r;
}

struct s
bank (unsigned char x) USING0
{
  struct s r;
  r.a = x;
  r.b = 0x3400 + x;
  r.c = x ^ 0xff;
  return r;
}

struct s
two (unsigned char x, int y) __reentrant
{
  struct s r;
  r.a = x;
  r.b = y;
  r.c = x ^ 0xff;
  return r;
}

union u
un (long x) __reentrant
{
  union u r;
  r.b = x + 1;
  return r;
}

/* The return type is a pointer, which does not change how the parameter
   is passed. */
struct s *
ptr (struct s *p) __reentrant
{
  p->a++;
  return p;
}
#endif

void
testDefinition (void)
{
#if !defined(__SDCC_hc08) && !defined(__SDCC_s08) && !defined(SDCC_MOS) // struct return not yet supported
  struct s r;
  union u v;

  r = reent (0x11);
  ASSERT (r.a == 0x11);
  ASSERT (r.b == 0x1211);
  ASSERT (r.c == 0xee);

  r = bank (0x31);
  ASSERT (r.a == 0x31);
  ASSERT (r.b == 0x3431);
  ASSERT (r.c == 0xce);

  r = two (0x41, 0x4567);
  ASSERT (r.a == 0x41);
  ASSERT (r.b == 0x4567);
  ASSERT (r.c == 0xbe);

  v = un (0x12345677);
  ASSERT (v.b == 0x12345678);

  ASSERT (ptr (&r)->a == 0x42);
#endif
}

void
testDeclaration (void)
{
#if !defined(__SDCC_hc08) && !defined(__SDCC_s08) && !defined(SDCC_MOS) // struct return not yet supported
  struct s r;

  r = late (0x51);
  ASSERT (r.a == 0x51);
  ASSERT (r.b == 0x5651);
  ASSERT (r.c == 0xae);

  r = lateBank (0x71);
  ASSERT (r.a == 0x71);
  ASSERT (r.b == 0x7871);
  ASSERT (r.c == 0x8e);

  fp = reent;
  r = fp (0x61);
  ASSERT (r.a == 0x61);
  ASSERT (r.b == 0x1261);
  ASSERT (r.c == 0x9e);
#endif
}

#if !defined(__SDCC_hc08) && !defined(__SDCC_s08) && !defined(SDCC_MOS) // struct return not yet supported
struct s
late (unsigned char x) __reentrant
{
  struct s r;
  r.a = x;
  r.b = 0x5600 + x;
  r.c = x ^ 0xff;
  return r;
}

struct s
lateBank (unsigned char x) USING0
{
  struct s r;
  r.a = x;
  r.b = 0x7800 + x;
  r.c = x ^ 0xff;
  return r;
}
#endif
