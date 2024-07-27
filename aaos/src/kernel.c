/** \mainpage An AMIKaya Operating System
\section intro Introduction
AMIKaya is:
- a complete Operating System project specification developed by Enrico Cataldi
  and based on Kaya and AMIKE;
- designed as a microkernel system, to be developed in a number of phases;
  can be implemented using the uMPS simulator.
	
Double A O.S. (or A.A.O.S.) is an implementation of the AMIKaya specification
written by \em Andrea \em Simonetto.

\section phase1 Phase 1
Phase 1 is the first software layer of AMIKaya (below it there are only bare
hardware and ROM microcode). Phase 1 requires the development of \link TCB
Thread \endlink Queue and \link MSG Message \endlink Queue Managers.
Threads, messages and their queues are the basic data structures managed by
any microkernel.

\section phase2 Phase 2
\link KInterface The OS nucleus \endlink, including a message passing subsystem,
interrupt and exception event handling and the implementation of a System
Service Interface (SSI). These are the points developed in Phase 2:
	- basic initialization of the system after boot;
	- creation and termination of asynchronous sequential threads;
	- CPU scheduler and nucleus pseudo-clock;
	- exception and interrupt handlers;
	- message passing facilities;
	- thread sync primitives based on the message passing paradigm;
	- SSI initialization and start-up;
	- management of exceptions and I/O;
	- "pass up" of requests and events to upper levels;
	- system shutdown when no more serviceable threads exist.
*/

#include "tcb.h"
#include "msg.h"
#include "io_event.h"
#include "kernel.h"
#include "init.h"
#include "termio.h"


/*----------------------------------------------------------------------------*/
/*--- Kernel internal structures ---------------------------------------------*/
/*----------------------------------------------------------------------------*/

struct request_t { 
	unsigned service;
	unsigned payload;
	unsigned *reply;
};

/* Timeslice in clock ticks */
#define TIMESLICE_TK (timeslice_usec * TIMESCALE)

/* Reset thread's timeslice timer */
#define sched_reset_timeslice() (*((unsigned*)INTERVALTMR) = (TIMESLICE_TK + 0x67))

/*----------------------------------------------------------------------------*/
/*--- Kernel internal variables ----------------------------------------------*/
/*----------------------------------------------------------------------------*/

/* Exception vector for pre/post exception processor status */
static state_t *exc_vect = (state_t*)ROMPAGESTART;

/* Pointer to System Service TCB and init TCB */
static tcb_t *ssi_thread, *init_thread;

/* Current timeslice value in microseconds */
static unsigned timeslice_usec;

/* Queues of ready, wait, clock wait, I/O wait threads */
static tcb_t *ready_queue, *wait_queue, *clockwait_queue, *iowait_queue;

/* Current running thread */
static tcb_t *current_thread;

/* I/O events queue */
static ioevent_t *ioevent_queue;

/* When the last pseudoclock ticks */
static unsigned last_pseudoclock;

/* Pseudoclock interval in microseconds */
static unsigned pseudoclock_interval = PSEUDOCLOCK_INTERVAL;

/* Number of thread in scheduler */
static unsigned thread_count = 0;

/* Number of thread soft blocked by the scheduler */
static unsigned softblock_count = 0;

/*----------------------------------------------------------------------------*/
/*--- Kernel internal functions prototypes -----------------------------------*/
/*----------------------------------------------------------------------------*/

/* Initialize scheduler structures, setting the timeslice in microseconds */
static void sched_init(unsigned timeslice_usec);

/* Start the scheduler from the first thread in the ready queue */
static void sched_start();

/* Scheduler handle routine (for interrupt handle) */
static void sched_handler();

/* Test wether or not a thread is in the wait queue */
static int sched_is_waiting(const tcb_t *t);

/* Add a thread to the running queue */
static void sched_add(tcb_t *t);

/* Terminate a thread and all it's progeny */
static void sched_kill(tcb_t *t);

/* Wake a thread (move it from wait to ready queue) */
static void sched_wake(tcb_t *t);

/* Get the current running thread */
static tcb_t* current_thread;

