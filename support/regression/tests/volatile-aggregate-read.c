/** volatile-aggregate-read.c
    Volatile members of structs and unions and volatile elements of arrays
    are read once for each read in the source, at the place of that read.
    The value of a read stays the same when the object changes later.
 */

#include <testfwk.h>

struct s
{
  unsigned char c;
  volatile unsigned char m;
  volatile unsigned int w;
};

struct in
{
  unsigned char c;
  unsigned char d;
};

struct out
{
  unsigned char c;
  volatile struct in in;
};

union u
{
  unsigned char b[2];
  volatile unsigned char m;
};

__data struct s st;
__data volatile unsigned char a[4];
__data volatile struct in vst;
__data struct out o;
__data union u un;

#ifdef __SDCC_mcs51
/* The mcs51 port examines the writes in C that follow a read of one of
   these objects before it selects where the value is kept. The tests
   therefore change the objects with instructions that the compiler does
   not examine, as an interrupt handler changes them. */
#define CHANGE_ST_M()   __asm__ ("mov (_st + 1),#0x22")
#define CHANGE_ST_W()   __asm__ ("mov (_st + 2),#0x22\nmov (_st + 3),#0x22")
#define CHANGE_A_1()    __asm__ ("mov (_a + 1),#0x22")
#define CHANGE_VST_C()  __asm__ ("mov _vst,#0x22")
#define CHANGE_O_IN_D() __asm__ ("mov (_o + 2),#0x22")
#define CHANGE_UN_M()   __asm__ ("mov _un,#0x22")
#else
#define CHANGE_ST_M()   (st.m = 0x22)
#define CHANGE_ST_W()   (st.w = 0x2222)
#define CHANGE_A_1()    (a[1] = 0x22)
#define CHANGE_VST_C()  (vst.c = 0x22)
#define CHANGE_O_IN_D() (o.in.d = 0x22)
#define CHANGE_UN_M()   (un.m = 0x22)
#endif

unsigned char g, h;
unsigned int gi, hi;

/* The value is used twice, and the object changes between the uses. */
static void
member (void)
{
  unsigned char x = st.m;

  g = x;
  CHANGE_ST_M ();
  h = x;
}

static void
member16 (void)
{
  unsigned int x = st.w;

  gi = x;
  CHANGE_ST_W ();
  hi = x;
}

static void
element (void)
{
  unsigned char x = a[1];

  g = x;
  CHANGE_A_1 ();
  h = x;
}

static void
memberOfVolatile (void)
{
  unsigned char x = vst.c;

  g = x;
  CHANGE_VST_C ();
  h = x;
}

static void
memberOfMember (void)
{
  unsigned char x = o.in.d;

  g = x;
  CHANGE_O_IN_D ();
  h = x;
}

static void
memberOfUnion (void)
{
  unsigned char x = un.m;

  g = x;
  CHANGE_UN_M ();
  h = x;
}

/* The value is used once, after the object has changed. */
static void
memberLater (void)
{
  unsigned char x = st.m;

  CHANGE_ST_M ();
  g = x + 1;
}

static void
member16Later (void)
{
  unsigned int x = st.w;

  CHANGE_ST_W ();
  gi = x + 1;
}

void
testUsedTwice (void)
{
  st.m = 0x11;
  member ();
  ASSERT (g == 0x11);
  ASSERT (h == 0x11);
  ASSERT (st.m == 0x22);

  st.w = 0x1111;
  member16 ();
  ASSERT (gi == 0x1111);
  ASSERT (hi == 0x1111);
  ASSERT (st.w == 0x2222);

  a[1] = 0x11;
  element ();
  ASSERT (g == 0x11);
  ASSERT (h == 0x11);
  ASSERT (a[1] == 0x22);

  vst.c = 0x11;
  memberOfVolatile ();
  ASSERT (g == 0x11);
  ASSERT (h == 0x11);
  ASSERT (vst.c == 0x22);

  o.in.d = 0x11;
  memberOfMember ();
  ASSERT (g == 0x11);
  ASSERT (h == 0x11);
  ASSERT (o.in.d == 0x22);

  un.m = 0x11;
  memberOfUnion ();
  ASSERT (g == 0x11);
  ASSERT (h == 0x11);
  ASSERT (un.m == 0x22);
}

void
testUsedLater (void)
{
  st.m = 0x11;
  memberLater ();
  ASSERT (g == 0x12);
  ASSERT (st.m == 0x22);

  st.w = 0x1111;
  member16Later ();
  ASSERT (gi == 0x1112);
  ASSERT (st.w == 0x2222);
}
