/* define.h */

#ifndef DEFINEH
#define DEFINEH 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>

typedef int BOOLEAN;

#ifndef TRUE
#define TRUE  (1)
#define FALSE (0)
#endif

#define HEPS 1.0E-08F
#define EPS 1.0E-15
#define FEPS 1.0E-7
/* #define EPS  DBL_EPSILON * DBL_EPSILON * DBL_EPSILON * DBL_EPSILON * DBL_EPSILON */
/* #define FEPS FLT_EPSILON * FLT_EPSILON * FLT_EPSILON * FLT_EPSILON * FLT_EPSILON // 1.0E-8 */

#define IS_ZERO(x)     ( (fabs( (x) ) < EPS) ? 1 : 0)
#define IS_NOT_ZERO(x) ( (fabs( (x) ) >= EPS) ? 1 : 0)
#define EQUAL(x,y)     ( (fabs((x) - (y)) < EPS ) ? 1 : 0)
#define NOT_EQUAL(x,y) ( (fabs((x) - (y)) >= EPS ) ? 1 : 0)
#define F_IS_ZERO(x)     ( (fabs( (x) ) < FEPS) ? 1 : 0)
#define F_IS_NOT_ZERO(x) ( (fabs( (x) ) >= FEPS) ? 1 : 0)
#define F_EQUAL(x,y)     ( (fabs((x) - (y)) < FEPS ) ? 1 : 0)
#define F_NOT_EQUAL(x,y) ( (fabs((x) - (y)) >= FEPS ) ? 1 : 0)


#define	MIN(a,b) ((a)<(b)?(a):(b))
#define MAX(a,b) ((a)>(b)?(a):(b))

#define MALLOC(ptr,type_ptr,size) \
	if ((ptr=(type_ptr)malloc(size))==NULL) \
		exit (111);

#define CALLOC(ptr,type,num) \
	if ((ptr = (type *)calloc((size_t)(num),sizeof(type))) == NULL) \
	    exit(111);

#define REALLOC(ptr,type_ptr,size) \
	if ((ptr = (type_ptr)realloc((void *)(ptr),size)) == NULL) \
	    exit(111);

#define FREE(ptr) \
	if (ptr != NULL) free((void *)(ptr));

#endif

