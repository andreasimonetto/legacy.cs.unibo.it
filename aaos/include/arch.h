/** \file arch.h
	Architecture constants and macros.
*/
#ifndef _ARCH_H
#define _ARCH_H
#include <stddef.h>
#include "libumps.h"
#include "tcb.h"

/** \defgroup arch Architecture constants
	arch.h: This constants are uMPS related.
\{ */

/** Page size in bytes. */
#define PAGESIZE 4096

/** Word size in bytes. */
#define WORDLEN 4

/** \internal */
#define PTEMAGICNO 0x2A

/** ROM Reserved Page. */
#define ROMPAGESTART 0x20000000

/** RAM Base address. */
#define RAMBASEADDR 0x10000000

/** Time Of the Day Base address. */
#define TODLOADDR 0x1000001C

/** \internal */
#define INTERVALTMR 0x10000020	

/** \internal */
#define TIMESCALEADDR 0x10000024

/** Vectors number. */
#define VECTSNUM 4

/** Exception vector type. */
enum ExcVectorTypes {
	INT_STATUS,
	INT_HANDLER,
	TLB_STATUS,
	TLB_HANDLER,
	TRAP_STATUS,
	TRAP_HANDLER,
	SYS_STATUS,
	SYS_HANDLER
};

/** Device interrupts. */
enum DeviceInts {
	TIMEINT = 2,
	DISKINT,
	TAPEINT,
	NETWINT,
	PRNTINT,
	TERMINT
};

/** Device register field length in bytes & regs per device. */
#define DEVREGLEN 4

/** Device register size in bytes. */
#define DEVREGSIZE 16

/** Device register field number for non-terminal devices. */
enum DeviceRegisters {
	STATUS,
	COMMAND,
	DATA0,
	DATA1
};

/** Device common STATUS codes. */
enum DeviceCommonStatus {
	UNINSTALLED = 0,
	READY = 1,
	BUSY = 3
};

/** Device common COMMAND codes. */
enum DeviceCommonCommands {
	RESET = 0,
	ACK = 1
};

/** \internal */
#define STATUSMASK 0x000000FF

/** \internal */
#define CAUSEMASK 0x000000FF

/** Timescale. */
#define TIMESCALE (*((unsigned*)TIMESCALEADDR))

/** Get time of day in microseconds. */
#define get_tod() ((*((volatile unsigned*)TODLOADDR)) / TIMESCALE)

/** \} */ 
#endif
