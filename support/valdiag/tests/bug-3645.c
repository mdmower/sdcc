/* bug-3645.c

   An array of character type initialized by an array object in code memory
   was taken for one initialized by a string literal, and the compiler
   crashed when it looked for the characters of the string.
 */

#ifdef TEST1
const unsigned char map[1];

typedef struct
{
  unsigned char x;
  const unsigned char maps[];
} test_struct;

test_struct test = {.x = 0, .maps = map}; /* ERROR */
#endif

#ifdef TEST2
const unsigned char map[]; /* WARNING(SDCC) */ /* IGNORE(GCC) */

typedef struct
{
  unsigned char x;
  const unsigned char maps[];
} test_struct;

test_struct test = {.x = 0, .maps = map}; /* ERROR */
#endif

#ifdef TEST3
const unsigned char map[1];

unsigned char a[2] = map; /* ERROR */
#endif

#ifdef TEST4
const unsigned char map[1];

unsigned char f (void)
{
  unsigned char a[2] = map; /* ERROR */

  return a[0];
}
#endif

#ifdef TEST5
const unsigned char map[1];

unsigned char f (void)
{
  unsigned char a[2] = {map}; /* ERROR */

  return a[0];
}
#endif

#ifdef TEST6
/* A string literal still is an initializer for all of these. */
typedef struct
{
  unsigned char x;
  const unsigned char maps[];
} test_struct;

test_struct test = {.x = 0, .maps = "ab"}; /* IGNORE(GCC) */

unsigned char a[3] = "ab";
unsigned char b[] = {"ab"};

unsigned char f (void)
{
  unsigned char c[3] = "ab";

  return c[0];
}
#endif
