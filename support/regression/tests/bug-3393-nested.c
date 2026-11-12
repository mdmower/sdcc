/* bug-3393-nested.c
   A designated initializer names a member of an alternative of an anonymous
   union, and the first alternative of that union has an anonymous union of
   its own with members after it.

   The members of anonymous structs and unions are promoted into the
   enclosing struct. In its member list, the members after the inner union
   separate the members of the other alternatives from the member that the
   outer union starts at. The data image of the object has the designated
   value, and zeros in the rest of the union.

   The constant objects are emitted as data, the automatic one is
   initialized by code.
 */

#include <testfwk.h>

/* the struct of the ticket */
struct W
{
  char a;
  union
  {
    struct
    {
      union
      {
        char p;
        struct
        {
          char x;
          char y;
        };
      };
      char r;
    };
    struct
    {
      char s;
      char t;
      char u;
    };
  };
  char z;
};

/* the object is a union itself */
union V
{
  struct
  {
    union
    {
      char p;
      char q;
    };
    char r;
  };
  char c[2];
};

/* the inner union is not the first member of its alternative */
struct Y
{
  union
  {
    struct
    {
      char a;
      union
      {
        char b;
        char c;
      };
      char r;
    };
    struct
    {
      char s;
      char t;
      char u;
    };
  };
  char z;
};

/* three unions that start at the same member */
struct Z
{
  int a;
  union
  {
    struct
    {
      union
      {
        struct
        {
          union
          {
            char p;
            char q;
          };
          char r;
        };
        char m[2];
      };
      int n;
    };
    struct
    {
      int s;
      long t;
    };
  };
  char z;
};

const struct W cwt = {.a = 1, .t = 9, .z = 4};
const struct W cwu = {.u = 6};
const struct W cwa[2] = {{.s = 5}, {.a = 1, .t = 9, .z = 4}};
struct W gwa[2] = {{.t = 9}, {.a = 1, .u = 6, .z = 4}};
/* initializers for the first alternative */
const struct W cwy = {.a = 1, .y = 7, .r = 3, .z = 4};
const struct W cwp = {1, 2, 3, 4};

const union V cvc = {.c = {1, 2}};
const union V cvq = {.q = 1, .r = 2};

const struct Y cyt = {.t = 9, .z = 4};
const struct Y cyc = {.a = 5, .c = 1, .r = 2, .z = 4};

const struct Z czt = {.a = 0x0102, .t = 0x0a0b0c0dL, .z = 4};
const struct Z czm = {.a = 0x0102, .m = {5, 6}, .z = 4};
const struct Z czq = {.a = 0x0102, .q = 7, .r = 8, .n = 0x0304, .z = 4};

void
testNested (void)
{
  struct W w = {.a = 1, .t = 9, .z = 4};

  ASSERT (cwt.a == 1 && cwt.s == 0 && cwt.t == 9 && cwt.u == 0 && cwt.z == 4);
  ASSERT (cwu.a == 0 && cwu.s == 0 && cwu.t == 0 && cwu.u == 6 && cwu.z == 0);
  ASSERT (cwa[0].s == 5 && cwa[0].t == 0 && cwa[0].z == 0);
  ASSERT (cwa[1].a == 1 && cwa[1].t == 9 && cwa[1].z == 4);
  ASSERT (gwa[0].t == 9 && gwa[0].u == 0 && gwa[0].z == 0);
  ASSERT (gwa[1].a == 1 && gwa[1].u == 6 && gwa[1].z == 4);
  ASSERT (w.a == 1 && w.s == 0 && w.t == 9 && w.u == 0 && w.z == 4);

  ASSERT (cwy.a == 1 && cwy.x == 0 && cwy.y == 7 && cwy.r == 3 && cwy.z == 4);
  ASSERT (cwp.a == 1 && cwp.p == 2 && cwp.y == 0 && cwp.r == 3 && cwp.z == 4);
}

void
testUnionObject (void)
{
  ASSERT (cvc.c[0] == 1 && cvc.c[1] == 2);
  ASSERT (cvq.q == 1 && cvq.r == 2);
}

void
testInnerNotFirst (void)
{
  ASSERT (cyt.s == 0 && cyt.t == 9 && cyt.u == 0 && cyt.z == 4);
  ASSERT (cyc.a == 5 && cyc.c == 1 && cyc.r == 2 && cyc.z == 4);
}

void
testLevels (void)
{
  ASSERT (czt.a == 0x0102 && czt.s == 0 && czt.t == 0x0a0b0c0dL && czt.z == 4);
  ASSERT (czm.a == 0x0102 && czm.m[0] == 5 && czm.m[1] == 6 && czm.n == 0 && czm.z == 4);
  ASSERT (czq.a == 0x0102 && czq.q == 7 && czq.r == 8 && czq.n == 0x0304 && czq.z == 4);
}
