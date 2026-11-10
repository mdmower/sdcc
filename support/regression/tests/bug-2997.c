/* bug-2997.c
   The value returned by an inlined __critical function was lost when the
   call was inside a nested block. The CRITICAL node wrapped around the
   inlined body was created at the wrong nesting level, so the return value
   was assigned to one variable and read from another ("may be used before
   initialization").
 */

#include <testfwk.h>

volatile int cnt;
int cnt0;

static inline int
cnt_get (void) __critical
{
  return cnt;
}

static inline unsigned char
diff_or_sum (unsigned char a, unsigned char b) __critical
{
  if (a > b)
    return a - b;
  return a + b;
}

void
testBug (void)
{
  unsigned char i;

  cnt = 0x1234;
  cnt0 = 0x1200;

  for (i = 0; i < 3; i++)
    {
      if (i == 1)
        {
          if ((cnt_get () - cnt0) >= 1)
            cnt0++;
          ASSERT (cnt0 == 0x1201);
        }
      else if (i == 2)
        {
          cnt = 7;
          ASSERT (cnt_get () == 7);
          ASSERT (diff_or_sum (5, 3) == 2);
          ASSERT (diff_or_sum (3, 5) == 8);
        }
    }
}
