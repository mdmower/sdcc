/* bug-3979.c
   Designated initializers for several members of an anonymous struct
   that is an alternative of a union.

   The members of the anonymous struct are promoted into the union. Of
   the designated initializers, only one was used. The others were
   dropped, with warning 147 for a constant object and without a
   diagnostic for an object that code initializes.

   The constant objects are emitted as data, the automatic ones are
   initialized by code, and the others by code or as data, depending on
   the memory model.
 */

#include <testfwk.h>

/* the anonymous struct is the second alternative */
union u
{
  unsigned int raw;
  struct
  {
    unsigned char lo, hi;
  };
};

/* the anonymous struct is the first alternative */
union q
{
  struct
  {
    unsigned char a, b, c, d;
  };
  unsigned long l;
};

/* an anonymous union in a struct, with a member after it */
struct s
{
  unsigned char n;
  union
  {
    unsigned char a;
    struct
    {
      unsigned char b, c;
    };
    struct
    {
      unsigned char d, e, f;
    };
  };
  unsigned char z;
};

/* an anonymous union in the anonymous struct */
union n
{
  int a;
  struct
  {
    char b;
    union
    {
      char c;
      char d;
    };
    char e;
  };
};

const union u cu1 = {.lo = 0x34, .hi = 0x12};
const union u cu2 = {.hi = 0x12, .lo = 0x34};
const union u cu3 = {.hi = 0x12};
const union q cq1 = {.a = 1, .b = 2, .c = 3, .d = 4};
const union q cq2 = {.b = 2, .d = 4};
const struct s cs1 = {.n = 100, .a = 123, .z = 9};
const struct s cs2 = {.n = 100, .c = 123};
const struct s cs3 = {.n = 100, .e = 123};
const struct s cs4 = {100, .d = 1, .e = 2, .f = 3, 9};
const struct s cs5 = {100, .b = 1, .c = 2, .z = 9};
/* The first 0 is for c, the member after b, and replaces the initializer
   of d. */
const union n cn1 = {.d = 5, .b = 1, 0, 0};

union u gu1 = {.lo = 0x34, .hi = 0x12};
union q gq1 = {.a = 1, .b = 2, .c = 3, .d = 4};
struct s gs4 = {100, .d = 1, .e = 2, .f = 3, 9};
/* The 4 is for b, the member after a, which has an initializer. */
union q gq2 = {.b = 4, .a = 0, 4};

void
testConst (void)
{
  ASSERT (cu1.lo == 0x34 && cu1.hi == 0x12);
  ASSERT (cu2.lo == 0x34 && cu2.hi == 0x12);
  ASSERT (cu3.lo == 0 && cu3.hi == 0x12);

  ASSERT (cq1.a == 1 && cq1.b == 2 && cq1.c == 3 && cq1.d == 4);
  ASSERT (cq2.a == 0 && cq2.b == 2 && cq2.c == 0 && cq2.d == 4);

  ASSERT (cs1.n == 100 && cs1.a == 123 && cs1.z == 9);
  ASSERT (cs2.n == 100 && cs2.b == 0 && cs2.c == 123 && cs2.z == 0);
  ASSERT (cs3.n == 100 && cs3.d == 0 && cs3.e == 123 && cs3.f == 0 && cs3.z == 0);
  ASSERT (cs4.n == 100 && cs4.d == 1 && cs4.e == 2 && cs4.f == 3 && cs4.z == 9);
  ASSERT (cs5.n == 100 && cs5.b == 1 && cs5.c == 2 && cs5.f == 0 && cs5.z == 9);

  ASSERT (cn1.b == 1 && cn1.d == 0 && cn1.e == 0);
}

void
testGlobal (void)
{
  ASSERT (gu1.lo == 0x34 && gu1.hi == 0x12);
  ASSERT (gq1.a == 1 && gq1.b == 2 && gq1.c == 3 && gq1.d == 4);
  ASSERT (gs4.n == 100 && gs4.d == 1 && gs4.e == 2 && gs4.f == 3 && gs4.z == 9);
  ASSERT (gq2.a == 0 && gq2.b == 4 && gq2.c == 0 && gq2.d == 0);
}

void
testAuto (void)
{
  unsigned char i;

  /* a member without an initializer is zero, also the second time round */
  for (i = 0; i < 2; i++)
    {
      union u au1 = {.lo = 0x34, .hi = 0x12};
      union u au2 = {.hi = 0x12, .lo = 0x34};
      union u au3 = {.hi = 0x12};
      union q aq1 = {.a = 1, .b = 2, .c = 3, .d = 4};
      union q aq2 = {.b = 2, .d = 4};
      union q aq3 = {.c = 3, 4};
      struct s as4 = {100, .d = 1, .e = 2, .f = 3, 9};

      ASSERT (au1.lo == 0x34 && au1.hi == 0x12);
      ASSERT (au2.lo == 0x34 && au2.hi == 0x12);
      ASSERT (au3.lo == 0 && au3.hi == 0x12);

      ASSERT (aq1.a == 1 && aq1.b == 2 && aq1.c == 3 && aq1.d == 4);
      ASSERT (aq2.a == 0 && aq2.b == 2 && aq2.c == 0 && aq2.d == 4);
      ASSERT (aq3.a == 0 && aq3.b == 0 && aq3.c == 3 && aq3.d == 4);

      ASSERT (as4.n == 100 && as4.d == 1 && as4.e == 2 && as4.f == 3 && as4.z == 9);

      au1.raw = au2.raw = au3.raw = 0x5a5a;
      aq1.l = aq2.l = aq3.l = 0x5a5a5a5a;
    }
}