/* Kill the current running thread and load the next thread in queue */
static void sched_kill_running();

/* Suspend (move in wait queue) the running thread and switches to the next */
static void sched_suspend_running(unsigned cktimer);

/* Put a thread in the clock wait status. */
static void sched_clockwait(tcb_t *t);

/* Put a thread in the I/O wait status */
static void sched_iowait(tcb_t *t);

/* Notify an I/O event */
static void sched_ionotify(unsigned *device_addr, unsigned status);

/* This function is for pseudoclock sync */
static void sched_update_pseudoclock();

/* Check the number of running processes, put the system in wait status and
	detect deadlock statuses */
static void sched_check_running();

/* Interrupt handling functions */
static void disk_handler();
static void tape_handler();
static void network_handler();
static void printer_handler();
static void terminal_handler();

/* Return the trap manager of t, if exist, NULL otherwise */
static tcb_t* get_trap_manager(tcb_t *t);

/* Return the TLB manager of t, if exist, NULL otherwise */
static tcb_t* get_tlb_manager(tcb_t *t);

/* Return the SYSCALL manager of t, if exist, NULL otherwise */
static tcb_t* get_syscall_manager(tcb_t *t);

/* Message Send SYSCALL routine */
static void syscall_msg_send(tcb_t *th, unsigned payload, tcb_t *sender);

/*----------------------------------------------------------------------------*/
/*--- Scheduler routines -----------------------------------------------------*/
/*----------------------------------------------------------------------------*/

static void sched_init(unsigned usec) {
	/* Init I/O events memory area */
	initIOEvents();
	
	/* Set the timeslice */
	timeslice_usec = usec;
	
	/* Create the ready queue */
	ready_queue = mkEmptyThreadQ();
	
	/* Create the wait queue */
	wait_queue = mkEmptyThreadQ();
	
	/* Create the I/O wait wait queue */
	iowait_queue = mkEmptyThreadQ();
	
	/* Create the clock wait queue */
	clockwait_queue = mkEmptyThreadQ();
	
	/* Create the I/O event queue */
	ioevent_queue = mkEmptyIOEventQ();
}

static void sched_start() {
	if(emptyThreadQ(ready_queue)) {
		term_puts(0, "No running threads at sched_start()\n");
		PANIC();
	}
	
	/* Store the value of time of day */
	last_pseudoclock = get_tod();
	
	/* Load the first thread in ready queue, reset its timeslice and
	pass it the cpu control */
	current_thread = headThread(ready_queue);
	sched_reset_timeslice();
	LDST(&(current_thread -> proc_state));
}

static void sched_handler() {
	sched_update_pseudoclock();
	sched_check_running();
	
	/* Get the next thread to run, set its timeslice and pass it
	the cpu control */
	if(current_thread) {
		current_thread -> proc_state = exc_vect[INT_STATUS];
		current_thread -> cpu_time += timeslice_usec;
		current_thread = current_thread -> t_next;
	}
	else
		current_thread = ready_queue;
	
	sched_reset_timeslice();
	LDST(&(current_thread -> proc_state));
}

static void sched_suspend_running(unsigned cktimer)
{
tcb_t *t_next = current_thread -> t_next;
	
	/* Calculate the thread's cpu timeslice */
	current_thread -> cpu_time += ((TIMESLICE_TK - cktimer) / TIMESCALE);
	
	/* Remove it from the ready queue and put it into the wait queue */
	outThread(&ready_queue, current_thread);
	insertThread(&wait_queue, current_thread);
	
	/* Here maybe the system fall in the wait status */
	sched_check_running();
	
	/* Otherwise run the next thread */
	current_thread = t_next;
	sched_update_pseudoclock();
	sched_reset_timeslice();
	LDST(&(current_thread -> proc_state));
}

static void sched_add(tcb_t *t) {
	/* First set TCB default parameters */
	t -> t_parent = t -> t_first_child = t -> t_last_child = t -> t_next_sibling = t -> t_prev_sibling = NULL;
	t -> trap_manager = t -> tlb_manager = t -> syscall_manager = 0;
	t -> cpu_time = 0;
	
	/* Then insert the new thread in the ready queue and increment the 
	thread counter */
	insertThread(&ready_queue, t);
	thread_count++;
}

