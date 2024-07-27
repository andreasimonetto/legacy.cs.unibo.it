/* xmview.h */

#define MAXVER     600000 /* max. number of vertices */
#define MAXSEG     1250000 /* max. number of segments */
#define MAXFACE    1200000 /* max. number of facets */

#define XW             0 /* window X coordinate */
#define YW             0 /* window Y coordinate */
#define WW           802 /* window width */
#define HW           802 /* window height */
#define ROTSTEP       24 /* number of step to rotate the cop */

#define SQR(a)                ((a)*(a))

#define MEMORY_FAULT(test)      if( test ) { PANIC( "memory alloc fault\n" ); exit( EXIT_FAILURE ); }
#define GENERIC_FAULT(test,msg) if( test ) { PANIC1( "%s\n", msg ); exit( EXIT_FAILURE ); }
#define	PANIC(str)	        fprintf( stderr, "%s, %d : ", __FILE__, __LINE__ );\
			        fprintf( stderr, str )
#define	PANIC1(str,p)	        fprintf( stderr, "%s, %d : ", __FILE__, __LINE__ );\
			        fprintf( stderr, str, p )

#define	REMARK(fd)	      { char _c; while( (_c=fgetc(fd))=='#' ) while( fgetc(fd)!='\n' && !feof(fd) ); ungetc(_c,fd); }
#define	GET_VERTNO(fd,n)      if( fscanf( (fd), "Vertex %d", &(n) )<1 ) return FALSE;
#define	GET_VERTEX(fd,x,y,z)  if( fscanf( (fd), "%lf %lf %lf\n", &(x), &(y), &(z) )<3 ) return FALSE;
#define	GET_FACENO(fd,n)      if( fscanf( (fd), "Face %d", &(n) )<1 ) return FALSE;
#define	GET_FACE(fd,v0,v1,v2) if( fscanf( (fd), "%d %d %d\n", &(v0), &(v1), &(v2) )<3 ) return FALSE;

/* global functions */
int main( int, char ** );

/* local functions */
static void init_X( Display **, Colormap * );
static void new_window( Display *, TWin_t *, GC *, Colormap );
static Bool new(int v1, int v2, Poledge_t *seg, int nseg);
static BOOL read_surface_data( FILE *, Trimmed_surface_t ** );
static BOOL new_cop( point_r3 *, point_r3 *, int );
static void visual_do( Display *, TWin_t *, GC, Trimmed_surface_t ** );

