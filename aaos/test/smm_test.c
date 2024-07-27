#include <stdio.h>

/* STATIC MEMORY MANAGER TEST PROGRAM */
struct info {
    int id;
    char details[50];
    char authors[50];
};
static struct info _Int_mem_block[1024];
static struct info *_Int_free_stack[1024];
static int _Int_free_stack_top;
void initInts()
{
    int i;

    _Int_free_stack_top = 0;
    for (i = 1024 - 1; i >= 0; i--) {
	_Int_free_stack[(_Int_free_stack_top)++] = _Int_mem_block + i;
    }
}
void freeInt(struct info *m)
{
    _Int_free_stack[(_Int_free_stack_top)++] = m;
} struct info *allocInt()
{
    struct info *m;

    if (!(_Int_free_stack_top))
	return 0;
    m = _Int_free_stack[--(_Int_free_stack_top)];
    return m;
}
int getIntID(const struct info *m)
{
    const int id = (m - _Int_mem_block);

    if (id < 0 || id >= 1024)
	return -1;
    return id;
}

int main()
{
    int i;
    struct info *ptr[1024];

    printf("Initing memory area... ");
    initInts();
    printf("ok.\nNow, allocating all memory... ");
    for (i = 0; i < 1024; i++) {
	if ((ptr[i] = allocInt()) == ((void *) 0)) {
	    printf("\nerror: NULL pointer at %d!!\n", i);
	    return 0;
	}
    }
    printf("ok.\nNow, freeing all memory... ");
    for (i = 0; i < 1024; i++)
	freeInt(ptr[i]);
    printf("ok.\nNow, reallocating all memory... ");
    for (i = 1024 - 1; i >= 0; i--) {
	if ((ptr[i] = allocInt()) == ((void *) 0)) {
	    printf("\nerror: NULL pointer at %d!!\n", i);
	    return 0;
	}
    }
    printf("ok.\nFinally, refreeing all memory... ");
    for (i = 0; i < 1024; i++)
	freeInt(ptr[i]);
    printf("ok.\n");
    return 0;
}
