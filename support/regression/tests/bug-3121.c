/* bug-3121.c
   In the initializer list of a struct that is initialized by code, as one
   with automatic storage is, a designator was honoured ahead of the first
   initializer without one only. Behind it, initializers went to the members
   in order whatever they designated, or were rejected ("initialization needs
   curly braces"). Each of several initializers for parts of one member or
   element zeroed the rest of it again, and one for a part of a member that an
   expression had initialized was dropped. In an array, elements ahead of a
   designated one were left uninitialized, and zeroed after all when their
   initializer came later in the list.

   The first case is the one of the ticket, the example of C defect report 413.
 */

#include <testfwk.h>

#include <string.h>

struct T {
	int k;
	int l;
};

struct S {
	int i;
	struct T t;
};

struct P {
	char a;
	char b;
	char c;
};

struct Q {
	char h;
	struct P p;
	char v[2];
	char t;
};

union U {
	struct P p;
	long n;
};

/* comment 5 of the ticket */
struct B {
	char a;
	union {
		char b;
		char c;
	};
};

struct T x = {.l = 43, .k = 42, };
struct P y = {7, 8, 9};

struct P ga = {1, .c = 3};
struct Q gq = {1, .p.b = 2, .p.c = 3, .v[1] = 4, 6};
char gv[4] = {[2] = 4, 5, [0] = 3};
const struct B cb = {.c = 42};

/* Each of these is called twice and leaves a pattern in its variables, so
   that a part the initializer does not reach is not zero by accident. */

static void
checkOverride (void)
{
	struct S l = {1, .t = x, .t.l = 41, };

	ASSERT (l.i == 1);
#ifdef __SDCC /* GCC initializes t anew for its second initializer */
	ASSERT (l.t.k == 42);
#endif
	ASSERT (l.t.l == 41);
	memset (&l, 0x5a, sizeof (l));
}

static void
checkBehind (void)
{
	struct P a = {1, .c = 3};
	struct P b = {.b = 2, 5, .a = 1};

	ASSERT (a.a == 1);
	ASSERT (a.b == 0);
	ASSERT (a.c == 3);
	ASSERT (b.a == 1);
	ASSERT (b.b == 2);
	ASSERT (b.c == 5);
	memset (&a, 0x5a, sizeof (a));
	memset (&b, 0x5a, sizeof (b));
}

static void
checkNested (void)
{
	struct Q q = {1, .p.b = 2, .p.c = 3, .v[1] = 4, 6};

	ASSERT (q.h == 1);
	ASSERT (q.p.a == 0);
	ASSERT (q.p.b == 2);
	ASSERT (q.p.c == 3);
	ASSERT (q.v[0] == 0);
	ASSERT (q.v[1] == 4);
	ASSERT (q.t == 6);
	memset (&q, 0x5a, sizeof (q));
}

static void
checkUnion (void)
{
	union U u = {.p.a = 1, .p.c = 3};
	struct B b = {.c = 42};

	ASSERT (u.p.a == 1);
	ASSERT (u.p.b == 0);
	ASSERT (u.p.c == 3);
	ASSERT (b.a == 0);
	ASSERT (b.c == 42);
	memset (&u, 0x5a, sizeof (u));
	memset (&b, 0x5a, sizeof (b));
}

static void
checkArray (void)
{
	char v[4] = {[2] = 4, 5, [0] = 3};
	struct P w[2] = {[1].a = 2, [1].c = 3};

	ASSERT (v[0] == 3);
	ASSERT (v[1] == 0);
	ASSERT (v[2] == 4);
	ASSERT (v[3] == 5);
	ASSERT (w[0].a == 0);
	ASSERT (w[0].b == 0);
	ASSERT (w[0].c == 0);
	ASSERT (w[1].a == 2);
	ASSERT (w[1].b == 0);
	ASSERT (w[1].c == 3);
	memset (v, 0x5a, sizeof (v));
	memset (w, 0x5a, sizeof (w));
}

static void
checkElement (void)
{
	struct P w[2] = {[1] = y, [1].b = 3};

	ASSERT (w[0].a == 0);
	ASSERT (w[0].c == 0);
#ifdef __SDCC /* as above */
	ASSERT (w[1].a == 7);
	ASSERT (w[1].c == 9);
#endif
	ASSERT (w[1].b == 3);
	memset (w, 0x5a, sizeof (w));
}

void
testOverride (void)
{
	checkOverride ();
	checkOverride ();
}

void
testStruct (void)
{
	checkBehind ();
	checkBehind ();
	checkNested ();
	checkNested ();
	checkUnion ();
	checkUnion ();
}

void
testArray (void)
{
	checkArray ();
	checkArray ();
	checkElement ();
	checkElement ();
}

void
testGlobal (void)
{
	ASSERT (ga.a == 1);
	ASSERT (ga.b == 0);
	ASSERT (ga.c == 3);

	ASSERT (gq.h == 1);
	ASSERT (gq.p.a == 0);
	ASSERT (gq.p.b == 2);
	ASSERT (gq.p.c == 3);
	ASSERT (gq.v[0] == 0);
	ASSERT (gq.v[1] == 4);
	ASSERT (gq.t == 6);

	ASSERT (gv[0] == 3);
	ASSERT (gv[1] == 0);
	ASSERT (gv[2] == 4);
	ASSERT (gv[3] == 5);

	ASSERT (cb.a == 0);
	ASSERT (cb.c == 42);
}
