/* deref-ptr-array-elem.c
   For a read through a pointer that is an element of an array of pointers,
   SDCC generated mcs51 code that first stored the bytes of the pointer in
   the object that the pointer points to. It did so for arrays in data
   memory that are themselves an element of an array or a member of a
   struct.
 */

#include <testfwk.h>

struct s
{
  char c;
  char *names[2];
};

long v, v0;
char buf[3];
__data int w;

long * __data a[2][2];
__data struct s st;
int __data * __data np[2][2]; /* one-byte pointers on mcs51 */

long
readNested (void)
{
  return *a[1][1];
}

long
readFirst (void)
{
  return *a[0][0];
}

char
readMember (void)
{
  return *st.names[1];
}

int
readNear (void)
{
  return *np[1][0];
}

void
testNested (void)
{
  v = 0x11223344;
  v0 = 0x55667788;
  a[1][1] = &v;
  a[0][0] = &v0;

  ASSERT (readNested () == 0x11223344);
  ASSERT (v == 0x11223344);
  ASSERT (a[1][1] == &v);

  ASSERT (readFirst () == 0x55667788);
  ASSERT (v0 == 0x55667788);
  ASSERT (a[0][0] == &v0);
}

void
testMember (void)
{
  buf[0] = 0x55;
  buf[1] = 0x66;
  buf[2] = 0x77;
  st.names[1] = buf;

  ASSERT (readMember () == 0x55);
  ASSERT (buf[0] == 0x55);
  ASSERT (buf[1] == 0x66);
  ASSERT (buf[2] == 0x77);
  ASSERT (st.names[1] == buf);
}

void
testNear (void)
{
  w = 0x12f0;
  np[1][0] = &w;

  ASSERT (readNear () == 0x12f0);
  ASSERT (w == 0x12f0);
  ASSERT (np[1][0] == &w);
}
