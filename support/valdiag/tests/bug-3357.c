/* bug-3357.c

   After a declaration that followed the definition of a function, the symbol
   of the function did not record the body, and the compiler accepted a second
   definition.
 */

#ifdef TEST1
int f (int c)
{
  return c;
}

int f (int c);

int f (int c) /* ERROR */
{
  return c + 1;
}
#endif

#ifdef TEST2
/* Declarations before and after the one definition are valid. */
int g (int c);

int g (int c)
{
  return c;
}

int g (int c);
extern int g (int c);
#endif

#ifdef TEST3
static int h (int c)
{
  return c;
}

static int h (int c);

static int h (int c) /* ERROR */
{
  return c + 1;
}

int i (void)
{
  return h (1);
}
#endif