static int sched_is_waiting(const tcb_t *t) {
	return searchThread(wait_queue, t) != NULL;
}

static void sched_wake(tcb_t *t) {
	outThread(&wait_queue, t);
	insertThread(&ready_queue, t);
}

static void sched_kill(tcb_t *t)
{
	while(!emptyChild(t)) {
		sched_kill(removeChild(t));
	}
	
	if(t == current_thread) {
		current_thread = current_thread -> t_next;
		outThread(&ready_queue, t);
	}
	else if(searchThread(ready_queue, t))
		outThread(&ready_queue, t);
	else if(searchThread(wait_queue, t))
		outThread(&wait_queue, t);
	else if(searchThread(iowait_queue, t)) {
		outThread(&iowait_queue, t);
		/* delete thread related pending I/O events */
		if(!emptyIOEventQ(ioevent_queue))
		{
		ioevent_t *ioevent = headIOEvent(ioevent_queue);
			
			do
			{
			ioevent_t *next_event = ioevent -> next;
				if(ioevent -> device_addr == ((unsigned*)(t -> proc_state.t1))) {
					*((unsigned*)(t -> proc_state.t2)) = ioevent -> status;
					freeIOEvent(outIOEvent(&ioevent_queue, ioevent));
				}
				
				ioevent = next_event;
			} while(!emptyIOEventQ(ioevent_queue) && ioevent != headIOEvent(ioevent_queue));
		}
		softblock_count--;
	}
	else if(searchThread(clockwait_queue, t)) {
		outThread(&clockwait_queue, t);
		softblock_count--;
	}
	else
		return;

	/* clear thread's inbox */
	while(!emptyMessageQ(t -> inbox))
		freeMsg(outMessage(&(t -> inbox), t -> inbox));

	freeTcb(t);
	thread_count--;
	sched_check_running();
}

static void sched_kill_running()
{
	/* Kill the current thread and run the next in the thread queue */
	sched_kill(current_thread);
	sched_update_pseudoclock();
	sched_reset_timeslice();
	LDST(&(current_thread -> proc_state));
}

static void sched_clockwait(tcb_t *t)
{
	if(!searchThread(wait_queue, t)) 
		return;
	
	/* Put the thread into the queue of "next clock tick waiters" */
	insertThread(&clockwait_queue, outThread(&wait_queue, t));
	softblock_count++;
}

static void sched_iowait(tcb_t *t)
{
	if(!searchThread(wait_queue, t)) 
		return;

	/* First check if there's a notice into I/O event queue for the wanted device */
	if(!emptyIOEventQ(ioevent_queue))
	{
	ioevent_t *ioevent = headIOEvent(ioevent_queue);
		
		do {
			if(ioevent -> device_addr == ((unsigned*)(t -> proc_state.t1))) {
				/* If found, wake the current thread, returning the status */
				*((unsigned*)(t -> proc_state.t2)) = ioevent -> status;
				freeIOEvent(outIOEvent(&ioevent_queue, ioevent));
				sched_wake(t);
				return;
			}
			
			ioevent = ioevent -> next;
		} while(ioevent != headIOEvent(ioevent_queue));
	}
	
	/* else, insert the thread into the I/O wait queue */
	
	insertThread(&iowait_queue, outThread(&wait_queue, t));
	softblock_count++;
	
	sched_update_pseudoclock();
}

