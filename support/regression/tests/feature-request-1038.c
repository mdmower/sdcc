/** Case ranges with many values (RFE #1038)
 */

#include <testfwk.h>

#ifdef __SDCC
_Pragma("save")
// case range expressions are a C2y feature
_Pragma("std_c2y")
#endif

// case range expressions were originally a GNU extension => test GNU-like host compilers, too
#if defined(__SDCC) || defined(__GNUC__)

/* More values than a jump table can have for some targets. Single cases are
   next to both ends of the wide range. The first range starts at the lowest
   value of the type and, where int has 16 bits, the last range ends at the
   highest. */
static unsigned char
classifyUnsigned (unsigned int v)
{
  switch (v)
    {
    case 0x0000 ... 0x000f:
      return 1;
    case 0x00ff:
      return 2;
    case 0x0100 ... 0x03ff:
      return 3;
    case 0x0400:
      return 4;
    case 0xfff0 ... 0xffff:
      return 5;
    default:
      return 0;
    }
}

static unsigned char
classifySigned (int v)
{
  switch (v)
    {
    case -32767 - 1 ... -32760:
      return 1;
    case -600 ... -101:
      return 2;
    case -100:
      return 3;
    case -1 ... 1:
      return 4;
    case 100 ... 399:
      return 5;
    case 32760 ... 32767:
      return 6;
    default:
      return 0;
    }
}

/* The cases have all values of the type but 11 to 99 and 201 to 249. */
static unsigned char
classifyChar (unsigned char v)
{
  switch (v)
    {
    case 0 ... 9:
      return 1;
    case 10:
      return 2;
    case 100 ... 199:
      return 3;
    case 200:
      return 4;
    case 250 ... 255:
      return 5;
    default:
      return 0;
    }
}

static unsigned char
classifyLong (long v)
{
  switch (v)
    {
    case -70000 ... -69997:
      return 1;
    case -2 ... 2:
      return 2;
    case 0x10000 ... 0x10007:
      return 3;
    case 0x10008:
      return 4;
    default:
      return 0;
    }
}

/* Few values and most of them are cases, which makes a jump table likely. */
static unsigned char
classifyDense (unsigned char v)
{
  switch (v)
    {
    case 0:
      return 10;
    case 1 ... 3:
      return 11;
    case 4:
      return 12;
    case 5:
      return 13;
    case 6 ... 9:
      return 14;
    case 10:
      return 15;
    case 11 ... 15:
      return 16;
    default:
      return 0;
    }
}

static inline unsigned char
inRange (unsigned int v)
{
  switch (v)
    {
    case 300 ... 339:
      return 1;
    default:
      return 0;
    }
}

/* The switch statement is copied to the caller. */
static unsigned char
callInline (unsigned int v)
{
  return inRange (v);
}

static unsigned char
classifyConstant (void)
{
  switch (0x03ff)
    {
    case 0x00ff:
      return 1;
    case 0x0100 ... 0x03ff:
      return 2;
    default:
      return 0;
    }
}

void
testUnsigned (void)
{
  ASSERT (classifyUnsigned (0x0000) == 1);
  ASSERT (classifyUnsigned (0x000f) == 1);
  ASSERT (classifyUnsigned (0x0010) == 0);
  ASSERT (classifyUnsigned (0x00fe) == 0);
  ASSERT (classifyUnsigned (0x00ff) == 2);
  ASSERT (classifyUnsigned (0x0100) == 3);
  ASSERT (classifyUnsigned (0x0101) == 3);
  ASSERT (classifyUnsigned (0x01ff) == 3);
  ASSERT (classifyUnsigned (0x0200) == 3);
  ASSERT (classifyUnsigned (0x03fe) == 3);
  ASSERT (classifyUnsigned (0x03ff) == 3);
  ASSERT (classifyUnsigned (0x0400) == 4);
  ASSERT (classifyUnsigned (0x0401) == 0);
  ASSERT (classifyUnsigned (0x8100) == 0);
  ASSERT (classifyUnsigned (0xffef) == 0);
  ASSERT (classifyUnsigned (0xfff0) == 5);
  ASSERT (classifyUnsigned (0xffff) == 5);
}

