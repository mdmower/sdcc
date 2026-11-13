/* bug-3857.c
   A struct assigned from a conditional expression whose third operand
   dereferences the pointer that a function returns got a wrong value when
   that operand was selected. The pointer was dereferenced twice.
 */

#include <testfwk.h>

typedef struct
{
  unsigned char x;
  unsigned char y;
} maths_point_u4;

maths_point_u4 room1 = {11, 12};
maths_point_u4 result;

const maths_point_u4 *
roomGet (void)
{
  return &room1;
}

/* the reported case */
void
positionAdjust (const maths_point_u4 *room, maths_point_u4 *room_new)
{
  *room_new = room ? *room : *roomGet ();
}

void
g1 (const maths_point_u4 *p)
{
  result = p ? *p : *roomGet ();
}

/* controls, a conditional expression of pointer type and an if statement */
void
g2 (const maths_point_u4 *p)
{
  result = *(p ? p : roomGet ());
}

void
g3 (const maths_point_u4 *p)
{
  if (p)
    result = *p;
  else
    result = *roomGet ();
}

void
testBug (void)
{
  maths_point_u4 room2 = {5, 6};
  maths_point_u4 room3 = {7, 8};

  positionAdjust (&room3, &room2);
  ASSERT (room2.x == 7);
  ASSERT (room2.y == 8);
  positionAdjust (0, &room2);
  ASSERT (room2.x == 11);
  ASSERT (room2.y == 12);

  g1 (&room3);
  ASSERT (result.x == 7);
  ASSERT (result.y == 8);
  g1 (0);
  ASSERT (result.x == 11);
  ASSERT (result.y == 12);

  g2 (&room3);
  ASSERT (result.x == 7);
  ASSERT (result.y == 8);
  g2 (0);
  ASSERT (result.x == 11);
  ASSERT (result.y == 12);

  g3 (&room3);
  ASSERT (result.x == 7);
  ASSERT (result.y == 8);
  g3 (0);
  ASSERT (result.x == 11);
  ASSERT (result.y == 12);
}