static void sched_ionotify(unsigned *device_addr, unsigned status)
{
ioevent_t *ioevent;
	
	/* First check if there's a thread waiting for this I/O event notice */
	if(!emptyThreadQ(iowait_queue))
	{
	tcb_t *t = headThread(iowait_queue);
		
		do {
			if(((unsigned*)(t -> proc_state.t1)) == device_addr) {
				/* If found, put it into ready queue */
				*((unsigned*)(t -> proc_state.t2)) = status;
				insertThread(&ready_queue, outThread(&(iowait_queue), t));
				softblock_count--;
				return;
			}
			
			t = t -> t_next;
		} while(t != headThread(iowait_queue));
	} 
	
	/* else equeue the current event notice into the I/O event queue */
	
	ioevent = allocIOEvent();
	if(!ioevent) {
		term_puts(0, "ionotify: Unable to notify an I/O event (queue full)\n");
		PANIC();
	}
	
	ioevent -> device_addr = device_addr;
	ioevent -> status = status;
	insertIOEvent(&ioevent_queue, ioevent);
	
	sched_update_pseudoclock();
}

static void sched_update_pseudoclock()
{ 
unsigned tod = get_tod(); 
	
	/* If last stored Time Of Day differs from current TOD by the clock interval */
	if(tod - last_pseudoclock >= pseudoclock_interval) {
		/* Store current TOD */
		while(tod - last_pseudoclock > pseudoclock_interval)
			last_pseudoclock += pseudoclock_interval;
		
		/* Make the clockwait queue empty
			(moving the threads into ready queue) */
		while(!emptyThreadQ(clockwait_queue))
		{
		tcb_t *t = removeThread(&clockwait_queue);
			
			insertThread(&ready_queue, t);
			softblock_count--;
		}
	}
}

static void sched_check_running()
{
	if(!emptyThreadQ(ready_queue)) 
		return;
	
	current_thread = NULL;
	if(softblock_count > 0) {
		if(emptyThreadQ(clockwait_queue)) {
			/* Wait status */
			sched_reset_timeslice();
			setSTATUS(0x1000FF01);
			while(1);
		}
		else {
			/* Pseudoclock busy wait */
			while(!emptyThreadQ(clockwait_queue))
				sched_update_pseudoclock();
			
			/* Load first pseudoclock waiter */
			current_thread = headThread(ready_queue);
			LDST(&(current_thread -> proc_state));
		}
	}
	else if(thread_count > 1) {
		term_printf(0, "Detected deadlock status (%d threads in wait queue).\n", thread_count);
		PANIC();
	}
	else {
		term_puts(0, "No more threads alive.\n");
		HALT();
	}
}

/*----------------------------------------------------------------------------*/
/*--- Interrupts handling routines -------------------------------------------*/
/*----------------------------------------------------------------------------*/

/* Main interrupt dispatch routine */
static void interrupt_dispatch() {
	/* Set the program counter after the interrupt handle */
	exc_vect[INT_STATUS].pc = exc_vect[INT_STATUS].t9 = getEPC();
	
	/* Interrupt dispatch */
	if((exc_vect[INT_STATUS].cause >> (8 + TIMEINT)) & 0x1) 
		sched_handler();
	if((exc_vect[INT_STATUS].cause >> (8 + DISKINT)) & 0x1) 
		disk_handler();
	if((exc_vect[INT_STATUS].cause >> (8 + TAPEINT)) & 0x1) 
		tape_handler();
	if((exc_vect[INT_STATUS].cause >> (8 + NETWINT)) & 0x1) 
		network_handler();
	if((exc_vect[INT_STATUS].cause >> (8 + PRNTINT)) & 0x1) 
		printer_handler();
	if((exc_vect[INT_STATUS].cause >> (8 + TERMINT)) & 0x1) 
		terminal_handler();
	
	/* Restore the interrupted program */
	LDST(exc_vect + INT_STATUS);
}

static void disk_handler()
{
	/* Not implemented yet. */
	PANIC();
}

static void tape_handler()
{
	/* Not implemented yet. */
	PANIC();
}

static void network_handler() 
{
	/* Not implemented yet. */
	PANIC();
}

static void printer_handler() 
{
	/* Not implemented yet. */
	PANIC();
}

