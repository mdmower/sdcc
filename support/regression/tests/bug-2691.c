/* bug-2691.c
   The address of an element of an array of pointers had the type of the
   element when the array was itself an array element or a struct member.
   On every port, the compiler scaled arithmetic on such an address by the
   wrong size. For mcs51, it also stopped with the internal error "need
   pointerCode" or converted the address to a generic pointer into the
   wrong address space.
 */

#include <testfwk.h>

#if !defined(__SDCC_pdk13) && !defined(__SDCC_pdk14) // Lack of memory
struct s
{
  char c;
  char *names[2];
  void *ptrs[2];
};

long g;
char buf[2];
long *arr[2][2];
void *varr[2][2];
void *q;
struct s st;
unsigned char one = 1;

/* The loop of the Csmith program in the bug report, with fewer iterations
   and a smaller array. */
static unsigned char
fromReport (void)
{
  long *l_2 = &g;
  unsigned char n = 0;

  for (g = 0; g > -3; g -= 1)
    {
      long **l_15[2][2] = {{&l_2, 0}, {&l_2, 0}};
      long ***l_14 = &l_15[1][0];

      (*l_14) = 0;
      if (l_15[1][0] == 0 && l_15[0][0] == &l_2)
        n++;
    }
  return n;
}

static void
store (void *p, void *v)
{
  *(void **)p = v;
}

/* The address is converted to void * by assignment and as an argument. */
static void
toVoid (void)
{
  q = &varr[1][1];
  *(void **)q = &g;
  store (&varr[0][1], buf);
  store (&st.ptrs[one], buf + 1);
}

static void
arith (void)
{
  long **p = &arr[1][0] + 1;
  char **n = &st.names[0] + one;

  *p = &g;
  *n = buf;
  (&arr[0][0])[1] = &g;
}
#endif

void
testBug (void)
{
#if !defined(__SDCC_pdk13) && !defined(__SDCC_pdk14) // Lack of memory
  ASSERT (fromReport () == 3);

  toVoid ();
  ASSERT (varr[1][1] == &g);
  ASSERT (varr[0][1] == buf);
  ASSERT (varr[0][0] == 0 && varr[1][0] == 0);
  ASSERT (st.ptrs[1] == buf + 1);
  ASSERT (st.ptrs[0] == 0);

  arith ();
  ASSERT (arr[1][1] == &g);
  ASSERT (st.names[1] == buf);
  ASSERT (arr[0][1] == &g);
  ASSERT (arr[0][0] == 0 && arr[1][0] == 0 && st.names[0] == 0 && st.c == 0);

  ASSERT (&arr[one][1] - &arr[0][one] == 2);
  ASSERT (*(&arr[one][0])[one] == g);
#endif
}
