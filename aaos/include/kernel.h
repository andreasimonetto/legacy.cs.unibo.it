/** \file kernel.h
	Kernel interface module.
*/
#ifndef _KERNEL_H
#define _KERNEL_H
#include "libumps.h"

/** \defgroup KInterface Kernel interface
	Thread's kernel interface. All the kernel internal
	functions and parameters can only be modified by recompiling the
	kernel itself. The only way to communicate with	the running kernel is 
	using this interface. You can request a service invoking
	\link SSIRequest SSIRequest() \endlink function: the implemented 
	services are explaned in this page. There is also a couple of 
	send/receive functions for message passing between threads.
\{ */

/** Magic number (it points to bad address in memory) for sending/receiving
messages to/from System Service Thread. */
#define SSI_MAGICNO  0x0FFFFFFF

/** Scheduler timeslice in microseconds. */
#define SCHED_TIMESLICE 5000

/** Scheduler pseudoclock tick in microseconds. */
#define PSEUDOCLOCK_INTERVAL 100000

/** Syscall mnemonic values. */
enum SyscallMsg {
	/** For message send. */
	SEND = 1,
	
	/** For message receive. */
	RECV = 2
};

/** For receiving messages from any sender. */
#define ANYMESSAGE 0

/** Service request values. These are services you can ask through SSIRequest
call.
	\sa SSIRequest
*/
enum ServiceRequest {
	/** Create a new thread whose initial processor state is passed
	by reference as service request \em payload as brother of the caller. */
	CREATEBROTHER = 1,
	
	/** Create a new thread whose initial processor state is passed
	by reference as service request \em payload as son of the caller. */
	CREATESON,
	
	/** Terminate the caller thread and all of its progeny in a recursive way. */
	TERMINATE,
	
	/** This request allows a thread to specify his own trap management
	thread for Program Trap exceptions. The trap management thread address
	is passed as service request \em payload; if the management thread does
	not exist, the requestor thread and its progeny will be terminated. Once
	assigned or inherited, the trap management thread cannot be redefined:
	further service requests of this type will cause the termination of the
	thread and its progeny. The field \em reply is not used. */
	SPECPRGMGR,
	
	/** This request allows a thread to specify his own trap management
	thread for TLB Management exceptions. The trap management thread address
	is passed as service request \em payload; if the management thread does 
	not exist, the requestor thread and its progeny will be terminated. Once
	assigned or inherited, the trap management thread cannot be redefined:
	further service requests of this type will cause the termination of the
	thread and its progeny. The field \em reply is not used. */
	SPECTLBMGR,
	
	/** This request allows a thread to specify his own trap management
	thread for SYSCALL/BREAK exceptions. The trap management thread address
	is passed as service request \em payload; if the management thread does
	not exist, the requestor thread and its progeny will be terminated. Once
	assigned or inherited, the trap management thread cannot be redefined:
	further service requests of this type will cause the termination of the
	thread and its progeny. This trap management thread will be called when 
	"pass up" of SYSCALLs and service requests is attempted. The field
	\em reply is not used. */
	SPECSYSMGR,
	
	/** Return the CPU time (in microseconds) used by the requesting thread.
	The field \em payload is not used. */
	GETCPUTIME,
	
	/** The nucleus mantains a pseudo-clock (a pseudo-device which raise
	an interrupt every 100 milliseconds). Upon request, the requesting
	thread is blocked until the next pseudo-clock tick. The fields 
	\em payload and \em reply are not used. */
	WAITFORCLOCK,
	
	/** Block the requesting thread, until the device specified by
	\em payload completes its I/O operation. If the device does not exist, 
	the thread and its progeny will be terminated. Return the STATUS
	register value of the device upon I/O completion. */
	WAITFORIO
};

/** Service reply values. These are the possible reply values upon SSIRequest
call.
	\sa SSIRequest
*/
enum ServiceReply {
	/** Indicate that thread creation went wrong. */
	CREATENOGOOD = 1,

	/** Indicate that message passing went wrong. */
	MSGNOGOOD
};

/** Commands for the SYSCALL/TLB/TRAP manager. After the trap handling the manager
should send this message to the trapped thread in order to continue it or
terminate it. */
enum TrapCommand {
	/** Terminate the trapped thread. */
	TRAPTERMINATE,
	
	/** Continue the trapped thread. */
	TRAPCONTINUE
};

/** Receive a message from a thread.
	\hideinitializer
	\param source Source thread. ANYMESSAGE for receiving from any source thread.
	\param reply Pointer to the reply.
	\return A pointer to the TCB of the sender.
*/
#define msg_recv(source, reply) (((tcb_t*)SYSCALL(RECV, (unsigned)(source), (unsigned)(reply), 0)))

/** Send a message to a thread.
	\hideinitializer
	\param dest Destination thread.
	\param payload Content of the message.
*/
#define msg_send(dest, payload) (SYSCALL(SEND, (unsigned)(dest), (unsigned)(payload), 0))

/** Perform a System Service request. This function sends a request to the 
	System Service Thread and block the caller until the service is done.
	\param service The service to require (one of ServiceRequest enumeration).
	\param payload The (optional) parameter for the service.
	\param reply The (optional) service output parameter.
	\sa ServiceRequest, ServiceReply
*/
void SSIRequest(unsigned service, unsigned payload, unsigned *reply);

/** \} */
#endif
