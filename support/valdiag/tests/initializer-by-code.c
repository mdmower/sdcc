/* initializer-by-code.c

   SDCC accepts some initializers of objects with static storage duration
   that are not constant expressions. Code that runs before main() computes
   them. The code for a global variable is not part of a function, and on
   mcs51 only a function has spill locations.
 */

struct s { char a; int b[3]; };

extern char x, y;
extern char ar[4];
extern char ar2[2][3];
extern struct s st, sta[2];
extern int vi;
void fn (void);

/* The sum needs a spill location for its 8 bytes. */
#ifdef TEST1
#ifdef __SDCC_mcs51
unsigned long long a = (unsigned long long)(unsigned)&x + (unsigned)&y; /* ERROR(SDCC_mcs51) */
#endif
#endif

/* Without --stack-auto, the result of the logical operation is a bit that
   needs a spill location. */
#ifdef TEST2
#ifdef __SDCC_mcs51
int a = &ar[1] && &ar[2]; /* ERROR(SDCC_mcs51 && !SDCC_STACK_AUTO) */
#endif
#endif

/* The registers cannot hold the 8 bytes of the sum together with the
   pointer to the variable. */
#ifdef TEST3
#ifdef __SDCC_mcs51
__idata unsigned long long a = (unsigned long long)&x + 1; /* ERROR(SDCC_mcs51) */
#endif
#endif

/* Addresses with a constant offset. */
#ifdef TEST4
char *p1 = &ar[1];
char *p2 = ar + 2;
char *p3 = 2 + ar;
char *p4 = ar + 1 + 1;
char *p5 = &ar2[1][2];
int *p6 = &st.b[1];
int *p7 = &sta[1].b[2];
char *p8 = &(sta + 1)->a;
char *p9 = (char *)&st + sizeof (int);
const char *p10 = "abc" + 1;
void (*p11) (void) = fn;

char *f (void)
{
  static char *p = &ar[1];

  return p;
}
#endif

/* The mcs51 small model initializes these variables by code. */
#ifdef TEST5
#if defined (__SDCC_mcs51) && defined (__SDCC_MODEL_SMALL)
char e = ar[1];
int m = vi & 1;
char *p = &ar[vi];
int c = (&x == &y) ? 1 : 2;
unsigned long long w = (unsigned long long)&x + 1;
#endif
#endif

/* A static variable of a function with a conditional operator in its
   initializer. */
#ifdef TEST6
int f (void)
{
  static int a = (&x == &y) ? 1 : 2;

  return a;
}
#endif
