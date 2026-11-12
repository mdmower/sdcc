/* bug-4159.c

   An identifier after a struct or union tag was read as a typedef name when
   a typedef of that name was visible, so error 141 was given for a typedef
   that repeats an earlier typedef of the tag. C11 allows such a typedef when
   it denotes the same type.
 */

#ifdef TEST1
typedef struct s s_t;
typedef struct s s_t;
typedef union u u_t;
typedef union u u_t;
typedef struct d { unsigned char a; } d_t;
typedef struct d d_t;
#endif

#ifdef TEST2
struct s { unsigned char a; };
struct r { unsigned char a; };
typedef struct s s_t;	/* IGNORE */
typedef struct r s_t;	/* ERROR */
#endif

/* Standards before C11 do not allow a second typedef of a name. */
#ifdef TEST3_C99
typedef struct s s_t;	/* IGNORE */
typedef struct s s_t;	/* ERROR(SDCC) */ /* IGNORE(GCC) */
#endif

/* A typedef name after the tag is a second type specifier. */
#ifdef TEST4
typedef unsigned char t;
struct s { unsigned char a; };
struct s t x;		/* ERROR */
#endif
