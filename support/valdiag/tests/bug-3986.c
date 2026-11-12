/* bug-3986.c

   Constant sums, differences, and left shifts that are assigned to a char
   were folded after their operands had been cast to char. Warning 158
   (overflow in implicit constant conversion) was given for values that
   fit, such as 30 * 8 - 1 and 1 << 7 for an unsigned char, and was not
   given for values that do not fit, such as 200 + 100.
 */

volatile unsigned char u;
volatile signed char s;

void g (unsigned char c);

#ifdef TEST1
/* Sums and differences that fit. */
unsigned char i = 30 * 8 - 1;

void f (void)
{
  unsigned char foo = 30 * 8 - 1;

  u = foo;
  u = 30 * 8;
  u = 30 * 8 - 1;
  u = 240 - 1;
  u = 256 - 1;
  u = 127 + 1;
  u = 200 + 55;
  u = 100000 - 99800;
  u = 100L + 100L;
  u = 1 ? 240 - 1 : 0;
  s = 1 * 5 - 2 * 5;
  s = 100 + 27;
  s = 0 - 128;
  s = -127 - 1;
  g (30 * 8 - 1);
}
#endif

#ifdef TEST2
/* Sums and differences that do not fit. */
void f (void)
{
  u = 200 + 100;        /* WARNING(SDCC) */ /* IGNORE(GCC) */
  u = 255 + 1;          /* WARNING(SDCC) */ /* IGNORE(GCC) */
  u = 20 * 10 + 10 * 10;/* WARNING(SDCC) */ /* IGNORE(GCC) */
  u = 0 - 1;            /* WARNING(SDCC) */ /* IGNORE(GCC) */
  s = 100 + 100;        /* WARNING(SDCC) */ /* IGNORE(GCC) */
  s = 127 + 1;          /* WARNING(SDCC) */ /* IGNORE(GCC) */
  s = 0 - 129;          /* WARNING(SDCC) */ /* IGNORE(GCC) */
  s = -127 - 2;         /* WARNING(SDCC) */ /* IGNORE(GCC) */
}
#endif

#ifdef TEST3
/* Left shifts that fit. */
void f (void)
{
  u = 1 << 7;
  u = 3 << 6;
  u = 239 << 0;
  u = (1 << 7) | 1;
  u = (1 << 8) - 1;
  s = 1 << 6;
  g (1 << 7);
}
#endif

#ifdef TEST4
/* Left shifts that do not fit. */
void f (void)
{
  u = 1 << 8;           /* WARNING(SDCC) */ /* IGNORE(GCC) */
  u = 3 << 7;           /* WARNING(SDCC) */ /* IGNORE(GCC) */
  s = 1 << 7;           /* WARNING(SDCC) */ /* IGNORE(GCC) */
}
#endif

#ifdef TEST5
/* Folded constants with the value 0 or 1 have the type bool in SDCC. These
   are operands of a bitwise operation with an unsigned char constant, or
   of a negation. */
void f (void)
{
  u = (1 << 7) | (1 << 0);
  u = ((unsigned char) 1 << 7) | ((unsigned char) 1 << 0);
  u = (unsigned char) 0x80 | (2 - 1);
  u = (unsigned char) 0x80 ^ (1 + 0);
  u = (unsigned char) 0x80 | ((unsigned char) 1 - (unsigned char) 1);
  s = -(2 - 1);
  s = -(1 << 0);
  s = -(1 * 1);
}
#endif
