#include "tcb.h"
#include "arch.h"
#include "adt.h"

/* */
DEF_ADT_SMM(tcb_t, Tcb, MAXTHREADS)

/* */
DEF_ADT_QUEUE(tcb_t, Thread, t_next, t_prev) 

/* */
int emptyChild(const tcb_t *t_ptr)
{
	return (t_ptr && t_ptr -> t_first_child == 0);
}

void insertChild(tcb_t *parent, tcb_t *t)
{
	if(!parent || !t)
		return;

	t -> t_prev_sibling = parent -> t_last_child;
	if(parent -> t_last_child)
		parent -> t_last_child -> t_next_sibling = t;
	else
		parent -> t_first_child = t;

	parent -> t_last_child = t;
	t -> t_parent = parent;
}

tcb_t* removeChild(tcb_t *t)
{
tcb_t *q;

	if(!t || !(t -> t_first_child))
		return 0;
	
	q = t -> t_first_child;
	if(q == t -> t_last_child)
		t -> t_first_child = t -> t_last_child = 0;
	else {
		t -> t_first_child = q -> t_next_sibling;
		t -> t_first_child -> t_prev_sibling = 0;
	}
	
	q -> t_parent = q -> t_next_sibling = q -> t_prev_sibling = 0;
	return q;
}

tcb_t* outChild(tcb_t *t)
{
	if(!t || !(t -> t_parent))
		return 0;
	
	if(t -> t_prev_sibling) {
		t -> t_prev_sibling -> t_next_sibling = t -> t_next_sibling;
	}
	else {
		t -> t_parent -> t_first_child = t -> t_next_sibling;
		t -> t_parent -> t_first_child -> t_prev_sibling = 0;
	}
	
	if(t -> t_next_sibling) {
		t -> t_next_sibling -> t_prev_sibling = t -> t_prev_sibling;
	}
	else {
		t -> t_parent -> t_last_child = t -> t_prev_sibling;
		t -> t_parent -> t_last_child -> t_next_sibling = 0;
	}
	
	t -> t_next_sibling = t -> t_prev_sibling = t -> t_parent = 0;
	return t;
}
