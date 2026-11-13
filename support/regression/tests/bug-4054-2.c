/* bug-4054-2.c
   A struct or union argument that is the target of a pointer, as in f (*p),
   was read through twice, so that the first bytes of the object were taken
   for its address. With an index, as in f (p[i]), mcs51 rejected the call
   (error 129). The value of an assignment to such a target, as in
   x = *p = y, was read through twice too. For a function that is not
   reentrant, mcs51 also copied the argument from the wrong address space
   when the address was a pointer that is not generic or the address of the
   value of a call.
 */

#include <testfwk.h>

#include <stdarg.h>

struct s
{
  unsigned char a;
  int b;
};

struct w
{
  unsigned char k;
  struct s m[2];
};

union u
{
  unsigned char a;
  long b;
};

const struct s ca[3] = {{0x20, 0x2020}, {0x21, 0x2121}, {0x22, 0x2222}};
const struct s cm[2][2] = {{{0x30, 0x3030}, {0x31, 0x3131}}, {{0x32, 0x3232}, {0x33, 0x3333}}};
const struct w cw = {0x35, {{0x36, 0x3636}, {0x37, 0x3737}}};
struct s rs;
struct s rd;
struct s ra[2];
unsigned char cnt;

/* the members of v are a and a * 0x101 */
unsigned char
is (struct s v, unsigned char a)
{
  return v.a == a && v.b == a * 0x101;
}

unsigned char
mid (unsigned char x, struct s v, unsigned char y)
{
  return v.a == x && v.b == x * 0x101 && y == 0x99;
}

unsigned char
two (struct s v, struct s w, unsigned char a)
{
  return v.a == a && v.b == a * 0x101 && w.a == a + 1 && w.b == (a + 1) * 0x101;
}

unsigned char
isUnion (union u v)
{
  return v.b == 0x12345678;
}

unsigned char
param (const struct s *p)
{
  return is (*p, 0x21) && mid (0x21, *p, 0x99);
}

unsigned char
variadic (unsigned char a, ...)
{
  va_list ap;
  unsigned char r;

  va_start (ap, a);
  r = is (va_arg (ap, struct s), a);
  va_end (ap);
  return r;
}

void
testBug (void)
{
  const struct s *p = &ca[1];
  const struct s **pp = &p;
  const struct s (*pm)[2] = cm;
  const struct w *pw = &cw;
  struct s *q = &rs;
  union u x;
  union u *px = &x;
  unsigned char i = 1;

  rs.a = 0x41;
  rs.b = 0x4141;
  ra[0].a = 0x50;
  ra[0].b = 0x5050;
  ra[1].a = 0x51;
  ra[1].b = 0x5151;
  cnt = 0;

  ASSERT (is (*p, 0x21));
  ASSERT (is (*q, 0x41));
  ASSERT (is (**pp, 0x21));
  ASSERT (is (*(p + i), 0x22));
  ASSERT (is (*(p - 1), 0x20));
  ASSERT (is ((cnt++, *p), 0x21));
  ASSERT (cnt == 1);
  ASSERT (is (*p++, 0x21));
  ASSERT (p == &ca[2]);
  ASSERT (is (*--p, 0x21));
  ASSERT (p == &ca[1]);
  ASSERT (mid (0x41, *q, 0x99));
  ASSERT (two (*(p - 1), *p, 0x20));
  ASSERT (two (ca[1], *(p + i), 0x21));
  ASSERT (param (&ca[1]));
  ASSERT (variadic (0x41, *q));
  ASSERT (variadic (0x22, p[i]));

  /* with an index */
  ASSERT (is (p[i], 0x22));
  ASSERT (is (p[0], 0x21));
  ASSERT (is (ca[i], 0x21));
  ASSERT (is (ra[i], 0x51));
  ASSERT (is (cm[i][0], 0x32));
  ASSERT (is (pm[1][i], 0x33));
  ASSERT (is ((*pm)[i], 0x31));
  ASSERT (is (cw.m[i], 0x37));
  ASSERT (is (pw->m[i], 0x37));
  ASSERT (is (pw->m[0], 0x36));
  q = ra;
  ASSERT (is (q[i], 0x51));
  ASSERT (two (q[0], q[i], 0x50));

  x.b = 0x12345678;
  ASSERT (isUnion (*px));
}

/* the value of an assignment to the target of a pointer */
void
testAssign (void)
{
  struct s *q = ra;
  unsigned char i = 1;

  rs.a = 0x44;
  rs.b = 0x4444;

  rd = ra[0] = rs;
  ASSERT (is (ra[0], 0x44));
  ASSERT (is (rd, 0x44));

  rd = q[i] = ca[0];
  ASSERT (is (ra[1], 0x20));
  ASSERT (is (rd, 0x20));

  rd = *q = ca[2];
  ASSERT (is (ra[0], 0x22));
  ASSERT (is (rd, 0x22));
}

#ifdef __SDCC_mcs51
__xdata struct s xs;
__data struct s ds;
__idata struct s ids;
struct s __xdata *xp;
struct s __data *dp;
struct s __idata *ip;
const struct s __code *cp;
#endif

/* pointers that are not generic */
void
testSpace (void)
{
#ifdef __SDCC_mcs51
  xs.a = 0x61;
  xs.b = 0x6161;
  ds.a = 0x62;
  ds.b = 0x6262;
  ids.a = 0x63;
  ids.b = 0x6363;
  xp = &xs;
  dp = &ds;
  ip = &ids;
  cp = &ca[1];

  ASSERT (is (*xp, 0x61));
  ASSERT (is (*dp, 0x62));
  ASSERT (is (*ip, 0x63));
  ASSERT (is (*cp, 0x21));
  ASSERT (is (xp[0], 0x61));
  ASSERT (is (cp[1], 0x22));
  ASSERT (is (*(cp - 1), 0x20));
  ASSERT (is (*(struct s *)xp, 0x61));
  ASSERT (mid (0x62, *dp, 0x99));
  ASSERT (two (*dp, *ip, 0x62));
  ASSERT (two (*(cp - 1), *cp, 0x20));
#endif
}

#if !defined(__SDCC_hc08) && !defined(__SDCC_s08) && !defined(SDCC_MOS) // struct return not yet supported
struct s
get (unsigned char a)
{
  struct s r;

  r.a = a;
  r.b = a * 0x101;
  return r;
}
#endif

/* the value of a call */
void
testCall (void)
{
#if !defined(__SDCC_hc08) && !defined(__SDCC_s08) && !defined(SDCC_MOS) // struct return not yet supported
  rs.a = 0x42;
  rs.b = 0x4242;

  ASSERT (is (get (0x71), 0x71));
  ASSERT (mid (0x72, get (0x72), 0x99));
  ASSERT (two (get (0x73), get (0x74), 0x73));
  ASSERT (two (get (0x41), rs, 0x41));
  ASSERT (two (rs, get (0x43), 0x42));
#endif
}
