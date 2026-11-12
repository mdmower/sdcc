/** Relative branches of mcs51 to absolute addresses.

    The linker computes the displacement of such a branch. It made the
    displacement too low by 2 when a byte with a relocation was in front of
    the displacement in the same T line of the object file.
*/
#include <testfwk.h>

#ifdef __SDCC_mcs51
__data unsigned char path;
__data unsigned char count;

/* The assembler begins a new T line at each label, so the direct address
   and the displacement that follow a label are in one T line. Each
   subroutine sets one bit of path. A branch with a displacement that is too
   low by 2 arrives at the first of the two ret instructions behind it. */
void
branch (void)
{
  __asm
	lcall	pcr_sjmp
	lcall	pcr_djnz
	.area	PCRABS (ABS,CODE)
	.org	0xf800
pcr_sjmp:
	mov	a, _path		; 0xf800
	sjmp	0xf806			; 0xf802
	ret				; 0xf804
	ret				; 0xf805
	orl	_path, #0x01		; 0xf806
	ret				; 0xf809
pcr_djnz:
	djnz	_count, 0xf80f		; 0xf80a
	ret				; 0xf80d
	ret				; 0xf80e
	orl	_path, #0x02		; 0xf80f
	ret				; 0xf812
	.area	CSEG (CODE)
  __endasm;
}
#endif

void
testBranch (void)
{
#ifdef __SDCC_mcs51
  path = 0;
  count = 2;
  branch ();
  ASSERT (path == 0x03);
#endif
}
