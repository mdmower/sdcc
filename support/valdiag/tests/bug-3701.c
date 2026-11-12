/* bug-3701.c

   Identifiers that have linkage can be declared in a block more than once.
   The types have to be compatible, and identifiers without linkage cannot
   be declared again.
 */

#ifdef TEST1
const char *const names[2] = {"a", "b"};

char f(void)
{
  extern const char *const names[];
  extern const char *const names[];
  extern int i, i;
  extern int i;

  return names[i][0];
}
#endif

#ifdef TEST2
int f(void)
{
  int g(int);
  extern int g(int);
  int g(int);

  return g(1);
}
#endif

#ifdef TEST3
int a[2];

int f(void)
{
  extern int a[];
  {
    extern int a[];
    a[0]++;
  }
  return a[1];
}
#endif

#ifdef TEST4
int f(void)
{
  extern int i; /* IGNORE */
  extern char i; /* ERROR */

  return i; /* IGNORE */
}
#endif

#ifdef TEST5
int f(void)
{
  extern int a[2]; /* IGNORE */
  extern int a[3]; /* ERROR */

  return a[0]; /* IGNORE */
}
#endif

#ifdef TEST6
int f(void)
{
  int i; /* IGNORE */
  extern int i; /* ERROR */

  return i; /* IGNORE */
}
#endif

#ifdef TEST7
int f(void)
{
  extern int i; /* IGNORE */
  int i; /* ERROR */

  return i; /* IGNORE */
}
#endif

#ifdef TEST8
int f(int i) /* IGNORE */
{
  extern int i; /* ERROR */

  return i; /* IGNORE */
}
#endif
