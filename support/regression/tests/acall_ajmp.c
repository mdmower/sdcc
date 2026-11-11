/** acall and ajmp tests for mcs51.
*/
#include <testfwk.h>

__data unsigned char path;

void
branch (void)
{
#ifdef __SDCC_mcs51
  __asm
	acall	00001$
	ajmp	00002$
00001$:
	orl	_path, #0x01
	ret
00002$:
	orl	_path, #0x02
  __endasm;
#endif
}

void
testAcallAjmp (void)
{
#ifdef __SDCC_mcs51
  path = 0;
  branch ();
  ASSERT (path == 0x03);
#endif
}
