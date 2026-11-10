/* bug-4228.c
   The check whether an initializer is a constant expression decorated the
   stored initializer and dropped the tree that decoration returned. Where
   decoration replaces the root of the expression (a conditional operator
   with a constant condition, _Generic), the object was later initialized
   from the old root, which has no type. That broke constexpr objects and
   compound literals. A bit-field width was evaluated twice, the second time
   from the old root.
 */

#include <testfwk.h>

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
#define HAVE_GENERIC
#endif

struct pair
{
  int a;
  int b;
};

struct bits
{
  unsigned int a : 1 ? 3 : 4;
  unsigned int : 0 ? 3 : 2;
  unsigned int b : 0 ? 3 : 5;
#ifdef HAVE_GENERIC
  unsigned int c : _Generic (1, int : 6, default : 1.0);
#endif
};

static int x = 5;

#if defined(__SDCC)
_Pragma("std_c23")
#define HAVE_CONSTEXPR
#elif defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
#define HAVE_CONSTEXPR
#endif

#ifdef HAVE_CONSTEXPR
constexpr int fileGeneric = _Generic (1, int : 7, default : 1.0);
constexpr int fileDefault = _Generic (1, float : 1.0f, default : 8);
constexpr int fileCond = 1 ? 9 : 3;
constexpr int fileSum = fileGeneric + fileCond;
char fileArray[fileGeneric];
#endif

void
testConstexpr (void)
{
#ifdef HAVE_CONSTEXPR
  constexpr int generic = _Generic (1, int : 13, default : 1.0);
  constexpr int nested = _Generic (1, int : _Generic (1L, long : 17, default : 1.0), default : 2.0);
  constexpr int cond = 0 ? 1 : 15;
  constexpr int previous = _Generic (1, int : generic, default : 1.0);
  char array[generic];

  ASSERT (fileGeneric == 7);
  ASSERT (fileDefault == 8);
  ASSERT (fileCond == 9);
  ASSERT (fileSum == 16);
  ASSERT (sizeof (fileArray) == 7);

  ASSERT (generic == 13);
  ASSERT (nested == 17);
  ASSERT (cond == 15);
  ASSERT (previous == 13);
  ASSERT (generic + cond == 28);
  ASSERT (sizeof (array) == 13);
#endif
}

void
testCompoundLiteral (void)
{
  struct pair v = (struct pair){1 ? 2 : 3, 4};

  ASSERT (v.a == 2);
  ASSERT (v.b == 4);

  ASSERT ((int){1 ? 2 : 3} == 2);
  ASSERT ((int){0 ? x : 3} == 3);
  ASSERT ((int){1 ? x : 3} == 5);
  ASSERT (*(int *){1 ? &x : 0} == 5);

#ifdef HAVE_GENERIC
  ASSERT ((int){_Generic (1, int : 27, default : 1.0)} == 27);
  ASSERT ((int){_Generic (1, float : 1.0f, default : 29)} == 29);
  ASSERT ((int){_Generic (1, int : x, default : 1.0)} == 5);
#endif
}

void
testBitfieldWidth (void)
{
  struct bits s;

  s.a = 0xff;
  s.b = 0xff;
  ASSERT (s.a == 7);
  ASSERT (s.b == 31);

#ifdef HAVE_GENERIC
  s.c = 0xff;
  ASSERT (s.c == 63);
#endif
}
