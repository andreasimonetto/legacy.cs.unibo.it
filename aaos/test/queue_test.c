#include <stdio.h>

/* QUEUE TEST PROGRAM */
struct elem {
    struct elem *ptr_next, *ptr_prev;
    int val;
};
static struct elem _Element_mem_block[128];
static struct elem *_Element_free_stack[128];
static int _Element_free_stack_top;
void initElements()
{
    int i;

    _Element_free_stack_top = 0;
    for (i = 128 - 1; i >= 0; i--) {
	_Element_free_stack[(_Element_free_stack_top)++] =
	    _Element_mem_block + i;
    }
}
void freeElement(struct elem *m)
{
    _Element_free_stack[(_Element_free_stack_top)++] = m;
} struct elem *allocElement()
{
    struct elem *m;

    if (!(_Element_free_stack_top))
	return 0;
    m = _Element_free_stack[--(_Element_free_stack_top)];
    return m;
}
int getElementID(const struct elem *m)
{
    const int id = (m - _Element_mem_block);

    if (id < 0 || id >= 128)
	return -1;
    return id;
}
struct elem *mkEmptyElementQ()
{
    return ((void *) 0);
} int emptyElementQ(const struct elem *q)
{
    return (q == ((void *) 0));
} void insertElement(struct elem **q, struct elem *v)
{
    if (*q) {			/* Are there any element in the queue? */
	v->ptr_next = (*q)->ptr_next;
	v->ptr_next->ptr_prev = v;
	v->ptr_prev = *q;
	(*q)->ptr_next = v;
	*q = v;
    } else {
	v->ptr_prev = v->ptr_next = v;
	*q = v;
    }
}
struct elem *outElement(struct elem **q, struct elem *v)
{
    if (!q || !*q || !v)	/* Queue is empty or invalid pointer */
	return ((void *) 0);
    if ((*q)->ptr_next == *q) {	/* Queue has a single element */
	if (v != *q)
	    return ((void *) 0);
	*q = ((void *) 0);
	v->ptr_prev = v->ptr_next = ((void *) 0);
	return v;
    }
    if (!(v->ptr_prev) || !(v->ptr_next))	/* v has no prev or next field */
	return ((void *) 0);
    v->ptr_prev->ptr_next = v->ptr_next;
    v->ptr_next->ptr_prev = v->ptr_prev;
    if (v == *q)		/* Is v the tail of the queue? */
	*q = (*q)->ptr_prev;
    v->ptr_prev = v->ptr_next = ((void *) 0);
    return v;
}
struct elem *removeElement(struct elem **q)
{
    return ((!q || !*q) ? ((void *) 0) : outElement(q, (*q)->ptr_next));
} struct elem *headElement(const struct elem *q)
{
    if (!q)
	return ((void *) 0);
    else
	return q->ptr_next;
}
struct elem *searchElement(const struct elem *q, const struct elem *v)
{
    const struct elem *it;

    if (!emptyElementQ(q)) {
	it = q->ptr_next;
	do {
	    if (it == v)
		return (struct elem *) v;
	    it = it->ptr_next;
	} while (it != q->ptr_next);
    }
    return ((void *) 0);
}

/* Main queue test program */
int main()
{
    struct elem *t, *tq = mkEmptyElementQ();
    int finished = 0;
    char cmd;

    initElements();
    do {
	t = headElement(tq);
	if (!emptyElementQ(tq)) {
	    do {
		printf("%X ", (unsigned) t);
		t = t->ptr_next;
	    } while (t != headElement(tq));
	    putchar('\n');
	    t = tq;
	    do {
		printf("%X ", (unsigned) t);
		t = t->ptr_prev;
	    } while (t != tq);
	    putchar('\n');
	} else
	    puts("empty queue.");
	printf("Command: ");
	scanf("\n%c", &cmd);
	switch (cmd) {
	case 'e':
	    printf("empty queue: %d\n", emptyElementQ(tq));
	    break;
	case 'i':
	    t = allocElement();
	    if (t) {
		insertElement(&tq, t);
		printf("Element %X inserted\n", (unsigned) t);
	    } else {
		puts("Unable to allocate new thread");
	    }
	    break;
	case 'r':
	    t = removeElement(&tq);
	    printf("Element %X removed\n", (unsigned) t);
	    freeElement(t);
	    break;
	case 'o':
	    printf("Address of the Element to remove: ");
	    scanf("%X", (unsigned *) &t);
	    t = outElement(&tq, t);
	    if (t) {
		printf("Element %X removed\n", (unsigned) t);
		freeElement(t);
	    }
	    break;
	case 'h':
	    if ((t = headElement(tq)))
		printf("First Element: %X\n", (unsigned) t);
	    else
		puts("empty queue.");
	    break;
	case 'q':
	    finished = 1;
	    break;
	default:
	    puts("Unrecognized command.");
	    break;
	}
    } while (!finished);
    return 0;
}
