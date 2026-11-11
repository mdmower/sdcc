/* bug-4233.c
   An explicit cast of an operation to a type smaller than int has to stay
   in effect when the result is converted to a wider type. As an argument
   for an unsigned long parameter, (unsigned char)(c << 1) is the low byte
   of the shift. The cast was removed as superfluous, and the shift was
   then done in int for the argument.
 */

#include <testfwk.h>

/* Not static. For a static object the cast is not to the same type as far
   as the compiler is concerned, and the cast is never removed. */
unsigned char nv;
unsigned char nh;
unsigned char nt;
signed char sh;

unsigned long
passLong (unsigned long x)
{
  return x;
}

unsigned int
passInt (unsigned int x)
{
  return x;
}

long
passSLong (long x)
{
  return x;
}

void
testShift (void)
{
  nv = 0;
  nh = 0x80;
  sh = 0x40;

  ASSERT (passLong ((unsigned char)(nh << 1)) == 0);
  ASSERT (passInt ((unsigned char)(nh << 1)) == 0);
  ASSERT (passSLong ((signed char)(sh << 2)) == 0);
  ASSERT (passLong ((unsigned char)(nh << 1) == 0) == 1);
  ASSERT (passLong ((unsigned char)(unsigned char)(nh << 1)) == 0);
  ASSERT (passLong ((unsigned char)(nv ? nv : nh << 1)) == 0);
}

void
testArith (void)
{
  nv = 0;
  nh = 0x80;
  nt = 2;

  ASSERT (passLong ((unsigned char)(nh + nh)) == 0);
  ASSERT (passLong ((unsigned char)(nv - nt)) == 0xfe);
  ASSERT (passLong ((unsigned char)(nh * nt)) == 0);
  ASSERT (passLong ((unsigned char)-nh) == 0x80);
  ASSERT (passInt ((unsigned char)(nv - nt)) == 0xfe);
}

void
testComplement (void)
{
  nv = 0;

  ASSERT (passLong ((unsigned char)~nv) == 0xff);
  ASSERT (passInt ((unsigned char)~nv) == 0xff);
}
