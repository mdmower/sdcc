/* bug-4155.c

   A cast from an integer to a generic pointer warned that the storage of the
   pointer cannot be set (warning 127) even for uintptr_t, which is wide
   enough to hold a generic pointer including its storage byte.
 */

#include <stdint.h>

#ifdef TEST1
void *f (uintptr_t u)
{
  return (void *)u;
}
#endif

#ifdef TEST2
uintptr_t g (void *p)
{
  return (uintptr_t)(void *)(uintptr_t)p;
}
#endif

#ifdef TEST3
/* An integer narrower than a generic pointer cannot say where it points. */
void *h (unsigned char c)
{
  return (void *)c; /* WARNING(SDCC_mcs51) */
}
#endif

#ifdef TEST4
/* Without a cast the conversion still deserves a diagnostic. */
void *i (uintptr_t u)
{
  return u; /* WARNING(SDCC) */ /* IGNORE(GCC) */
}
#endif
