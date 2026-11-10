/* bug-4249.c
   The address of an array, &array, has type pointer to array; adding 1 to it
   must step over the whole array. It was scaled by the element size instead.
 */

#include <testfwk.h>

static char array[16];
static char *seen;

static void
sink (char *p)
{
  seen = p;
}

static int
test (void)
{
  char (*p)[16] = &array;

  sink (*p++);
  return p == &array + 1;
}

void
testBug (void)
{
  ASSERT (test ());
  ASSERT (seen == array);
  ASSERT ((char *)(&array + 1) == array + 16);
}
