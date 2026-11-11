/* bug-3703.c
   sdas8051 wrote the relocation of an acall or ajmp to an absolute address
   past the end of its relocation buffer when fewer than four bytes of the
   buffer were free. Where the text buffer follows the relocation buffer in
   memory, the T line in the object file then had a wrong address.
 */

#include <testfwk.h>

#ifdef __SDCC_mcs51
__data unsigned char d;
extern const __code unsigned char insns[];

/* This function is never called. The relocation buffer of the assembler is
   empty at the label. The two mov instructions leave two of its 16 bytes
   free, and the acall needs four. The linker accepts the address 0x0123
   because it places this code in the first 2K page. */
void
overrun (void) __naked
{
  __asm
_insns::
	mov	a, _d
	mov	a, _d
	acall	0x0123
  __endasm;
}
#endif

void
testBug (void)
{
#ifdef __SDCC_mcs51
  ASSERT (insns[4] == 0x31);
  ASSERT (insns[5] == 0x23);
#endif
}
