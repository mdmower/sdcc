/* bug-4226.c
   Numeric escape sequences in wide string literals give the values of single
   elements. Values above 0xff keep all their bits, and values from 0x80 to
   0xff are not decoded as bytes of UTF-8.
 */

#include <testfwk.h>

#include <stddef.h> // For wchar_t
#if defined(__SDCC) || defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L && !defined(__APPLE__) && !defined(__OpenBSD__) // macOS and OpenBSD lack uchar.h.
#include <uchar.h> // For char16_t, char32_t
#define HAVE_UCHAR
#endif

#define ELEMENTS(a) (sizeof (a) / sizeof ((a)[0]))

#ifdef HAVE_UCHAR
static const char16_t s16[] = u"\x100\400\x7f\x80\xff\x7ff\xd7ff\xd800\xdfff\xffff\0X";
static const char16_t e16[] = {0x100, 0x100, 0x7f, 0x80, 0xff, 0x7ff, 0xd7ff, 0xd800, 0xdfff, 0xffff, 0, 'X', 0};

static const char32_t s32[] = U"\x100\400\x80\xff\xd800\x10000\x110000\x80000000\xffffffff\0X";
static const char32_t e32[] = {0x100, 0x100, 0x80, 0xff, 0xd800, 0x10000, 0x110000, 0x80000000, 0xffffffff, 0, 'X', 0};

/* Characters of the source and universal character names are code points. */
static const char16_t c16[] = u"A\xe9éé\x20ac€€";
static const char16_t ec16[] = {'A', 0xe9, 0xe9, 0xe9, 0x20ac, 0x20ac, 0x20ac, 0};

static const char32_t c32[] = U"\xe9éé\x1f600😀\U0001f600";
static const char32_t ec32[] = {0xe9, 0xe9, 0xe9, 0x1f600, 0x1f600, 0x1f600, 0};
#endif

static const wchar_t sw[] = L"\x80\xe9\xff\x100\2000\0X";
static const wchar_t ew[] = {0x80, 0xe9, 0xff, 0x100, 0x80, '0', 0, 'X', 0};

void
testChar16 (void)
{
#ifdef HAVE_UCHAR
  unsigned char i;

  ASSERT (ELEMENTS (s16) == ELEMENTS (e16));
  for (i = 0; i < ELEMENTS (e16); i++)
    ASSERT (s16[i] == e16[i]);

  ASSERT (ELEMENTS (c16) == ELEMENTS (ec16));
  for (i = 0; i < ELEMENTS (ec16); i++)
    ASSERT (c16[i] == ec16[i]);
#endif
}

void
testChar32 (void)
{
#ifdef HAVE_UCHAR
  unsigned char i;

  ASSERT (ELEMENTS (s32) == ELEMENTS (e32));
  for (i = 0; i < ELEMENTS (e32); i++)
    ASSERT (s32[i] == e32[i]);

  ASSERT (ELEMENTS (c32) == ELEMENTS (ec32));
  for (i = 0; i < ELEMENTS (ec32); i++)
    ASSERT (c32[i] == ec32[i]);
#endif
}

void
testWchar (void)
{
  unsigned char i;

  ASSERT (ELEMENTS (sw) == ELEMENTS (ew));
  for (i = 0; i < ELEMENTS (ew); i++)
    ASSERT (sw[i] == ew[i]);
}

/* An array with automatic storage duration, adjacent string literals, and a
   pointer to a string literal. */
void
testLocal (void)
{
#ifdef HAVE_UCHAR
  char16_t a16[] = u"A" u"\x1234" "\x80";
  const char32_t *p32 = U"\xe9\x12345678";

  ASSERT (ELEMENTS (a16) == 4);
  ASSERT (a16[0] == 'A');
  ASSERT (a16[1] == 0x1234);
  ASSERT (a16[2] == 0x80);
  ASSERT (a16[3] == 0);

  ASSERT (p32[0] == 0xe9);
  ASSERT (p32[1] == 0x12345678);
  ASSERT (p32[2] == 0);
#endif
}
