/* bug-4163.c
   A struct initialized from a conditional expression whose operands are
   dereferenced pointers or array elements got its bytes from the address
   that the first bytes of the selected object formed.
 */

#include <testfwk.h>

typedef struct
{
  unsigned char x;
  unsigned char y;
} point;

point room1 = {11, 12};
point rooms[3] = {{21, 22}, {31, 32}, {41, 42}};
point result;

const point *
roomGet (void)
{
  return &room1;
}

/* the reported case */
void
initialiser (const point *room)
{
  point room2 = room ? *room : *roomGet ();

  result = room2;
}

void
derefs (unsigned char c, const point *p, const point *q)
{
  point room2 = c ? *p : *q;

  result = room2;
}

void
elements (unsigned char c, unsigned char i, const point *p)
{
  point room2 = c ? rooms[i] : p[1];

  result = room2;
}

void
variableAndElement (unsigned char c)
{
  point room2 = c ? room1 : rooms[1];

  result = room2;
}

void
testBug (void)
{
  point room = {5, 6};

  initialiser (&room);
  ASSERT (result.x == 5);
  ASSERT (result.y == 6);
  initialiser (0);
  ASSERT (result.x == 11);
  ASSERT (result.y == 12);

  derefs (1, &room, rooms);
  ASSERT (result.x == 5);
  ASSERT (result.y == 6);
  derefs (0, &room, rooms);
  ASSERT (result.x == 21);
  ASSERT (result.y == 22);

  elements (1, 2, rooms);
  ASSERT (result.x == 41);
  ASSERT (result.y == 42);
  elements (0, 2, rooms);
  ASSERT (result.x == 31);
  ASSERT (result.y == 32);

  variableAndElement (1);
  ASSERT (result.x == 11);
  ASSERT (result.y == 12);
  variableAndElement (0);
  ASSERT (result.x == 31);
  ASSERT (result.y == 32);
}
