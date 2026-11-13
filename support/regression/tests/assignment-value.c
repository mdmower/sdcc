/** assignment-value.c
    The value of an assignment expression is the value of the left operand
    after the assignment, and it has the type of the left operand. When the
    right operand has the other signedness, the value that is used is the
    converted one.
 */

#include <testfwk.h>

#include <stdint.h>

#ifdef __SDCC
/* some of the constants below do not fit in the type of the left operand */
#pragma disable_warning 158
#endif

volatile uint8_t vu8;
volatile int8_t vs8;
volatile uint16_t vu16;
volatile int16_t vs16;
volatile uint32_t vu32;

uint8_t gu8;
int8_t gs8;
uint8_t au8[2];
struct { int8_t s; uint8_t u; } st;
uint8_t *pu8 = &gu8;
uint8_t idx = 1;

int8_t s8 = -1;
uint8_t u8 = 200;
int16_t s16 = -1;
uint16_t u16 = 50000u;
int32_t s32 = -1;

int16_t i;
int32_t l;

/* The compiler may not read a volatile left operand to get the value. */
void
testVolatile (void)
{
  i = vu8 = s8;
  ASSERT (i == 255);
  i = vs8 = u8;
  ASSERT (i == -56);

  l = vu16 = s16;
  ASSERT (l == 65535l);
  l = vs16 = u16;
  ASSERT (l == -15536l);

  ASSERT ((vu32 = s32) >> 31 == 1);
}

void
testConstant (void)
{
  i = vs8 = 100 + 100;
  ASSERT (i == -56);
  i = vu8 = 100 + 100;
  ASSERT (i == 200);

  i = vs8 = 30 * 8;
  ASSERT (i == -16);
  i = vu8 = -1;
  ASSERT (i == 255);
}

void
testObject (void)
{
  uint8_t u;
  int8_t s;

  i = u = s8;
  ASSERT (i == 255);
  ASSERT (u == 255);
  i = s = u8;
  ASSERT (i == -56);
  ASSERT (s == -56);

  i = gu8 = s8;
  ASSERT (i == 255);
  i = gs8 = u8;
  ASSERT (i == -56);

  i = *pu8 = s8;
  ASSERT (i == 255);
  i = au8[idx] = s8;
  ASSERT (i == 255);
  i = st.s = u8;
  ASSERT (i == -56);
}

/* The right operand is volatile, so it is read once. */
void
testVolatileRight (void)
{
  vs8 = -1;
  i = vu8 = vs8;
  ASSERT (i == 255);

  vu8 = 200;
  i = gs8 = vu8;
  ASSERT (i == -56);
  ASSERT (gs8 == -56);
}

int16_t
widen (int16_t x)
{
  return x;
}

int16_t
store (int8_t x)
{
  return vu8 = x;
}

void
testUse (void)
{
  ASSERT ((vs8 = u8) < 0);
  ASSERT ((vu8 = s8) == 255);

  i = (vu8 = s8) + 1;
  ASSERT (i == 256);
  i = (vs8 = u8) / 2;
  ASSERT (i == -28);
  i = (vu8 = s8) >> 1;
  ASSERT (i == 127);

  ASSERT (widen (vu8 = s8) == 255);
  ASSERT (store (-1) == 255);
}

void
testCompound (void)
{
  uint8_t u = 0;

  vu8 = 0;
  i = (vu8 += s8);
  ASSERT (i == 255);
  vu8 = 0;
  i = (vu8 -= 1);
  ASSERT (i == 255);

  vs16 = 0;
  l = (vs16 += u16);
  ASSERT (l == -15536l);

  i = (u += s8);
  ASSERT (i == 255);
}
