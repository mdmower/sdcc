/* strlit-ptr-init.c

   A pointer to void that is initialized with a string literal has the
   right level of indirection. For such a pointer into code memory the
   mcs51 port gave warning 18 (Initializer different levels of
   indirections).
 */

#ifdef TEST1
const void *p = "AB";
const void * const cp = "CD";
#ifdef __SDCC_mcs51
const void __code *q = "EF";
const void __code * const cq = "GH";
#endif
#endif
