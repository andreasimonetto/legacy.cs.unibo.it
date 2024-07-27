/** \file msg.h
	Message Block module.
*/
#ifndef _MSG_H
#define _MSG_H
#include "tcb.h"

/** \defgroup MSG Message Block
	msg.h: A Message Block (MSG) is a fundamental microkernel structure for the
	message passing mechanism. This module is divided in two parts:
	- allocating/deallocating MSGs;
	- managing queues of MSGs;
\{ */

/** Maximum number of messages. */
#define MAXMESSAGES 128

/** Message Block structure. */
typedef struct msg_t {
	/** Pointer to next entry into message queue. */
	struct msg_t *m_next;
		
	/** Pointer to previous entry into message queue. */
	struct msg_t *m_prev;
		
	/** Sender of this message. */
	struct tcb_t *m_sender;
		
	/** Payload of the message. */
	unsigned message;
} msg_t;

/** \defgroup MSGmem Memory management
	Abstraction for	Message Block allocation/deallocation. Once the
	hardware is started, the Operating System is not yet available.
	So we can't use functions for dynamic memory allocation (these
	functions are usually provided by another Operating System module: 
	the Memory Manager). These module hide this problem, creating a fixed
	memory area for MSG allocation and maintaining a list of unused MSGs.
\{ */
/** Initialize the Message Block memory area.
	This function is called once, at the beginning of the main kernel
	program. Provides to initialize the structures needed by MSG
	related functions.
	\sa void initTcbs()
*/
void initMsgs();

/** Allocate a MSG, if possible.
	NOTE: The initial values of the allocated MSG is unknown.
	\return An unused MSG, if exists, NULL otherwise.
*/
msg_t* allocMsg();

/** Free a MSG.
	\param m A MSG previously allocated with \em allocMsg().
*/
void freeMsg(msg_t *m);

/** Get a MSG's identifier.
	\param m A MSG previously allocated with allocMsg().
	\return The identifier of this message.
*/
int getMsgID(const msg_t *m);
/** \} */


/** \defgroup MSGQ Message Queue
	Queues of messages are one of the basic data structures managed by any
	microkernel. These functions are useful for managing message queues.
	\par Example of Message Queue iteration:
	\include msg_queue.example
\{ */
/** Create an empty Message Queue.
	\return An empty MQ (i.e. pointer to NULL).
*/
msg_t* mkEmptyMessageQ();

/** Test whether or not a Message Queue is empty.
	\param mq A Message Queue.
	\return 1 if tq is empty, 0 otherwise.
*/
int emptyMessageQ(const msg_t *mq);

/** Enqueue a MB into a Message Queue.
	\param mq Pointer to a MQ.
	\param m_ptr Pointer to a MSG.
*/
void insertMessage(msg_t **mq, msg_t *m_ptr);

/** Remove a MSG from a queue.
	NOTE: The MSG you want to remove must be present in the queue. This 
	function doesn't search occurrences of \em m_ptr in the queue. The result
	of removing a MSG that's not present in the queue is unpredictable.
	\param mq Pointer to a MQ.
	\param m_ptr The MSG you want to remove.
	\return m_ptr, if removed, NULL otherwise.
*/
msg_t* outMessage(msg_t **mq, msg_t *m_ptr);

/** Push a message into a queue.
	Put the given message at the head of the queue.
	\param mq Pointer to a MQ.
	\param m_ptr The MSG to push.
*/
void pushMessage(msg_t **mq, msg_t *m_ptr);

/** Pop the first message in the queue which sender is t_ptr.
	\param mq Pointer to a MQ.
	\param t_ptr Pointer to the sender's TCB.
	\return The message removed, if any, NULL otherwise.
*/
msg_t* popMessage(msg_t **mq, tcb_t *t_ptr);

/** Return the outgoing MSG.
	\param mq Pointer to a MQ.
	\return The outgoing MSG, if exists, NULL otherwise.
*/
msg_t* headMessage(const msg_t *mq);

/** Search in a queue for a MSG.
	\param mq Pointer to a MQ.
	\param m Pointer to a Message.
	\return m, if found, NULL otherwise.
*/
msg_t* searchMessage(const msg_t *mq, const msg_t *m);
/** \} */
/** \} */

#endif