static void terminal_handler()
{
int i, term_id = -1;
unsigned *statusp;
	
	/* Find the terminal that raise this interrupt. */
	for(i = 0; i < MAX_TERMINALS && term_id == -1; i++) {
		if((*((volatile unsigned*)0x1000004C) >> i) & 0x1)
			term_id = i;
	}
	
	/* If terminal not found, kill thread and it's progeny */
	if(term_id == -1) {
		sched_kill_running();
	}
	
	statusp = (unsigned*) (TERMINAL0ADDR + (term_id * DEVREGSIZE) + (TRANSTATUS * DEVREGLEN));
	
	/* Notify the I/O event to the scheduler */
	sched_ionotify(statusp, (*statusp) & STATUSMASK);
	
	/* Acknowledge */		
	*((unsigned*) (TERMINAL0ADDR + (term_id * DEVREGSIZE) + (TRANCOMMAND * DEVREGLEN))) = ACK;
}

/*----------------------------------------------------------------------------*/
/*--- TLB handling routines --------------------------------------------------*/
/*----------------------------------------------------------------------------*/

static void tlb_handler()
{
	/* Not implemented yet. */
	PANIC();
}

static tcb_t* get_tlb_manager(tcb_t *t) {
	if(t -> tlb_manager)
		return (tcb_t*)(t -> tlb_manager);
	else if(t -> t_parent)
		return get_tlb_manager(t -> t_parent);
	
	return NULL;
}

/*----------------------------------------------------------------------------*/
/*--- SYSCALL handling routines ----------------------------------------------*/
/*----------------------------------------------------------------------------*/

static void syscall_handler()
{
unsigned timer_value = *((volatile unsigned*)INTERVALTMR) + 0xAF;
tcb_t *th = (tcb_t*)exc_vect[SYS_STATUS].a1; /* SEND/RECV caller's TCB */
tcb_t *syscall_manager;
	
	/* Move program counter of the caller to the next instruction */
	exc_vect[SYS_STATUS].pc = exc_vect[SYS_STATUS].t9 = getEPC() + 4;
	
	/* Request switch (first SYSCALL argument) */
	switch(exc_vect[SYS_STATUS].a0) {
		case SEND:
			/* If the message is TRAPCONTINUE or TRAPTERMINATE and the sender's trap manager is the current thread */
			if((exc_vect[SYS_STATUS].a2 == TRAPCONTINUE || exc_vect[SYS_STATUS].a2 == TRAPTERMINATE) && 
				((get_trap_manager(th) == current_thread) ||
				(get_tlb_manager(th) == current_thread) ||
				(get_syscall_manager(th) == current_thread)) && sched_is_waiting(th)) {
					
				if(exc_vect[SYS_STATUS].a2 == TRAPCONTINUE) {
					/* Continue the trapped thread */
					sched_wake(th);
				}
				else {
					/* Terminate the trapped thread */
					sched_kill(th);
					LDST(&(current_thread -> proc_state));
				}
			}
			else {
				/* Send the message */
				syscall_msg_send(th, exc_vect[SYS_STATUS].a2, current_thread);
			}
			break;
		case RECV:
			/* If dest of message is the System Service Thread */
			if(((unsigned)th) == SSI_MAGICNO) {
				/* Send it the request */
				syscall_msg_send(ssi_thread, exc_vect[SYS_STATUS].a2, current_thread);
			}
			
			/* If the receiver inbox isn't empty */
			if(!emptyMessageQ(current_thread -> inbox))
			{
			msg_t *msg = popMessage(&(current_thread -> inbox), th);
				
				/* Try to get the message directly */
				if(msg) {
					/* Set the return parameters and restart the SYSCALLer thread */
					*((unsigned*)exc_vect[SYS_STATUS].a2) = msg -> message;
					exc_vect[SYS_STATUS].v0 = (unsigned)(msg -> m_sender);
					freeMsg(msg);
					LDST(exc_vect + SYS_STATUS);
				}
			}
			
			/* If the message cannot be delivered directly, suspend the SYSCALLer */
			current_thread -> proc_state = exc_vect[SYS_STATUS];
			sched_suspend_running(timer_value);
			break;
		default:
			/* If current thread has got his own SYSCALL manager */
			syscall_manager = get_syscall_manager(current_thread);
			
			if(syscall_manager) {
				/* Send the SYSCALL notice to the manager */
				syscall_msg_send(syscall_manager, exc_vect[SYS_STATUS].cause, current_thread);
				
				/* Store sender's status and put it into the wait queue */
				current_thread -> proc_state = exc_vect[SYS_STATUS];
				sched_suspend_running(timer_value);
			}
			else {
				/* Unknown SYSCALL, kill the sender */
				sched_kill_running();
			}
			break;
	}

	/* Restore SYSCALLer process */
	LDST(exc_vect + SYS_STATUS);
}

