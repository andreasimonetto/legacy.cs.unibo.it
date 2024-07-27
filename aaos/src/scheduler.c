#include "scheduler.h"
#include "termio.h"
#include "msg.h"
#include "io_event.h"

/* Timeslice in clock ticks */
#define TIMESLICE_TK (timeslice_usec * TIMESCALE)

/* Reset the timeslice timer */
#define sched_reset_timeslice() (*((unsigned*)INTERVALTMR) = (TIMESLICE_TK + 0x67))

/* Current timeslice value in microseconds */
static unsigned timeslice_usec;

/* Queues of ready, wait, clock wait, I/O wait threads */
static tcb_t *ready_queue, *wait_queue, *clockwait_queue, *iowait_queue;

/* Current running thread */
static tcb_t *current_thread;

/* I/O events queue */
static ioevent_t *ioevent_queue;

/* Location where current running thread's processor status is stored */
static state_t *int_status = (state_t*)(ROMPAGESTART + INT_STATUS);

/* When the last pseudoclock ticks */
static unsigned last_pseudoclock;

/* Pseudoclock interval in microseconds */
static unsigned pseudoclock_interval = 100000;

/* Number of thread in scheduler */
static unsigned thread_count = 0;

/* Number of thread soft blocked by the scheduler */
static unsigned softblock_count = 0;

/* This function is for pseudoclock sync */
static void sched_update_pseudoclock();

/* Check the number of running processes, put the system in wait status and
	detect deadlock statuses */
static void sched_check_running();
