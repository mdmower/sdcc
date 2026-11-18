/* bug-3357.c
   Functions that were declared again after their definition, and not called
   in their translation unit, did not get a global symbol for some ports, so
   other translation units could not call them. bug_3357 is defined in
   fwk/lib/extern1.c, and a declaration follows the definition there.
 */

#include <testfwk.h>

int bug_3357 (int c);

void
testBug (void)
{
  ASSERT (bug_3357 (41) == 42);
}