static tcb_t* get_syscall_manager(tcb_t *t) {
	if(t -> syscall_manager)
		return (tcb_t*)(t -> syscall_manager);
	else if(t -> t_parent)
		return get_syscall_manager(t -> t_parent);
	
	return NULL;
}

static void syscall_msg_send(tcb_t *th, unsigned payload, tcb_t *sender)
{
	/* If the dest of this message is waiting */
	if(sched_is_waiting(th) &&
		th -> proc_state.a0 == RECV &&
		(th -> proc_state.a1 == (unsigned)sender ||
		th -> proc_state.a1 == ANYMESSAGE)) {
			/* Deliver the message and wake the waiting thread */
			th -> proc_state.v0 = (unsigned)sender;
			*((unsigned*)th -> proc_state.a2) = payload;
			sched_wake(th);
	}	
	else
	{
	msg_t *msg = allocMsg();
		
		/* Enqueue the message in receiver's inbox waiting for it's next msg_recv() call */
		msg -> message = payload;
		msg -> m_sender = sender;
		insertMessage(&(th -> inbox), msg);
	}
}

/*----------------------------------------------------------------------------*/
/*--- Program TRAP handling routines -----------------------------------------*/
/*----------------------------------------------------------------------------*/

static void trap_handler()
{
unsigned timer_value = *((volatile unsigned*)INTERVALTMR) + 0x9D;
tcb_t *trap_manager = get_trap_manager(current_thread);
	
	if(trap_manager) {
		/* "pass up" the trap */
		syscall_msg_send(trap_manager, exc_vect[TRAP_STATUS].cause, current_thread);
		sched_suspend_running(timer_value);
	}
	else {
		/* Kill the thread that raises the trap */
		sched_kill_running();
	}
	
	LDST(exc_vect + TRAP_STATUS);
}

static tcb_t* get_trap_manager(tcb_t *t) {
	if(t -> trap_manager)
		return (tcb_t*)(t -> trap_manager);
	else if(t -> t_parent)
		return get_trap_manager(t -> t_parent);
	
	return NULL;
}

/*----------------------------------------------------------------------------*/
/*--- SSI routines -----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

static void SSI_thread_main()
{
tcb_t *t, *sender;
struct request_t *request;

	while(1) {
		sender = msg_recv(ANYMESSAGE, &request);
		
		/* Set the caller's return value */
		sender -> proc_state.v0 = SSI_MAGICNO;
		
		/* Satisfy the request */
		switch(request -> service) {
			default:
			case TERMINATE:
				sched_kill(sender);
				break;
			case SPECPRGMGR:
				if(!(sender -> trap_manager)) {
					sender -> trap_manager = request -> payload;
					sched_wake(sender);
				}
				else
					sched_kill(sender);
				break;
			case SPECTLBMGR:
				if(!(sender -> tlb_manager)) {
					sender -> tlb_manager = request -> payload;
					sched_wake(sender);
				}
				else
					sched_kill(sender);
				break;
			case SPECSYSMGR:
				if(!(sender -> syscall_manager)) {
					sender -> syscall_manager = request -> payload;
					sched_wake(sender);
				}
				else
					sched_kill(sender);
				break;
			case CREATEBROTHER:
				t = allocTcb();
				if(t) {
					t -> t_parent = NULL;
					t -> proc_state = *((state_t*)(request -> payload));
					if(sender -> t_parent)
						insertChild(sender -> t_parent, t);
					
					sched_add(t);
					*(request -> reply) = (unsigned)t;
				}
				else {
					*(request -> reply) = (unsigned)CREATENOGOOD;
				}
				
				sched_wake(sender);
				break;
			case CREATESON:
				t = allocTcb();
				if(t) {
					t -> proc_state = *((state_t*)(request -> payload));
					insertChild(sender, t);
					sched_add(t);
					*(request -> reply) = (unsigned)t;
				}
				else {
					*(request -> reply) = (unsigned)CREATENOGOOD;
				}
				
				sched_wake(sender);
				break;
			case GETCPUTIME:
				*(request -> reply) = sender -> cpu_time;
				sched_wake(sender);
				break;
			case WAITFORCLOCK:
				sched_clockwait(sender);
				break;
			case WAITFORIO:
				sender -> proc_state.t1 = request -> payload;
				sender -> proc_state.t2 = (unsigned)(request -> reply);
				sched_iowait(sender);
				break;
		}
	}
}

