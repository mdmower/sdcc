/* bug-3286.c
   An internal error in the mcs51 code generator ("getFreePtr should never
   reach here") with --stack-auto: the result of a cast from a 64-bit stack
   parameter was given r0 and r1, the registers its spilt operand once had,
   leaving no pointer register to read the operand with.
 */

#include <testfwk.h>

#include <stdint.h>

#if !defined(__SDCC_pdk13) && !defined(__SDCC_pdk14) // Lack of memory
uint32_t g_x;
unsigned char calls;

uint8_t
func_2 (void)
{
  return 1;
}

void
func_1 (void)
{
  calls++;
}

void
func_0 (uint64_t x)
{
  if (func_2 ())
    {
      if (g_x == 0)
        g_x = (uint32_t)x | 1;

      if ((uint32_t)x - g_x > 10)
        func_1 ();
    }
}
#endif

void
testBug (void)
{
#if !defined(__SDCC_pdk13) && !defined(__SDCC_pdk14) // Lack of memory
  g_x = 0;
  calls = 0;

  func_0 (0x0123456789abcde0ull);
  ASSERT (g_x == 0x89abcde1ul);
  ASSERT (calls == 1); /* 0x89abcde0 - 0x89abcde1 wraps around */

  func_0 (0xffffffff89abcdf0ull);
  ASSERT (g_x == 0x89abcde1ul);
  ASSERT (calls == 2);

  func_0 (0x89abcde5ull);
  ASSERT (calls == 2);
#endif
}
