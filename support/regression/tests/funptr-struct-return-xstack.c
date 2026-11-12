/** Calls through a pointer to a function that returns a struct or union.

    With stack-auto, the object for the value of each call is on the stack.
    For mcs51 with --xstack that is the external stack, and the caller
    removes the hidden pointer to the object from that stack after the call.
*/
#include <testfwk.h>

#if !defined(__SDCC_hc08) && !defined(__SDCC_s08) && !defined(SDCC_MOS) // struct return not yet supported
#if !defined(__SDCC_pdk13) && !defined(__SDCC_pdk14) // Lack of memory
struct s
{
  unsigned char a;
  int b;
  unsigned char c;
};

union u
{
  unsigned char a;
  long b;
};

struct s gs;

struct s
mk (void)
{
  struct s r;
  r.a = 0x12;
  r.b = 0x3456;
  r.c = 0x78;
  return r;
}

union u
mku (void)
{
  union u r;
  r.b = 0x12345678;
  return r;
}

/* The value of the call initializes a variable. */
struct s
viaVariable (struct s (*fp) (void))
{
  struct s r = fp ();
  return r;
}

/* The value of the call is returned. */
struct s
viaReturn (struct s (*fp) (void))
{
  return fp ();
}

/* Parameters and another variable are used after the call. */
int
viaAssign (struct s (*fp) (void), int x, int y)
{
  struct s r;
  int z = x - y;

  r = fp ();
  return r.b + z + x;
}

/* The value of the call is assigned to an object that is not on the stack. */
void
toStatic (struct s (*fp) (void))
{
  gs = fp ();
}

long
viaUnion (union u (*fp) (void))
{
  union u r;

  r = fp ();
  return r.b;
}

/* The stack pointers must have the same values before and after each call. */
unsigned int
manyCalls (struct s (*fp) (void))
{
  unsigned char i;
  unsigned int sum = 0;

  for (i = 0; i < 100; i++)
    sum += fp ().a;
  return sum;
}
#endif
#endif

void
testFunPtrStructReturn (void)
{
#if !defined(__SDCC_hc08) && !defined(__SDCC_s08) && !defined(SDCC_MOS) // struct return not yet supported
#if !defined(__SDCC_pdk13) && !defined(__SDCC_pdk14) // Lack of memory
  struct s r;

  r = viaVariable (mk);
  ASSERT (r.a == 0x12);
  ASSERT (r.b == 0x3456);
  ASSERT (r.c == 0x78);

  r.b = 0;
  r = viaReturn (mk);
  ASSERT (r.a == 0x12);
  ASSERT (r.b == 0x3456);
  ASSERT (r.c == 0x78);

  ASSERT (viaAssign (mk, 0x100, 0x30) == 0x3456 + 0xd0 + 0x100);

  gs.b = 0;
  toStatic (mk);
  ASSERT (gs.a == 0x12);
  ASSERT (gs.b == 0x3456);
  ASSERT (gs.c == 0x78);

  ASSERT (viaUnion (mku) == 0x12345678);

  ASSERT (manyCalls (mk) == 100 * 0x12);
#endif
#endif
}
