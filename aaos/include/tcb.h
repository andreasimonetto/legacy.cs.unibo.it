/** \file tcb.h
	Thread Control Block module.
*/
#ifndef _TCB_H
#define _TCB_H
#include "libumps.h"
#include "arch.h"

/** \defgroup TCB Thread Control Block
	tcb.h: A Thread Control Block (or TCB) is a structure that represents the
	status of a thread into the Operating System. TCBs are principally used
	by the scheduler. This module is divided in three sub-modules:
	- allocating/deallocating TCBs;
	- managing queues of TCBs;
	- managing trees of Threads.
\{ */

/** Maximum number of threads. */
#define MAXTHREADS 128

/** Thread Control Block structure. */
typedef struct tcb_t {
	/* Thread queue fields. */
	/** Pointer to next entry into thread queue. */
	struct tcb_t *t_next;
	/** \brief Pointer to previous entry into thread queue. */
	struct tcb_t *t_prev;
		
	/* Thread tree fields. */
	/** Parent thread. */
	struct tcb_t *t_parent;
	/** Next sibling thread. */
	struct tcb_t *t_next_sibling;
	/** Previous sibling thread. */
	struct tcb_t *t_prev_sibling;
	/** First child. */
	struct tcb_t *t_first_child;
	/** Last child. */
	struct tcb_t *t_last_child;
	
	/** Thread's message queue. */
	struct msg_t *inbox;
		
	/** Processor state. */
	state_t proc_state;
	
	/** Program total cpu time. */
	unsigned cpu_time;
	
	/** Program trap manager. */
	unsigned trap_manager;
	
	/** TLB manager. */
	unsigned tlb_manager;
	
	/** System call manager. */
	unsigned syscall_manager;
} tcb_t;

/** \defgroup TCBmem Memory management
	Abstraction for	Thread Control Block allocation/deallocation. Once the
	hardware is started, the Operating System is not yet available.
	So we can't use functions for dynamic memory allocation (these
	functions are usually provided by another Operating System module: 
	the Memory Manager). These module hide this problem, creating a fixed
	memory area for TCB allocation and maintaining a list of unused TCBs.
\{ */

/** Initialize the Thread Control Block memory area.
	This function is called once, at the beginning of the kernel
	program. It provides to initialize the structures needed by TCB
	related functions.
	\sa void initMsgs()
*/
void initTcbs();

/** Allocate a TCB, if possible.
	NOTE: The initial values of the allocated TCB is unknown.
	\return An unused TCB, if exists, NULL otherwise.
*/
tcb_t* allocTcb();

/** Free a TCB.
	\param t A TCB previously allocated with allocTcb().
*/
void freeTcb(tcb_t *t);

/** Get a TCB's identifier.
	\param t A TCB previously allocated with allocTcb().
	\return The identifier of this thread.
*/
int getTcbID(const tcb_t *t);
/** \} */


/** \defgroup TCBQ Thread Queue
	Queues of threads are one of the basic data structures managed by any
	microkernel. These functions are useful for managing thread queues.
	\par Example of Thread Queue iteration:
	\include tcb_queue.example
\{ */
/** Create an empty Thread Queue.
	\return An empty TQ (i.e. pointer to NULL).
*/
tcb_t* mkEmptyThreadQ();

/** Test whether or not a Thread Queue is empty.
	\param tq A Thread Queue.
	\return 1 if tq is empty, 0 otherwise.
*/
int emptyThreadQ(const tcb_t *tq);

/** Enqueue a TCB into a Thread Queue.
	\param tq Pointer to a TQ.
	\param t_ptr Pointer to a TCB.
*/
void insertThread(tcb_t **tq, tcb_t *t_ptr);

/** Extract the outgoing TCB from a queue.
	\param tq Pointer to a TQ.
	\return The outgoing TCB, if exists, NULL otherwise.
*/
tcb_t* removeThread(tcb_t **tq);

/** Remove a TCB from a queue.
	NOTE: The TCB you want to remove must be present in the queue. This 
	function doesn't search occurrences of \em t_ptr in the queue. The result
	of removing a TCB that's not present in the queue is unpredictable.
	\param tq Pointer to a TQ.
	\param t_ptr The TCB you want to remove.
	\return t_ptr, if removed, NULL otherwise.
*/
tcb_t* outThread(tcb_t **tq, tcb_t *t_ptr);

/** Return the outgoing TCB.
	\param tq Pointer to a TQ.
	\return The outgoing TCB, if exists, NULL otherwise.
*/
tcb_t* headThread(const tcb_t *tq);

/** Search in a queue for a TCB.
	\param tq Pointer to a TQ.
	\param t Pointer to a Thread Control Block.
	\return t, if found, NULL otherwise.
*/
tcb_t* searchThread(const tcb_t *tq, const tcb_t *t);
/** \} */


/** \defgroup TCBtree Thread Tree
	Management of a tree of thread. Actually, both user and kernel threads
	need to fork themselves in order to do their jobs. The result of these
	fork operations is a tree of threads, and this module is useful to
	manage the tree.
	\par Visit a thread tree:
	\include tcb_tree.example
	
\{ */
/** Test whether or not a thread has children.
	\param t_ptr Pointer to a Thread Control Block.
	\return 1 if \em t_ptr has children, 0 otherwise.
*/
int emptyChild(const tcb_t *t_ptr);

/** Append a child to a thread.
	\param parent The parent thread.
	\param t_ptr The child to append.
*/
void insertChild(tcb_t *parent, tcb_t *t_ptr);

/** Remove the first child of a thread.
	\param t_ptr Pointer to a TCB.
	\return The remove child, if present, NULL otherwise.
*/
tcb_t* removeChild(tcb_t *t_ptr);

/** Make a thread no longer the child of its parent.
	\param t_ptr Pointer to a TCB.
	\return \em t_ptr if it has a parent, NULL otherwise.
*/
tcb_t* outChild(tcb_t *t_ptr);
/** \} */
/** \} */

#endif
