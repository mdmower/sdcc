/* bug-3715.c
   __has_attribute, __has_cpp_attribute and __has_c_attribute stopped the
   preprocessor with an internal compiler error. It asked the compiler proper
   for the identifier of the attribute, which is a dummy in sdcpp. Bug #3844
   is the same failure.
 */

#include <testfwk.h>

/* Bug #3844, the fallback from the Clang documentation. */
#ifndef __has_attribute
#define __has_attribute(x) 0
#endif

#if __has_attribute(always_inline)
#define ATTR_ALWAYS_INLINE __attribute__((always_inline))
#else
#define ATTR_ALWAYS_INLINE
#endif

/* Bug #3715, a header that asks for one attribute after the other. */
#if defined(__has_attribute)
#  if __has_attribute(unused)
#    define ATTR_UNUSED __attribute__((unused))
#  endif
#  if !defined(ATTR_NODISCARD) && __has_attribute(warn_unused_result)
#    define ATTR_NODISCARD __attribute__((warn_unused_result))
#  endif
#endif

/* The standard form. */
#if !defined(ATTR_NODISCARD) && defined(__has_c_attribute)
#  if __has_c_attribute(nodiscard)
#    define ATTR_NODISCARD [[nodiscard]]
#  endif
#endif

#ifndef ATTR_UNUSED
#  define ATTR_UNUSED
#endif
#ifndef ATTR_NODISCARD
#  define ATTR_NODISCARD
#endif

/* An attribute that no compiler has. */
#if __has_attribute(sdcc_bug_3715) || __has_attribute (__sdcc_bug_3715__)
#  define UNKNOWN_GNU 1
#else
#  define UNKNOWN_GNU 0
#endif

#ifdef __has_cpp_attribute
#  if __has_cpp_attribute(sdcc_bug_3715)
#    define UNKNOWN_CPP 1
#  endif
#endif
#ifndef UNKNOWN_CPP
#  define UNKNOWN_CPP 0
#endif

#ifdef __has_c_attribute
#  if __has_c_attribute(sdcc_bug_3715) || __has_c_attribute ( sdcc_bug_3715 )
#    define UNKNOWN_STD 1
#  endif
/* An attribute prefix needs the scope token of C23. */
#  if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
#    if __has_c_attribute(sdcc_bug_3715::unknown)
#      define UNKNOWN_STD 1
#    endif
#  endif
#endif
#ifndef UNKNOWN_STD
#  define UNKNOWN_STD 0
#endif

ATTR_NODISCARD static unsigned char
three (void)
{
  return 3;
}

static inline ATTR_ALWAYS_INLINE unsigned char
twice (unsigned char c)
{
  return c * 2;
}

static unsigned char
first (unsigned char a, ATTR_UNUSED unsigned char b)
{
  return a;
}

void
testBug (void)
{
  ASSERT (twice (three ()) == 6);
  ASSERT (first (4, 5) == 4);

  ASSERT (!UNKNOWN_GNU);
  ASSERT (!UNKNOWN_CPP);
  ASSERT (!UNKNOWN_STD);
}
