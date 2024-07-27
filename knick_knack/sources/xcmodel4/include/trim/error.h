/* error.h */

#ifndef __trim_error_h
#define __trim_error_h

#ifdef __trim_error_c
#define EXTERN
#else
#define EXTERN extern
#endif

#ifndef __trim_h
#include <stdio.h>
#include <stdarg.h>

#define _ERROR_MSG_LEN	256

#define ISERROR()		( Trim_error_number()!=_TRIM_ERR_NONE )
#define	RETURN_ON_ERROR()       if( ISERROR() ) return( Trim_error_number() )

#define MEMORY_FAULT(test)      if( test ) { PANIC( "memory alloc fault\n" ); exit( EXIT_FAILURE ); }
#define FOPEN_FAULT(test,name)	if( test ) { PANIC1( "cannot open file %s\n",name); exit( EXIT_FAILURE );}
#define GENERIC_FAULT(test,msg) if( test ) { PANIC1( "%s\n", msg ); exit( EXIT_FAILURE ); }

#ifdef TRIM_DEBUG
#define	TRACE(str)     	  /*fprintf( stderr, "%s, %d : ", __FILE__, __LINE__ );*/\
                            fprintf( stderr, str )
#define	TRACE1(str,p)	  /*fprintf( stderr, "%s, %d : ", __FILE__, __LINE__ );*/\
			    fprintf( stderr, str, p )
#define	TRACE2(str,p,q)	  /*fprintf( stderr, "%s, %d : ", __FILE__, __LINE__ );*/\
                            fprintf( stderr, str, p, q )
#define	TRACE3(str,p,q,r) /*fprintf( stderr, "%s, %d : ", __FILE__, __LINE__ );*/\
			    fprintf( stderr, str, p, q, r )
#else
#define	TRACE(str)
#define	TRACE1(str,p)
#define	TRACE2(str,p,q)
#define	TRACE3(str,p,q,r)
#endif

#define	PANIC(str)	fprintf( stderr, "%s, %d : ", __FILE__, __LINE__ );\
			fprintf( stderr, str )
#define	PANIC1(str,p)	fprintf( stderr, "%s, %d : ", __FILE__, __LINE__ );\
			fprintf( stderr, str, p )
#define PANIC2(str,p,q)	fprintf( stderr, "%s, %d : ", __FILE__, __LINE__ );\
			fprintf( stderr, str, p, q )
#endif

typedef enum {
  _TRIM_ERR_NONE = 0,
  _TRIM_ERR_FOPEN,
  _TRIM_ERR_FREAD,
  _TRIM_ERR_FWRITE,
  _TRIM_ERR_MEMORY,
  _TRIM_ERR_GENERIC,
  _TRIM_ERR_SRT,      /* srt valus is too small */
  _TRIM_ERR_GRID,     /* grid not useful for the trimming */
  _TRIM_ERR_CRT,      /* crt value too small */
  _TRIM_ERR_VIS,      /* visualization parameters not acceptable */
  _TRIM_ERR_ATTR,     /* object properties not acceptable */
  _TRIM_ERR_VIEW,     /* view parameters not accettable */
  _TRIM_ERR_INIT,     /* visual not inizialised */
  _TRIM_ERR_DISP      /* depth not supported */
} _TRIM_ERR_Tag;

#ifndef __trim_h
#define	_ErrorSet(a,b)	Trim_error_setmsg(__FILE__,__LINE__,a,b)

EXTERN void Trim_error_setmsg( char *, int, int , char *, ... );
#endif

EXTERN int Trim_error_number(void);
EXTERN char *Trim_error_msg(void);
EXTERN void Trim_error_clear(void);

#endif

