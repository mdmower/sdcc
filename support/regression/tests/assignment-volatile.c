/** assignment-volatile.c
    A volatile right operand of an assignment is read once, also when the
    value of the assignment is used.
 */

#include <testfwk.h>

#include <stdint.h>

/* right operands */
volatile uint8_t v8 = 200;
volatile uint16_t v16 = 50000u;
volatile uint32_t v32 = 0x89abcdeful;

/* left operands */
volatile uint8_t w8;
volatile uint16_t w16;
volatile uint32_t w32;

uint8_t g8;
uint16_t g16;
uint32_t g32;
uint8_t *p8 = &g8;

int16_t i;

static uint8_t
ret8 (void)
{
  return w8 = v8;
}

static uint16_t
arg16 (uint16_t a)
{
  return a + 1;
}

void
testValue (void)
{
  uint8_t c;
  uint16_t s;

  i = w8 = v8;
  ASSERT (i == 200);
  ASSERT (w8 == 200);

  g16 = w16 = v16;
  ASSERT (g16 == 50000u);
  ASSERT (w16 == 50000u);

  g32 = w32 = v32;
  ASSERT (g32 == 0x89abcdeful);
  ASSERT (w32 == 0x89abcdeful);

  g8 = 0;
  if ((c = v8) & 0x80)
    g8 = c;
  ASSERT (g8 == 200);

  g32 = s = v16;
  ASSERT (g32 == 50000ul);
  ASSERT (s == 50000u);

  g8 = 0;
  i = *p8 = v8;
  ASSERT (i == 200);
  ASSERT (g8 == 200);

  w8 = 0;
  ASSERT (ret8 () == 200);
  ASSERT (w8 == 200);

  ASSERT (arg16 (w16 = v16) == 50001u);
}

#ifdef __SDCC_stm8
/* The regression tests for stm8 run in uCsim, which has its simulator
   interface at 0x7fff. After the command 'I' with the parameter 'p', the
   interface answers with the bytes 2, 1, and 4, and each read of the
   interface gives the next byte. The byte that follows an expression
   shows how many times the expression read an object that has its last
   byte at 0x7fff. */
volatile __at (0x7fff) uint8_t sif;
volatile __at (0x7ffe) uint16_t sif16;
volatile __at (0x7ffc) uint32_t sif32;

uint8_t rest;

static void
sifStart (void)
{
  sif = 'I';
  sif = 'p';
}

/* The next byte of the answer is 1 after one read. The interface accepts
   the next command after the last byte of the answer is read. */
static uint8_t
sifNext (void)
{
  uint8_t next = sif;

  rest = sif;
  return next;
}

static uint8_t
sifRet (void)
{
  return w8 = sif;
}

static uint8_t
sifArg (uint8_t a)
{
  return a;
}
#endif

void
testReadOnce (void)
{
#ifdef __SDCC_stm8
  uint8_t c, next;
  uint16_t s;

  /* The interface answers the command '_' with '!'. */
  sif = '_';
  if (sif != '!')
    return;

  /* volatile left operand */
  sifStart ();
  i = w8 = sif;
  next = sifNext ();
  ASSERT (i == 2);
  ASSERT (w8 == 2);
  ASSERT (next == 1);

  /* local variable as the left operand */
  g8 = 0;
  sifStart ();
  if ((c = sif) & 2)
    g8 = c;
  next = sifNext ();
  ASSERT (g8 == 2);
  ASSERT (next == 1);

  /* left operand that is accessed through a pointer */
  g8 = 0;
  sifStart ();
  i = *p8 = sif;
  next = sifNext ();
  ASSERT (i == 2);
  ASSERT (g8 == 2);
  ASSERT (next == 1);

  /* return value and argument */
  sifStart ();
  c = sifRet ();
  next = sifNext ();
  ASSERT (c == 2);
  ASSERT (next == 1);

  sifStart ();
  c = sifArg (w8 = sif);
  next = sifNext ();
  ASSERT (c == 2);
  ASSERT (next == 1);

  /* 16 and 32 bits */
  sifStart ();
  g32 = s = sif16;
  next = sifNext ();
  ASSERT ((uint8_t) g32 == 2);
  ASSERT (g32 == s);
  ASSERT (next == 1);

  sifStart ();
  g16 = w16 = sif16;
  next = sifNext ();
  ASSERT ((uint8_t) g16 == 2);
  ASSERT (g16 == w16);
  ASSERT (next == 1);

  sifStart ();
  g32 = w32 = sif32;
  next = sifNext ();
  ASSERT ((uint8_t) g32 == 2);
  ASSERT (g32 == w32);
  ASSERT (next == 1);

  /* a statement, whose value is not used */
  sifStart ();
  w8 = sif;
  next = sifNext ();
  ASSERT (w8 == 2);
  ASSERT (next == 1);
#endif
}
