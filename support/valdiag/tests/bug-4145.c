/* bug-4145.c
   An expression computed from a constexpr object and a run-time value is not
   a constant expression, and what is declared with its type is not a
   constexpr object. Both used to crash the compiler.
 */

#ifdef __SDCC
#pragma std_c23
#endif

#ifdef TEST1
int f (int x)
{
  constexpr int c = 3;
  enum { E = x + c }; /* ERROR */
  return E;
}
#endif

#ifdef TEST2
int f (int x)
{
  constexpr int c = 3;
  struct s
  {
    int a;
    int b : x + c; /* ERROR */
  };
  return x;
}
#endif

#ifdef TEST3
int f (int x)
{
  constexpr int c = 3;
  static_assert (x ? 4 : c); /* ERROR */
  return x;
}
#endif

#ifdef TEST4
int f (int x)
{
  constexpr int c = 3;
  typeof (x + c) y = x;
  auto z = x ? 4 : c;
  y += c;
  z++;
  return y + z + sizeof (typeof (x + c));
}
#endif
