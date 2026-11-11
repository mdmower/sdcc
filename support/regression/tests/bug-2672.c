/* bug-2672.c
   The mcs51 peephole optimizer applied rules 107 and 200.a, which remove a
   jump to the label that follows it, also inside a jump table, and removed
   one more entry from the end of the table in each pass over the rules.
   When the two highest case values of a switch shared the first block of
   the switch body, the program jumped into the middle of the first
   instruction of that block for the highest value.
 */

#include <testfwk.h>

#ifdef __SDCC_mcs51
#define DATA __data
#else
#define DATA
#endif

static DATA unsigned char g;
static volatile DATA unsigned char v;

/* The two highest values share the first block. */
static unsigned char
twoLast (unsigned char s)
{
  switch (s)
    {
    case 4:
    case 3:
      g = 0x33;
      return 0xa3;
    case 0:
      return 0xa0;
    case 1:
      return 0xa1;
    case 2:
      return 0xa2;
    }
  return 0xff;
}

/* The three highest values share the first block. */
static unsigned char
threeLast (unsigned char s)
{
  switch (s)
    {
    case 5:
    case 4:
    case 3:
      g = 0x44;
      return 0xb3;
    case 0:
      return 0xb0;
    case 1:
      return 0xb1;
    case 2:
      return 0xb2;
    }
  return 0xff;
}

/* Only the highest value belongs to the first block. */
static unsigned char
oneLast (unsigned char s)
{
  switch (s)
    {
    case 3:
      g = 0x55;
      return 0xc3;
    case 0:
      return 0xc0;
    case 1:
      return 0xc1;
    case 2:
      return 0xc2;
    }
  return 0xff;
}

/* The optimizer goes through the rules again after it has applied a rule
   marked restart. The number of assignments to v is such that the jump of
   the range check is in the range of sjmp only after the table has become
   shorter. The optimizer then applies rule 112.b to that jump in the second
   pass and examines the table a third time. */
#define V4 v = 1; v = 2; v = 3; v = 4;

static unsigned char
padded (unsigned char s)
{
  switch (s)
    {
    case 4:
    case 3:
      g = 0x33;
      break;
    case 0:
      g = 1;
      break;
    case 1:
      g = 2;
      break;
    case 2:
      g = 3;
      V4 V4 V4 V4 V4 V4 V4
      v = 5;
      break;
    }
  return g;
}

static const unsigned char twoRet[] = {0xa0, 0xa1, 0xa2, 0xa3, 0xa3, 0xff};
static const unsigned char twoG[] = {0x00, 0x00, 0x00, 0x33, 0x33, 0x00};
static const unsigned char threeRet[] = {0xb0, 0xb1, 0xb2, 0xb3, 0xb3, 0xb3, 0xff};
static const unsigned char threeG[] = {0x00, 0x00, 0x00, 0x44, 0x44, 0x44, 0x00};
static const unsigned char oneRet[] = {0xc0, 0xc1, 0xc2, 0xc3, 0xff};
static const unsigned char oneG[] = {0x00, 0x00, 0x00, 0x55, 0x00};
static const unsigned char paddedRet[] = {0x01, 0x02, 0x03, 0x33, 0x33, 0x00};

void
testBug (void)
{
  unsigned char i;

  for (i = 0; i < 6; i++)
    {
      g = 0;
      ASSERT (twoLast (i) == twoRet[i]);
      ASSERT (g == twoG[i]);
    }

  for (i = 0; i < 7; i++)
    {
      g = 0;
      ASSERT (threeLast (i) == threeRet[i]);
      ASSERT (g == threeG[i]);
    }

  for (i = 0; i < 5; i++)
    {
      g = 0;
      ASSERT (oneLast (i) == oneRet[i]);
      ASSERT (g == oneG[i]);
    }

  for (i = 0; i < 6; i++)
    {
      g = 0;
      ASSERT (padded (i) == paddedRet[i]);
    }
}