void SSIRequest(unsigned service, unsigned payload, unsigned *reply)
{
struct request_t request;
	
	request.service = service;
	request.payload = payload;
	request.reply = reply;

	/* This function is mapped as a unique msg_recv() call to avoid thread
	scheduling between msg_send() and msg_recv() commands */
	msg_recv(SSI_MAGICNO, &request);
}

/*----------------------------------------------------------------------------*/
/*--- Main kernel function ---------------------------------------------------*/
/*----------------------------------------------------------------------------*/

int main()
{
const unsigned stack_size = PAGESIZE;
unsigned st = stack_size;
	
	/* Init TCB and messages area */
	initTcbs();
	initMsgs();
	
	/* Init the scheduler */
	sched_init(SCHED_TIMESLICE);
	
	/* Load the interrupt handler */
	STST(exc_vect + INT_HANDLER); 
	exc_vect[INT_HANDLER].pc = exc_vect[INT_HANDLER].t9 = (unsigned)interrupt_dispatch; 
	exc_vect[INT_HANDLER].sp -= st; 
	st += stack_size;
	
	/* Load the TLB handler */
	STST(exc_vect + TLB_HANDLER); 
	exc_vect[TLB_HANDLER].pc = exc_vect[TLB_HANDLER].t9 = (unsigned)tlb_handler; 
	exc_vect[TLB_HANDLER].sp -= st;
	st += stack_size;
	
	/* Load the program trap handler */
	STST(exc_vect + TRAP_HANDLER); 
	exc_vect[TRAP_HANDLER].pc = exc_vect[TRAP_HANDLER].t9 = (unsigned)trap_handler; 
	exc_vect[TRAP_HANDLER].sp -= st;
	st += stack_size;
	
	/* Load the SYSCALL/BREAK handler */
	STST(exc_vect + SYS_HANDLER); 
	exc_vect[SYS_HANDLER].pc = exc_vect[SYS_HANDLER].t9 = (unsigned)syscall_handler; 
	exc_vect[SYS_HANDLER].sp -= st;
	st += stack_size;

	/* Create the System Service Thread */
	ssi_thread = allocTcb();
	STST(&(ssi_thread -> proc_state));
	ssi_thread -> proc_state.status = 0x1000FF00;
	ssi_thread -> proc_state.pc = ssi_thread -> proc_state.t9 = (unsigned)SSI_thread_main;
	ssi_thread -> proc_state.sp -= st;	
	st += stack_size;
	
	/* Insert the System Service Thread into the scheduler's ready queue */
	sched_add(ssi_thread);

	/* Create the Test Thread */
	init_thread = allocTcb();
	STST(&(init_thread -> proc_state));
	init_thread -> proc_state.status = 0x1000FF04;
	init_thread -> proc_state.pc = init_thread -> proc_state.t9 = (unsigned)test;
	init_thread -> proc_state.ra = (unsigned)sched_suspend_running;
	init_thread -> proc_state.sp -= st * 2;
	st += stack_size;
	
	/* Insert the Test Thread into the scheduler's ready queue */
	sched_add(init_thread);
	
	/* Start the scheduler */
	sched_start();
	
	/* This point is never reached */
	return 0;
}
