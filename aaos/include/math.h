/** \file math.h
	Mathematical routines module.
*/
#ifndef _MATH_H
#define _MATH_H

/** \defgroup math Mathematical routines
	math.h: NOT IMPLEMENTED YET.
\{ */
/** Calculate an absolute value.
	\param x An integer value.
	\return The absolute value of x.
*/
#define abs(x) ((x) > 0 ? (x) : -(x))

/** Calculate The minimum between two integers.
	\param a First integer value.
	\param b Second integer value.
	\return The minimum between \em a and \em b.
*/
#define	min(a,b)	((a) < (b) ? a : b)

/** Calculate The maximum between two integers.
	\param a First integer value.
	\param b Second integer value.
	\return The maximum between \em a and \em b.
*/
#define max(a,b)	((a) < (b) ? b : a)
/* \} */

#endif
