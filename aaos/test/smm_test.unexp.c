#include "adt.h"
#include <stdio.h>

/*-- CUT POINT. DON'T REMOVE THIS LINE
#include <stdio.h>

/* STATIC MEMORY MANAGER TEST PROGRAM */

struct info {
	int id;
	char details[50];
	char authors[50];
};

#define TYPE struct info
#define BLK_SIZE 1024

DEF_ADT_SMM(TYPE, Int, BLK_SIZE)

int main()
{
int i;
TYPE *ptr[BLK_SIZE];

	printf("Initing memory area... ");
	initInts();
	
	printf("ok.\nNow, allocating all memory... ");
	for(i = 0; i < BLK_SIZE; i++) {
		if((ptr[i] = allocInt()) == NULL) {
			printf("\nerror: NULL pointer at %d!!\n", i);
			return 0;
		}
	}
	
	printf("ok.\nNow, freeing all memory... ");
	for(i = 0; i < BLK_SIZE; i++) 
		freeInt(ptr[i]);

	printf("ok.\nNow, reallocating all memory... ");
	for(i = BLK_SIZE - 1; i >= 0; i--) {
		if((ptr[i] = allocInt()) == NULL) {
			printf("\nerror: NULL pointer at %d!!\n", i);
			return 0;
		}
	}	

	printf("ok.\nFinally, refreeing all memory... ");
	for(i = 0; i < BLK_SIZE; i++) 
		freeInt(ptr[i]);
	
	printf("ok.\n");
	return 0;
}
