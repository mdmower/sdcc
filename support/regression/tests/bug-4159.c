/* bug-4159.c
   An identifier after a struct or union tag was read as a typedef name when
   a typedef of that name was visible. Error 141 was given for a typedef that
   repeats an earlier typedef of the tag and for an object that hides a
   typedef name.
 */

#include <testfwk.h>

typedef unsigned char t;

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
typedef struct s s_t;
typedef struct s s_t;

typedef union u u_t;
typedef union u u_t;

typedef struct d { unsigned char a; } d_t;
typedef struct d d_t;

typedef const struct s cs_t;
typedef const struct s cs_t;
#else
typedef struct s s_t;
typedef union u u_t;
typedef struct d { unsigned char a; } d_t;
typedef const struct s cs_t;
#endif

struct s { unsigned char a; };
union u { unsigned char a; unsigned char b; };

/* Members named like typedefs, followed by members of those types. */
struct o { struct s t; union u d_t; t x; d_t y; };

s_t gs;
u_t gu;
d_t gd;
cs_t gc = {9};
struct o go;

void
testTypedef (void)
{
  gs.a = 5;
  gu.b = 7;
  gd.a = sizeof (struct s) + sizeof (t);
  ASSERT (gs.a == 5);
  ASSERT (gu.a == 7);
  ASSERT (gd.a == 2);
  ASSERT (gc.a == 9);
}

void
testHide (void)
{
  struct s t, d_t;
  union u s_t;
  struct s u_t = {2};

  t.a = 3;
  d_t.a = t.a + 2;
  s_t.b = 4;
  ASSERT (t.a == 3);
  ASSERT (d_t.a == 5);
  ASSERT (s_t.a == 4);
  ASSERT (u_t.a == 2);
}

void
testMember (void)
{
  go.t.a = 1;
  go.d_t.b = 2;
  go.x = 3;
  go.y.a = 4;
  ASSERT (go.t.a + go.d_t.a + go.x + go.y.a == 10);
}
