/* bug-4118.c
   constexpr is not a storage class that parameters can have. For a parameter
   that is declared constexpr, the compiler reports an error and then treats
   the parameter as an ordinary one, which the function can use.
 */

#ifdef __SDCC
#pragma std_c23
#endif

#ifdef TEST1
int f (constexpr int x) /* ERROR */
{
  return x;
}
#endif

#ifdef TEST2
int f (constexpr int x) /* ERROR */
{
  return x == 1;
}
#endif

#ifdef TEST3
_Bool f (constexpr int x) /* ERROR */
{
  return x;
}
#endif

#ifdef TEST4
int f (int a, constexpr int b, int c) /* ERROR */
{
  return a + b + c;
}
#endif

#ifdef TEST5
typedef int I;

int f (constexpr I x) /* ERROR */
{
  return x;
}
#endif

#ifdef TEST6
int f (constexpr int); /* ERROR */
int g (constexpr int *); /* ERROR */
#endif
