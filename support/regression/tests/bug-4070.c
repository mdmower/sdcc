/* bug-4070.c
   The most significant byte of an __sfr32 was read from and written to
   address 0xff. The assembler evaluated expressions with 24 bits, so that
   ((_sfr >> 24) & 0xFF) gave the sign extension of bit 23 of the address
   list.
 */

#include <testfwk.h>

#if defined (__SDCC_mcs51)
__sfr __at 0xCC SFR_B0;         /* timer 2 low byte */
__sfr __at 0xCD SFR_B1;         /* timer 2 high byte */
__sfr __at 0x8A SFR_B2;         /* timer 0 low byte */
__sfr __at 0x8C SFR_B3;         /* timer 0 high byte */
__sfr16 __at 0xCDCC SFR_W0;     /* timer 2 */
__sfr16 __at 0x8C8A SFR_W1;     /* timer 0 */
__sfr32 __at 0x8C8ACDCC SFR_32; /* timer 0 and timer 2 */

/* The addresses in the reverse order. Bit 23 of this address list is set
   as well, because every address of an sfr is 0x80 or higher. */
__sfr32 __at 0xCCCD8A8C SFR_32R;

unsigned long g;
#endif

void
testWrite (void)
{
#if defined (__SDCC_mcs51)
  SFR_32 = 0x12345678;
  ASSERT (SFR_B0 == 0x78);
  ASSERT (SFR_B1 == 0x56);
  ASSERT (SFR_B2 == 0x34);
  ASSERT (SFR_B3 == 0x12);
  ASSERT (SFR_W0 == 0x5678);
  ASSERT (SFR_W1 == 0x1234);

  SFR_32R = 0xa1b2c3d4;
  ASSERT (SFR_B3 == 0xd4);
  ASSERT (SFR_B2 == 0xc3);
  ASSERT (SFR_B1 == 0xb2);
  ASSERT (SFR_B0 == 0xa1);
#endif
}

void
testRead (void)
{
#if defined (__SDCC_mcs51)
  SFR_32 = 0;
  SFR_B0 = 0x11;
  SFR_B1 = 0x22;
  SFR_B2 = 0x33;
  SFR_B3 = 0x44;
  g = SFR_32;
  ASSERT (g == 0x44332211);
  g = SFR_32R;
  ASSERT (g == 0x11223344);

  SFR_W0 = 0xa5a6;
  SFR_W1 = 0xa3a4;
  ASSERT (SFR_32 == 0xa3a4a5a6);
#endif
}

void
testModify (void)
{
#if defined (__SDCC_mcs51)
  SFR_32 = 0x00ffffff;
  SFR_32++;
  ASSERT (SFR_B3 == 0x01);
  ASSERT (SFR_W0 == 0x0000);

  SFR_B3 = 0x80;
  SFR_32 |= 0x01000000;
  ASSERT (SFR_B3 == 0x81);
#endif
}
