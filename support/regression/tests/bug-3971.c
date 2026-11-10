/* bug-3971.c
   A string literal plus an offset in a static initializer ("AAA" + 3)
   crashed the compiler: the sum has the type of the string literal, so
   constExprValue() took the '+' node for the literal itself and read a
   symbol out of it.
 */

#include <testfwk.h>

struct s
{
  const char *a;
  const char *b;
  const char *c;
  const char *d;
};

#ifdef __SDCC_mcs51
__xdata char inXdata[4];
__data char inData[4];
#else
char inXdata[4];
char inData[4];
#endif

struct s vars = { "ABC" + 1, inXdata, inData };
const struct s consts = { "DEF" + 2, 1 + "GHI", "JKL", "MNO" + 3 };
const char *ptr = "PQR" + 3;
const char * const constPtr = "STU" + 1;

void
testBug (void)
{
  ASSERT (vars.a[-1] == 'A');
  ASSERT (vars.a[0] == 'B');
  ASSERT (vars.a[1] == 'C');
  ASSERT (vars.a[2] == 0);
  ASSERT (vars.b == inXdata);
  ASSERT (vars.c == inData);
  ASSERT (vars.d == 0);

  ASSERT (consts.a[-2] == 'D');
  ASSERT (consts.a[0] == 'F');
  ASSERT (consts.b[-1] == 'G');
  ASSERT (consts.b[0] == 'H');
  ASSERT (consts.c[0] == 'J');
  ASSERT (consts.d[-1] == 'O');
  ASSERT (consts.d[0] == 0);

  ASSERT (ptr[-3] == 'P');
  ASSERT (ptr[0] == 0);
  ASSERT (constPtr[-1] == 'S');
  ASSERT (constPtr[0] == 'T');
}