void
testSigned (void)
{
  ASSERT (classifySigned (-32767 - 1) == 1);
  ASSERT (classifySigned (-32760) == 1);
  ASSERT (classifySigned (-32759) == 0);
  ASSERT (classifySigned (-601) == 0);
  ASSERT (classifySigned (-600) == 2);
  ASSERT (classifySigned (-257) == 2);
  ASSERT (classifySigned (-256) == 2);
  ASSERT (classifySigned (-101) == 2);
  ASSERT (classifySigned (-100) == 3);
  ASSERT (classifySigned (-99) == 0);
  ASSERT (classifySigned (-2) == 0);
  ASSERT (classifySigned (-1) == 4);
  ASSERT (classifySigned (0) == 4);
  ASSERT (classifySigned (1) == 4);
  ASSERT (classifySigned (2) == 0);
  ASSERT (classifySigned (99) == 0);
  ASSERT (classifySigned (100) == 5);
  ASSERT (classifySigned (255) == 5);
  ASSERT (classifySigned (256) == 5);
  ASSERT (classifySigned (399) == 5);
  ASSERT (classifySigned (400) == 0);
  ASSERT (classifySigned (32759) == 0);
  ASSERT (classifySigned (32760) == 6);
  ASSERT (classifySigned (32767) == 6);
}

void
testChar (void)
{
  ASSERT (classifyChar (0) == 1);
  ASSERT (classifyChar (9) == 1);
  ASSERT (classifyChar (10) == 2);
  ASSERT (classifyChar (11) == 0);
  ASSERT (classifyChar (99) == 0);
  ASSERT (classifyChar (100) == 3);
  ASSERT (classifyChar (199) == 3);
  ASSERT (classifyChar (200) == 4);
  ASSERT (classifyChar (201) == 0);
  ASSERT (classifyChar (249) == 0);
  ASSERT (classifyChar (250) == 5);
  ASSERT (classifyChar (255) == 5);
}

void
testLong (void)
{
  ASSERT (classifyLong (-70001) == 0);
  ASSERT (classifyLong (-70000) == 1);
  ASSERT (classifyLong (-69997) == 1);
  ASSERT (classifyLong (-69996) == 0);
  ASSERT (classifyLong (-65536) == 0);
  ASSERT (classifyLong (-3) == 0);
  ASSERT (classifyLong (-2) == 2);
  ASSERT (classifyLong (0) == 2);
  ASSERT (classifyLong (2) == 2);
  ASSERT (classifyLong (3) == 0);
  ASSERT (classifyLong (0x0ffff) == 0);
  ASSERT (classifyLong (0x10000) == 3);
  ASSERT (classifyLong (0x10007) == 3);
  ASSERT (classifyLong (0x10008) == 4);
  ASSERT (classifyLong (0x10009) == 0);
  ASSERT (classifyLong (0x20000) == 0);
  ASSERT (classifyLong (0x7fffffff) == 0);
}

void
testDense (void)
{
  static const unsigned char expected[] =
    {10, 11, 11, 11, 12, 13, 14, 14, 14, 14, 15, 16, 16, 16, 16, 16, 0, 0};
  unsigned char i;

  for (i = 0; i < sizeof (expected); i++)
    ASSERT (classifyDense (i) == expected[i]);
  ASSERT (classifyDense (255) == 0);
}

void
testInline (void)
{
  ASSERT (callInline (299) == 0);
  ASSERT (callInline (300) == 1);
  ASSERT (callInline (339) == 1);
  ASSERT (callInline (340) == 0);
}

void
testConstant (void)
{
  ASSERT (classifyConstant () == 2);
}
#endif

#ifdef __SDCC
_Pragma("restore")
#endif
