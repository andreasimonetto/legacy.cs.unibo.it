/** \file errno.h
	Errors handling module.
*/
#ifndef _ERRNO_H
#define _ERRNO_H

/** \defgroup errno Errors handling
	Constants and functions to catch/identify an error. A function can
	modify the value of \em errno in the event of an error to indicate
	what went wrong.
	
\{ */
/** Error identifier constants.
	These are the values that \em errno variable can take
	in order to identify an error.
*/
enum ErrorID {
	/** No errors. */
	SUCCESS,
	/** Terminal ID isn't valid. */
	ETERM_WRONGID,
	/** Terminal isn't available. */	
	ETERM_UNAVAIL 
};

/** Error number identifier.
*/
extern int errno;
/** \} */

#endif
