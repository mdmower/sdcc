/* bug-4228.c

   An initializer whose expression is replaced when its type is worked out
   (_Generic, a conditional operator with a constant condition) was rejected
   as an assignment from type 'void', followed by an internal error or a
   crash, when it initialized a constexpr object or a compound literal. A
   bit-field width of that kind was rejected as not constant.
 */

#ifdef __SDCC
#pragma std_c23
#elif !defined(__STDC_VERSION__) || __STDC_VERSION__ < 202311L
#define SKIP_C23
#endif

#ifndef SKIP_C23
#ifdef TEST1
constexpr int object = _Generic (1, int: 1, default: 1.0);
#endif

#ifdef TEST2
constexpr int object = 1 ? 2 : 3;
int array[object];
#endif

#ifdef TEST3
int test (void)
{
  constexpr int object = _Generic (1, int: 6, default: 1.0);
  int array[object];
  _Static_assert (sizeof array == 6 * sizeof (int), "constant array bound");
  array[0] = object;
  return array[0];
}
#endif

/* An initializer that is not constant is still found. */
#ifdef TEST4
int nonconstant;
constexpr int object = _Generic (1, int: nonconstant, default: 1); /* ERROR */
#endif

#ifdef TEST5
int nonconstant;
constexpr int object = 1 ? nonconstant : 3; /* ERROR */
#endif
#endif

#ifdef TEST6
int x;

int test (void)
{
  return (int){1 ? 2 : 3} + (int){1 ? x : 3} + (int){x ? 4 : 4};
}
#endif

#ifdef TEST7
struct s
{
  int a : 1 ? 3 : 4;
  int : 0 ? 3 : 2;
};
#endif

/* The value of the width is still checked. */
#ifdef TEST8
struct s
{
  int a : 0 ? 3 : -1; /* ERROR */
};
#endif
