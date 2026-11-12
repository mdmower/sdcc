/* bug-3973.c

   A bit-field that is initialized by data with a cast of a string literal
   got bits of the compiler's own address of the literal, and a float got
   that address as its value. Neither can hold an address, so these
   initializers are errors. An integer that is not a bit-field takes the
   address.
 */

#include <stdint.h>

#ifdef TEST1
struct bf
{
  unsigned int a : 4;
  unsigned int b : 12;
};

const struct bf s = { 1, (unsigned int)"AB" }; /* ERROR */
#endif

#ifdef TEST2
const float f = (float)"CD"; /* ERROR */
#endif

#ifdef TEST3
const uintptr_t u = (uintptr_t)"EF";
uintptr_t v = (uintptr_t)"GH";
#endif
