/** bug-3973.c
    An integer in initialized data got bytes of the compiler's own address
    of a string literal when a cast of the literal initialized it.
 */

#include <testfwk.h>

#include <stddef.h>
#include <stdint.h>

struct my_data_t
{
  const char *a;
  const void *b;
  uintptr_t c;
};

/* the object of the ticket */
const struct my_data_t my_data2 = { "AAA", "AAA", (uintptr_t)"AAA" };

/* only an integer refers to each of these literals */
uintptr_t var = (uintptr_t)"BC";
const uintptr_t cst = (uintptr_t)"DE";
const uintptr_t arr[2] = { (uintptr_t)"FG", (uintptr_t)"HI" };
const intptr_t sgn = (intptr_t)"JK";
const uintptr_t wide = (uintptr_t)L"LM";

#ifdef __SDCC
/* an integer that is wider than a pointer on some ports */
const unsigned long wider = (unsigned long)"NO";
/* the address of a string literal is not a null pointer */
const _Bool nonNull = (_Bool)"PQ";
#endif

#ifdef __SDCC_mcs51
/* an integer that is narrower than a generic pointer */
const unsigned int narrower = (unsigned int)"RS";
#endif

#define STR(u) ((const char *) (u))
#define WSTR(u) ((const wchar_t *) (u))

static uintptr_t
getLocal (void)
{
  static const uintptr_t loc = (uintptr_t)"TU";

  return loc;
}

void
testStruct (void)
{
  ASSERT (my_data2.a[0] == 'A');
  ASSERT (STR (my_data2.b)[1] == 'A');
  ASSERT (STR (my_data2.c)[0] == 'A');
  ASSERT (STR (my_data2.c)[2] == 'A');
  ASSERT (STR (my_data2.c)[3] == 0);
}

void
testScalar (void)
{
  ASSERT (STR (var)[0] == 'B');
  ASSERT (STR (var)[1] == 'C');
  ASSERT (STR (var)[2] == 0);

  ASSERT (STR (cst)[0] == 'D');
  ASSERT (STR (cst)[1] == 'E');

  ASSERT (STR (arr[0])[0] == 'F');
  ASSERT (STR (arr[0])[1] == 'G');
  ASSERT (STR (arr[1])[0] == 'H');
  ASSERT (STR (arr[1])[1] == 'I');

  ASSERT (STR (sgn)[0] == 'J');
  ASSERT (STR (sgn)[1] == 'K');

  ASSERT (WSTR (wide)[0] == L'L');
  ASSERT (WSTR (wide)[1] == L'M');

  ASSERT (STR (getLocal ())[0] == 'T');
  ASSERT (STR (getLocal ())[1] == 'U');
}

void
testSize (void)
{
#ifdef __SDCC
  ASSERT (STR ((uintptr_t) wider)[0] == 'N');
  ASSERT (STR ((uintptr_t) wider)[1] == 'O');
  /* the bytes above the pointer are zero */
  ASSERT ((unsigned long) (uintptr_t) wider == wider);

  ASSERT (nonNull);
#endif

#ifdef __SDCC_mcs51
  ASSERT (((const char __code *) narrower)[0] == 'R');
  ASSERT (((const char __code *) narrower)[1] == 'S');
#endif
}
