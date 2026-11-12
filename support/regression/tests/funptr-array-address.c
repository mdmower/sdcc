/* funptr-array-address.c
   The address of an element of an array of function pointers had the type
   pointer to the array when the array was itself an array element, a struct
   member, or the target of a pointer. The compiler scaled arithmetic on
   such an address by the size of the array. It also took the element at
   an index from such an address for an array, so that a read or a call
   used the address of the element instead of the pointer stored there.
 */

#include <testfwk.h>

#if !defined(__SDCC_pdk13) && !defined(__SDCC_pdk14) // Lack of memory
struct s
{
  char c;
  void (*m[2]) (void);
};

void (*fp[2][2]) (void);
struct s sa[2];
unsigned char zero = 0;
unsigned char one = 1;
unsigned char calls;

static void
f (void)
{
  calls += 1;
}

static void
g (void)
{
  calls += 16;
}

static void
store (void)
{
  void (*(*row)[2]) (void) = &fp[0];
  struct s *ps = &sa[1];
  void (**p) (void) = &ps->m[zero] + one;

  (&fp[1][0])[1] = f;
  (&(*row)[zero])[one] = g;
  (&sa[0].m[0])[1] = f;
  *p = g;
}
#endif

void
testStore (void)
{
#if !defined(__SDCC_pdk13) && !defined(__SDCC_pdk14) // Lack of memory
  store ();
  ASSERT (fp[1][1] == f);
  ASSERT (fp[0][1] == g);
  ASSERT (fp[0][0] == 0 && fp[1][0] == 0);
  ASSERT (sa[0].m[1] == f);
  ASSERT (sa[1].m[1] == g);
  ASSERT (sa[0].m[0] == 0 && sa[1].m[0] == 0 && sa[0].c == 0 && sa[1].c == 0);
#endif
}

void
testArith (void)
{
#if !defined(__SDCC_pdk13) && !defined(__SDCC_pdk14) // Lack of memory
  void (*(*row)[2]) (void) = &fp[1];

  ASSERT (&fp[1][0] + 1 == &fp[1][1]);
  ASSERT (&fp[one][zero] + one == &fp[1][1]);
  ASSERT (&(*row)[0] + 1 == &fp[1][1]);
  ASSERT (&sa[one].m[zero] + one == &sa[1].m[1]);
  ASSERT (&fp[1][1] - &fp[0][1] == 2);
  ASSERT (&fp[one][one] - &fp[zero][one] == 2);
  ASSERT (&sa[0].m[1] - &sa[0].m[0] == 1);
#endif
}

/* The elements have the values that testStore gave them. */
void
testLoad (void)
{
#if !defined(__SDCC_pdk13) && !defined(__SDCC_pdk14) // Lack of memory
  ASSERT ((&fp[one][0])[one] == f);
  ASSERT ((&fp[0][0])[1] == g);
  ASSERT (*(&sa[zero].m[0] + one) == f);
#endif
}

void
testCall (void)
{
#if !defined(__SDCC_pdk13) && !defined(__SDCC_pdk14) // Lack of memory
  calls = 0;
  (&fp[one][0])[one] ();
  (*(&sa[one].m[zero] + one)) ();
  ASSERT (calls == 17);
#endif
}
