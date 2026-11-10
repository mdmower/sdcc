/* bug-2948.c
   An anonymous struct or union is a member like any other: its initializer
   may be enclosed in braces of its own, and when it is the first member of
   a union, the union is initialized through all of its members.

   The members of an anonymous struct or union are promoted into the
   enclosing one. A union then initialized only the first of them, and
   braces around the initializers of the promoted members were taken for
   braces around the initializer of the first one, so that the rest of the
   list went to the wrong members or was dropped.

   The constant objects are emitted as data, the automatic ones are
   initialized by code.
 */

#include <testfwk.h>

/* the union of gcc-torture-execute-pr87053.c */
union U
{
  struct
  {
    char x[3];
    char y[3];
  };
  struct
  {
    char z[6];
  };
};

union V
{
  struct
  {
    char a;
    int b;
    char c;
  };
  long l;
};

struct S
{
  char p;
  struct
  {
    char a;
    char b;
  };
  char q;
};

/* two anonymous structs that start at the same member */
struct N
{
  struct
  {
    struct
    {
      char a;
      char b;
    };
    char c;
  };
  char d;
};

/* an anonymous union that starts with an aggregate */
struct W
{
  char p;
  union
  {
    struct
    {
      char c;
      char d;
    } n;
    int b;
  };
  char q;
};

const union U cu = {{"ab", "c"}};
const union U cuy = {.y = "c"};
const union U cuz = {.z = "abcde"};
const union V cv = {{1, 0x0203, 4}};
const union V cve = {5, 0x0607, 8};       /* braces of the struct left out */
const struct S cs = {1, {2, 3}, 4};
const struct S csp = {1, {2}, 4};         /* b left out */
const struct S csd = {1, {.b = 3}, 4};
const struct N cn = {{{1, 2}, 3}, 4};
const struct N cnp = {{{1}, 3}, 4};
const struct N cnd = {{{.b = 2, .a = 1}, 3}, 4};
const struct W cw = {1, {{2, 3}}, 4};
const struct W cwe = {1, {2, 3}, 4};      /* braces of the union left out */

void
testConst (void)
{
  ASSERT (cu.x[0] == 'a' && cu.x[1] == 'b' && cu.x[2] == 0);
  ASSERT (cu.y[0] == 'c' && cu.y[1] == 0 && cu.y[2] == 0);
  ASSERT (cu.z[3] == 'c');
  ASSERT (cuy.x[0] == 0 && cuy.y[0] == 'c' && cuy.y[1] == 0);
  ASSERT (cuz.z[0] == 'a' && cuz.z[4] == 'e' && cuz.z[5] == 0);
  ASSERT (cuz.y[1] == 'e');

  ASSERT (cv.a == 1 && cv.b == 0x0203 && cv.c == 4);
  ASSERT (cve.a == 5 && cve.b == 0x0607 && cve.c == 8);

  ASSERT (cs.p == 1 && cs.a == 2 && cs.b == 3 && cs.q == 4);
  ASSERT (csp.p == 1 && csp.a == 2 && csp.b == 0 && csp.q == 4);
  ASSERT (csd.p == 1 && csd.a == 0 && csd.b == 3 && csd.q == 4);

  ASSERT (cn.a == 1 && cn.b == 2 && cn.c == 3 && cn.d == 4);
  ASSERT (cnp.a == 1 && cnp.b == 0 && cnp.c == 3 && cnp.d == 4);
  ASSERT (cnd.a == 1 && cnd.b == 2 && cnd.c == 3 && cnd.d == 4);

  ASSERT (cw.p == 1 && cw.n.c == 2 && cw.n.d == 3 && cw.q == 4);
  ASSERT (cwe.p == 1 && cwe.n.c == 2 && cwe.n.d == 3 && cwe.q == 4);
}

void
testAuto (void)
{
  union U u = {{"ab", "c"}};
  union V ve = {5, 0x0607, 8};
  struct S s = {1, {2, 3}, 4};
  struct S sd = {1, {.b = 3}, 4};
  struct N n = {{{1, 2}, 3}, 4};
  struct N nd = {{{.b = 2, .a = 1}, 3}, 4};
  struct W we = {1, {2, 3}, 4};
  unsigned char i;

  ASSERT (u.x[0] == 'a' && u.x[1] == 'b' && u.x[2] == 0);
  ASSERT (u.y[0] == 'c' && u.y[1] == 0 && u.y[2] == 0);
  ASSERT (u.z[3] == 'c');

  ASSERT (ve.a == 5 && ve.b == 0x0607 && ve.c == 8);

  ASSERT (s.p == 1 && s.a == 2 && s.b == 3 && s.q == 4);
  ASSERT (sd.p == 1 && sd.a == 0 && sd.b == 3 && sd.q == 4);

  ASSERT (n.a == 1 && n.b == 2 && n.c == 3 && n.d == 4);
  ASSERT (nd.a == 1 && nd.b == 2 && nd.c == 3 && nd.d == 4);

  ASSERT (we.p == 1 && we.n.c == 2 && we.n.d == 3 && we.q == 4);

  /* a member left out is zero, also the second time round */
  for (i = 0; i < 2; i++)
    {
      struct S sp = {i, {2}, 4};
      union V vp = {{1, i}};

      ASSERT (sp.p == i && sp.a == 2 && sp.b == 0 && sp.q == 4);
      ASSERT (vp.a == 1 && vp.b == i && vp.c == 0);
      sp.b = 0x5a;
      vp.c = 0x5a;
    }
}
