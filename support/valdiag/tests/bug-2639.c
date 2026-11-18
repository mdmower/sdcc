/* bug-2639.c

   A call with a wrong number of arguments gets an error. That is also the
   only error when the call is an argument of another call and needs a
   conversion to the type of its parameter. Each inner call is written on
   two lines. An internal error for its arguments (node PARAM shouldn't be
   processed here) has the number of the second line, where a diagnostic is
   not expected.
 */

extern void g (unsigned int a);
extern void g2 (unsigned int a, unsigned int b);

#ifdef TEST1
/* The function of the inner call is not declared. */
void f (void)
{
  g (u (1, /* ERROR(SDCC) */ /* IGNORE(GCC) */
        2));
}
#endif

#ifdef TEST2
extern int h (void); /* IGNORE(GCC) */

void f (void)
{
  g (h (1, /* ERROR */
        2));
}
#endif

#ifdef TEST3
extern int h (void); /* IGNORE(GCC) */

/* The inner call is an operand of the argument. */
void f (void)
{
  g (h (1, /* ERROR */
        2) + 1);
}
#endif

#ifdef TEST4
extern int h (void); /* IGNORE(GCC) */

/* The inner call is the first of two arguments. */
void f (void)
{
  g2 (h (1, /* ERROR */
         2), 3);
}
#endif

#ifdef TEST5
extern int k (int a, int b, int c); /* IGNORE(GCC) */

/* The inner call has too few arguments. */
void f (void)
{
  g (k (1, /* ERROR */
        2));
}
#endif
