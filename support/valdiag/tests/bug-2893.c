/* bug-2893.c

   A first token that cannot begin a translation unit is a syntax error, and
   it does not get the warning for an empty translation unit.
 */

#ifdef TEST1
; /* ERROR(SDCC) */ /* WARNING(GCC) */
int a;
#endif

#ifdef TEST2
} /* ERROR */
int b;
#endif

#ifdef TEST3
/* valdiag.py does not report additional messages for lines that are marked,
   so this line is not marked for SDCC. valdiag.py ignores messages that
   contain the text of this token. The syntax error is such a message, and
   the warning for an empty translation unit is not. */
'unreferenced function argument' /* IGNORE(GCC) */
#endif

#ifdef TEST4
/* A translation unit of comments and directives gets the warning. SDCC gives
   it for the line after the pragma, which is the last line that the
   preprocessor passes to the compiler. */
#ifdef __SDCC
#pragma std_c99
#else /* WARNING(SDCC) */
int c;
#endif
#endif
