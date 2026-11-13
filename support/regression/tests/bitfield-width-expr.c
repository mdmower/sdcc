/** bitfield-width-expr.c
    The width of a bit-field is a constant expression. For a conditional
    operator with a constant condition and for a generic selection, the
    width is the value of the operand that is selected.
 */

#include <testfwk.h>

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
#define HAVE_GENERIC
#endif

#define MAX(a, b) ((a) > (b) ? (a) : (b))

enum { LOW_BITS = 3 };

struct bits
{
  unsigned int a : 1 ? 3 : 4;
  unsigned int : 0 ? 3 : 2;
  unsigned int b : 0 ? 3 : 5;
#ifdef HAVE_GENERIC
  unsigned int c : _Generic (1, int : 6, default : 1.0);
#endif
};

/* struct bits with each width written as a number */
struct plain
{
  unsigned int a : 3;
  unsigned int : 2;
  unsigned int b : 5;
#ifdef HAVE_GENERIC
  unsigned int c : 6;
#endif
};

struct sized
{
  unsigned int low : MAX (LOW_BITS, 2);
  unsigned int high : LOW_BITS > 4 ? 1 : 8 - LOW_BITS;
};

union
{
  struct bits expr;
  struct plain number;
} overlay;

void
testWidth (void)
{
  struct bits s;
  struct sized t;

  s.a = 0xff;
  s.b = 0xff;
  ASSERT (s.a == 7);
  ASSERT (s.b == 31);

#ifdef HAVE_GENERIC
  s.c = 0xff;
  ASSERT (s.c == 63);
#endif

  t.low = 0xff;
  t.high = 0xff;
  ASSERT (t.low == 7);
  ASSERT (t.high == 31);
}

/* The unnamed bit-field has 2 bits, so that b and c are at the positions
   that they have in struct plain. */
void
testLayout (void)
{
  ASSERT (sizeof (struct bits) == sizeof (struct plain));

  overlay.number.a = 0;
  overlay.number.b = 0;
#ifdef HAVE_GENERIC
  overlay.number.c = 0;
#endif

  overlay.expr.b = 21;
  ASSERT (overlay.number.a == 0);
  ASSERT (overlay.number.b == 21);

#ifdef HAVE_GENERIC
  ASSERT (overlay.number.c == 0);
  overlay.expr.c = 42;
  ASSERT (overlay.number.b == 21);
  ASSERT (overlay.number.c == 42);
#endif

  overlay.expr.a = 5;
  ASSERT (overlay.number.a == 5);
  ASSERT (overlay.number.b == 21);
}
