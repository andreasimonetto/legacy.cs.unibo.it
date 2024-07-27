#include <stdio.h>
#include "adt.h"

/*-- CUT POINT. DON'T REMOVE THIS LINE
#include <stdio.h>

/* QUEUE TEST PROGRAM */

#define MAX_ELEM 128

struct elem {
	struct elem *ptr_next, *ptr_prev;
	int val;
};

DEF_ADT_SMM(struct elem, Element, MAX_ELEM)

DEF_ADT_QUEUE(struct elem, Element, ptr_next, ptr_prev) 

/* Main queue test program */
int main()
{
struct elem *t, *tq = mkEmptyElementQ();
int finished = 0;	
char cmd;

	initElements();
	do {
		t = headElement(tq);
		if(!emptyElementQ(tq)) {
			do {
				printf("%X ", (unsigned)t);
				t = t -> ptr_next;
			} while(t != headElement(tq));
			
			putchar('\n');
			t = tq;
			do {
				printf("%X ", (unsigned)t);
				t = t -> ptr_prev;
			} while(t != tq);
			putchar('\n');
		}
		else
			puts("empty queue.");
		
		printf("Command: ");
		scanf("\n%c", &cmd);
		switch(cmd) {
			case 'e':
				printf("empty queue: %d\n", emptyElementQ(tq));
				break;
			case 'i':
				t = allocElement(); 
				if(t) {
					insertElement(&tq, t);
					printf("Element %X inserted\n", (unsigned)t);
				}
				else {
					puts("Unable to allocate new thread");
				}
				break;
			case 'r':
				t = removeElement(&tq);
				printf("Element %X removed\n", (unsigned)t);
				freeElement(t);
				break;
			case 'o':
				printf("Address of the Element to remove: ");
				scanf("%X", (unsigned*)&t);
				t = outElement(&tq, t);
				if(t) {
					printf("Element %X removed\n", (unsigned)t);
					freeElement(t);
				}
				break;
			case 'h':
				if((t = headElement(tq)))
					printf("First Element: %X\n", (unsigned)t);
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
	} while(!finished);
	return 0;
}
