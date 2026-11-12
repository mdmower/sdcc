/* bug-3701.c
   Identifiers that have linkage could not be declared in a block more than
   once. The second declaration of an object or of a function got error 91,
   also when a macro declared the object each time it was used.
 */

#include <testfwk.h>

#include <stdint.h>

const uint8_t d0[2] = {1, 2};
const uint8_t d1[1] = {3};
const uint8_t *name_data[2] = {d0, d1};

#define NAME(i) extern const uint8_t *name_data[]; p = name_data[i]

static uint8_t
byMacro (void)
{
  const uint8_t *p;
  uint8_t sum;

  NAME (0);
  sum = p[1];
  NAME (1);
  sum += p[0];

  return sum;
}

static int
arrays (void)
{
  extern const uint8_t *name_data[];
  extern const uint8_t *name_data[];
  extern int values[];
  extern int values[3];
  extern int values[];

  return name_data[0][0] + values[1] + sizeof (values) / sizeof (values[0]);
}

void bump (void);

static int
objects (void)
{
  extern int counter, counter;
  extern int counter;

  counter = 5;
  extern int counter;
  counter++;
  bump ();
  extern int counter;

  return counter;
}

static int
hidden (void)
{
  int counter = 1;

  {
    extern int counter;
    extern int counter;
    counter = 20;
  }

  return counter;
}

static int
functions (void)
{
  int add (int, int);
  extern int add (int a, int b);
  int add (int, int);

  return add (3, 4);
}

int values[3] = {7, 8, 9};
int counter;

void
bump (void)
{
  counter++;
}

int
add (int a, int b)
{
  return a + b;
}

void
testBug (void)
{
  ASSERT (byMacro () == 5);
  ASSERT (arrays () == 12);
  ASSERT (objects () == 7);
  ASSERT (hidden () == 1);
  ASSERT (counter == 20);
  ASSERT (functions () == 7);
}
