/* bug-3849.c
   A function could return a struct or union only from a variable. Returning
   any other value, such as *p, a[i] or s.m, gave a segfault or an internal
   error in the code generator (sm83, z80) or silently left the caller's
   object unwritten (mcs51).
 */

#include <testfwk.h>

#if !defined(__SDCC_hc08) && !defined(__SDCC_s08) && !defined(SDCC_MOS) // struct return not yet supported
struct Test
{
  int f;
};

struct s
{
  unsigned char a;
  int b;
};

struct w
{
  unsigned char c;
  struct s m;
};

union u
{
  unsigned char a;
  int b;
};

const struct s gs = {0x11, 0x1111};
const struct s ga[3] = {{0x20, 0x2020}, {0x21, 0x2121}, {0x22, 0x2222}};
const struct w gw = {0x33, {0x34, 0x3434}};
unsigned char cnt;

struct Test
bitcast (int arg)
{
  return *((struct Test *)(&arg));
}

struct s
deref (const struct s *p)
{
  return *p;
}

struct s
element (unsigned char i)
{
  return ga[i];
}

struct s
member (void)
{
  return gw.m;
}

struct s
arrow (const struct w *p)
{
  return p->m;
}

struct s
comma (void)
{
  return (cnt++, gs);
}

struct s
assign (const struct s *p)
{
  struct s x;
  return x = *p;
}

struct s
many (unsigned char c, const struct s *p)
{
  if (c == 0)
    return *p;
  if (c == 1)
    return ga[2];
  if (c == 2)
    return deref (&ga[0]);
  return gw.m;
}

static inline struct s
inl (unsigned char x)
{
  struct s r;
  r.a = x;
  r.b = 0x5600 + x;
  return r;
}

struct s
inlined (unsigned char x)
{
  return inl (x + 1);
}

union u
derefu (union u *p)
{
  return *p;
}
#endif

void
testBug (void)
{
#if !defined(__SDCC_hc08) && !defined(__SDCC_s08) && !defined(SDCC_MOS) // struct return not yet supported
  static const unsigned char expect[4] = {0x11, 0x22, 0x20, 0x34};
  struct Test t;
  struct s r;
  union u v;
  unsigned char c;

  t.f = 0;
  t = bitcast (0x1357);
  ASSERT (t.f == 0x1357);

  r = deref (&gs);
  ASSERT (r.a == 0x11);
  ASSERT (r.b == 0x1111);

  r = element (1);
  ASSERT (r.a == 0x21);
  ASSERT (r.b == 0x2121);

  r = member ();
  ASSERT (r.a == 0x34);
  ASSERT (r.b == 0x3434);

  r = arrow (&gw);
  ASSERT (r.a == 0x34);
  ASSERT (r.b == 0x3434);

  r = comma ();
  ASSERT (r.a == 0x11);
  ASSERT (r.b == 0x1111);
  ASSERT (cnt == 1);

  r = assign (&ga[1]);
  ASSERT (r.a == 0x21);
  ASSERT (r.b == 0x2121);

  for (c = 0; c < 4; c++)
    {
      r = many (c, &gs);
      ASSERT (r.a == expect[c]);
      ASSERT (r.b == expect[c] * 0x101);
    }

  /* the returned object is the one the argument points to */
  r.a = 0x45;
  r.b = 0x4545;
  r = deref (&r);
  ASSERT (r.a == 0x45);
  ASSERT (r.b == 0x4545);

  r = inlined (0x70);
  ASSERT (r.a == 0x71);
  ASSERT (r.b == 0x5671);

  v.b = 0x1234;
  v = derefu (&v);
  ASSERT (v.b == 0x1234);
#endif
}
