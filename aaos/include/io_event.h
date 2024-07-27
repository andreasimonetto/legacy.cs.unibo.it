/** \file io_event.h
	I/O events management module.
*/
#ifndef _IO_EVENT_H
#define _IO_EVENT_H

/** \defgroup IOEvent I/O events
	This module permits the creation of I/O event notices and queues of
	I/O events.
\{ */

/** Maximum number of pending I/O events. */
#define MAX_IO_EVENT 128

/** I/O event structure. */
typedef struct ioevent_t {
	/** The device raising this event. */
	unsigned *device_addr;
	
	/** The code of the event. */
	unsigned status;
	
	/** Pointer to previous event into an I/O event queue. */
	struct ioevent_t *prev;
		
	/** Pointer to next event into an I/O event queue. */
	struct ioevent_t *next;
} ioevent_t;

/** \defgroup IOEvent_mem Memory management
	Allocation/deallocation of I/O events.
\{ */

/** Initialize the I/O events memory area.
	This function is called once, at the beginning of scheduler
	function. It provides to initialize the structures needed by
	I/O events related functions.
*/
void initIOEvents();

/** Allocate a I/O event, if possible.
	NOTE: The initial values of the allocated event is unknown.
	\return An unused I/O event, if exists, NULL otherwise.
*/
ioevent_t* allocIOEvent();

/** Free an I/O event.
	\param io_event An I/O event previously allocated with allocIOEvent().
*/
void freeIOEvent(ioevent_t *io_event);

/** Get an event's identifier.
	\param io_event An I/O event previously allocated with allocIOEvent().
	\return The identifier of this event.
*/
int getIOEventID(const ioevent_t *io_event);
/** \} */


/** \defgroup IOEventQ Queues of I/O events
	Queues of I/O events.
\{ */
/** Create an empty I/O Event Queue.
	\return An empty I/O event queue (i.e. pointer to NULL).
*/
ioevent_t* mkEmptyIOEventQ();

/** Test whether or not an event queue is empty.
	\param io_eventq An event queue.
	\return 1 if the queue is empty, 0 otherwise.
*/
int emptyIOEventQ(const ioevent_t *io_eventq);

/** Enqueue an event into an event queue.
	\param io_eventq Pointer to an event queue.
	\param io_event Pointer to an I/O event.
*/
void insertIOEvent(ioevent_t **io_eventq, ioevent_t *io_event);

/** Extract the outgoing event from a queue.
	\param io_eventq Pointer to an event queue.
	\return The outgoing I/O event, if exists, NULL otherwise.
*/
ioevent_t* removeIOEvent(ioevent_t **io_eventq);

/** Remove an I/O event from a queue.
	NOTE: The event you want to remove must be present in the queue. This 
	function doesn't search occurrences of \em io_event in the queue. The result
	of removing an event that's not present in the queue is unpredictable.
	\param io_eventq Pointer to a queue.
	\param io_event The I/O event you want to remove.
	\return io_event, if removed, NULL otherwise.
*/
ioevent_t* outIOEvent(ioevent_t **io_eventq, ioevent_t *io_event);

/** Return the last I/O event.
	\param io_eventq Pointer to a event queue.
	\return The outgoing event, if exists, NULL otherwise.
*/
ioevent_t* headIOEvent(const ioevent_t *io_eventq);

/** Search in a queue for an event.
	\param io_eventq Pointer to a queue.
	\param io_event Pointer to an I/O event.
	\return io_event, if found, NULL otherwise.
*/
ioevent_t* searchIOEvent(const ioevent_t *io_eventq, const ioevent_t *io_event);
/** \} */
/** \} */

#endif
