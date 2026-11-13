/* bitfield-width-expr.c

   The width of a bit-field can be a conditional operator with a constant
   condition or a generic selection. The value of the operand that is
   selected is the width, and it is checked as a width that is written as
   a number.
 */

#ifdef TEST1
struct s
{
  int a : 1 ? 3 : 4;
  int : 0 ? 3 : 2;
  int b : 0 ? 3 : 5;
};
#endif

#ifdef TEST2
struct s
{
  int a : _Generic (1, int: 3, default: 1.0);
  int : _Generic (1, float: 1.0f, default: 2);
};
#endif

#ifdef TEST3
struct s
{
  int a : 0 ? 3 : -1; /* ERROR */
};
#endif

#ifdef TEST4
struct s
{
  int a;
  int : 1 ? -2 : 2; /* ERROR */
};
#endif
