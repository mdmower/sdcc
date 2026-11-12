/** strlit-ptr-init.c
    A pointer that is not a character pointer and is initialized by data
    with a string literal got bytes of the compiler's own address of the
    literal instead of the label of the literal.
 */

#include <testfwk.h>

#include <stddef.h>

#ifdef __SDCC
/* SDCC accepts a wide string literal for a pointer to int or long with this warning */
#pragma disable_warning 244
#endif

struct s
{
  int i;
  const void *p;
};

const void *voidPtr = "AB";
const void *wideVoidPtr = L"CD";
const void * const constVoidPtr = "EF";
const void * const constWideVoidPtr = L"GH";

struct s var = { 1, "IJ" };
const struct s cst = { 2, L"KL" };
const void * const arr[2] = { "MN", L"OP" };

/* one literal for a pointer to void and a character pointer */
const void * const sharedVoidPtr = "QR";
const char * const sharedCharPtr = "QR";

#ifdef __SDCC
const int *intPtr = L"ST";
const long * const constLongPtr = L"UV";
#endif

#ifdef __SDCC_mcs51
const void __code *codePtr = "WX";
const void __code * const constCodePtr = "YZ";
#endif

#define NARROW(p) ((const char *) (p))
#define WIDE(p) ((const wchar_t *) (p))

void
testVoidPtr (void)
{
  ASSERT (NARROW (voidPtr)[0] == 'A');
  ASSERT (NARROW (voidPtr)[1] == 'B');
  ASSERT (NARROW (voidPtr)[2] == 0);

  ASSERT (WIDE (wideVoidPtr)[0] == L'C');
  ASSERT (WIDE (wideVoidPtr)[1] == L'D');
  ASSERT (WIDE (wideVoidPtr)[2] == 0);
}

void
testConstVoidPtr (void)
{
  ASSERT (NARROW (constVoidPtr)[0] == 'E');
  ASSERT (NARROW (constVoidPtr)[1] == 'F');
  ASSERT (NARROW (constVoidPtr)[2] == 0);

  ASSERT (WIDE (constWideVoidPtr)[0] == L'G');
  ASSERT (WIDE (constWideVoidPtr)[1] == L'H');
  ASSERT (WIDE (constWideVoidPtr)[2] == 0);

  ASSERT (NARROW (sharedVoidPtr)[0] == 'Q');
  ASSERT (NARROW (sharedVoidPtr)[1] == 'R');
  ASSERT (sharedCharPtr[0] == 'Q');
  ASSERT (sharedCharPtr[1] == 'R');
}

void
testAggregate (void)
{
  ASSERT (var.i == 1);
  ASSERT (NARROW (var.p)[0] == 'I');
  ASSERT (NARROW (var.p)[1] == 'J');

  ASSERT (cst.i == 2);
  ASSERT (WIDE (cst.p)[0] == L'K');
  ASSERT (WIDE (cst.p)[1] == L'L');

  ASSERT (NARROW (arr[0])[0] == 'M');
  ASSERT (NARROW (arr[0])[1] == 'N');
  ASSERT (WIDE (arr[1])[0] == L'O');
  ASSERT (WIDE (arr[1])[1] == L'P');
}

void
testIntPtr (void)
{
#ifdef __SDCC
  ASSERT (WIDE (intPtr)[0] == L'S');
  ASSERT (WIDE (intPtr)[1] == L'T');

  ASSERT (WIDE (constLongPtr)[0] == L'U');
  ASSERT (WIDE (constLongPtr)[1] == L'V');
#endif
}

void
testCodePtr (void)
{
#ifdef __SDCC_mcs51
  ASSERT (((const char __code *) codePtr)[0] == 'W');
  ASSERT (((const char __code *) codePtr)[1] == 'X');

  ASSERT (((const char __code *) constCodePtr)[0] == 'Y');
  ASSERT (((const char __code *) constCodePtr)[1] == 'Z');
#endif
}
