/* feature-request-1038.c

   The compiler keeps a case range as one entry of a switch statement. The
   values of the range must not be values of other cases.
 */

int x;

#ifdef __SDCC
#pragma std_c2y
#elif defined(__GNUC__)
/* GCC implements case ranges as an extension. */
#pragma GCC diagnostic ignored "-Wpedantic"
#endif

#ifdef TEST1
/* A case inside an earlier range. */
char foo (void)
{
  switch (x)
    {
      case 100 ... 150:         /* IGNORE */
        return 1;
      case 149:                 /* ERROR */
        return 2;
      default:
        return 0;
    }
}
#endif

#ifdef TEST2
/* The last value of an earlier range. */
char foo (void)
{
  switch (x)
    {
      case 100 ... 150:         /* IGNORE */
        return 1;
      case 150:                 /* ERROR */
        return 2;
      default:
        return 0;
    }
}
#endif

#ifdef TEST3
/* A range that starts inside an earlier range. */
char foo (void)
{
  switch (x)
    {
      case 100 ... 150:         /* IGNORE */
        return 1;
      case 150 ... 160:         /* ERROR */
        return 2;
      default:
        return 0;
    }
}
#endif

#ifdef TEST4
/* A range that starts below an earlier case and has its value. */
char foo (void)
{
  switch (x)
    {
      case 150:                 /* IGNORE */
        return 1;
      case 100 ... 150:         /* ERROR */
        return 2;
      default:
        return 0;
    }
}
#endif

#ifdef TEST5
/* A range that has all values of an earlier range. */
char foo (void)
{
  switch (x)
    {
      case 120 ... 130:         /* IGNORE */
        return 1;
      case 100 ... 150:         /* ERROR */
        return 2;
      default:
        return 0;
    }
}
#endif

#ifdef TEST6
/* Ranges and cases next to each other, in any order, are valid. */
char foo (void)
{
  switch (x)
    {
      case 151:
        return 1;
      case 100 ... 150:
        return 2;
      case 99:
        return 3;
      case -20 ... 98:
        return 4;
      case 152 ... 160:
        return 5;
      default:
        return 0;
    }
}
#endif

#ifdef TEST7
/* Ranges that start at the lowest value or end at the highest value of the
   type of the controlling expression do not get a warning. */
char foo (unsigned char c)
{
  switch (c)
    {
      case 0 ... 99:
        return 1;
      case 200 ... 255:
        return 2;
      default:
        return 0;
    }
}
#endif

#ifdef TEST8
/* A value of the range that the type cannot have. */
char foo (unsigned char c)
{
  switch (c)                    /* WARNING(SDCC) */
    {
      case 200 ... 256:         /* IGNORE(GCC) */
        return 1;
      default:
        return 0;
    }
}
#endif
