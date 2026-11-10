/* bug-3674.c
   Some ports, when not using --std-cXX, omit integer promotion on explicitly
   cast variable arguments. A cast to the type the operand already has was
   honoured for variables only: the result of a function call, a dereferenced
   pointer, an array element or a structure member reached through a pointer
   was promoted nevertheless (also bug #3172).
 */

#include <testfwk.h>

#include <stdarg.h>

#if defined(__SDCC_mcs51) || defined (__SDCC_ds390) || defined (__SDCC_mos6502) || defined (__SDCC_mos65c02) || defined (__SDCC_pic14) || defined (__SDCC_pic16)
# define va_uchar unsigned char
#else
# define va_uchar int
#endif

struct s
{
  unsigned int i;
  unsigned char c;
};

/* Not static: for a static object the cast is not to the same type
   as far as the compiler is concerned. */
unsigned char seq;
unsigned char a[2];
unsigned char *p;
struct s st;
struct s *ps;

#ifndef __SDCC_pic16
unsigned char
next (void)
{
  return ++seq;
}

unsigned char
twice (unsigned char c)
{
  return c * 2;
}

/* Returns 0 if the variable arguments are c0 and c1, passed as va_uchar,
   followed by 0x1234, passed as int. */
static unsigned char
castArgs (unsigned char c0, unsigned char c1, ...)
{
  va_list ap;
  unsigned char bad = 0;

  va_start (ap, c1);
  if (va_arg (ap, va_uchar) != c0)
    bad |= 1;
  if (va_arg (ap, va_uchar) != c1)
    bad |= 2;
  if (va_arg (ap, int) != 0x1234)
    bad |= 4;
  va_end (ap);

  return bad;
}

/* The same for arguments that have been promoted to int. */
static unsigned char
plainArgs (unsigned char c0, unsigned char c1, ...)
{
  va_list ap;
  unsigned char bad = 0;

  va_start (ap, c1);
  if (va_arg (ap, int) != c0)
    bad |= 1;
  if (va_arg (ap, int) != c1)
    bad |= 2;
  if (va_arg (ap, int) != 0x1234)
    bad |= 4;
  va_end (ap);

  return bad;
}
#endif

void
testBug (void)
{
#ifndef __SDCC_pic16
  seq = 0x10;
  a[0] = 0x5a;
  a[1] = 0xa5;
  p = &a[1];
  st.i = 0x789a;
  st.c = 0xc3;
  ps = &st;

  /* variables: this has always worked */
  ASSERT (castArgs (0x10, 0xc3, (unsigned char)seq, (unsigned char)st.c, 0x1234) == 0);

  /* function calls */
  ASSERT (castArgs (0x11, 0x42, (unsigned char)next (), (unsigned char)twice (0x21), 0x1234) == 0);
  ASSERT (seq == 0x11);

  /* dereferenced pointer, array element */
  ASSERT (castArgs (0xa5, 0x5a, (unsigned char)*p, (unsigned char)a[0], 0x1234) == 0);
  ASSERT (castArgs (0x5a, 0xa5, (unsigned char)p[-1], (unsigned char)(*p), 0x1234) == 0);

  /* structure member */
  ASSERT (castArgs (0xc3, 0xc3, (unsigned char)ps->c, (unsigned char)(*ps).c, 0x1234) == 0);

  /* without a cast of their own the arguments are promoted */
  ASSERT (plainArgs (0x12, 0xa5, next (), *p, 0x1234) == 0);
  ASSERT (plainArgs (0xc3, 0x5a, ps->c, a[0], 0x1234) == 0);
  ASSERT (plainArgs (0x13, 0xa6, (unsigned char)seq + 1, (unsigned char)*p + 1, 0x1234) == 0);
#endif
}
