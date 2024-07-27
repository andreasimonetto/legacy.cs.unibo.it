/* const.h */

#define ZERO            1E-7 /* numeric tolerance; under this threshol the distance
				between two points is considered zero*/
#define TOL             1E-5 /* graphic tolerance; if two points to be visualized
				have a distance under this threshold,
                                the two points are considered the same point*/
#define ZTOL            1E-5 /* zbuffer tolerance*/
#define SPT             1E-5 /* numeric tolerance; under this threshold the distance
				between two 3D points is considered zero*/
#define ANGLETOL        (4.0*(M_PI/180.0)) /* tolerance for angles in rad */
#define AVERAGEPT       3 /* number of points used to compute the CRT value*/
#define MINANGLE        (2.0*(M_PI/180.0)) /* minimum angle in degree for curve segments
					      in the parametric domain*/
//#define MAXGRIDINT      2500 /* max. number of intersections curves/domain grid*/
#define MAXGRIDINT      5000 /* max. number of intersections curves/domain grid*/
#define MAXINTERV       1000  /* max. number of intervals to be subdivide*/
#define MAXARCPT        10   /* max. number of the original curve to be used to
				compute the intersection curve in a grid domain cell.*/
#define MAXCELLPT       (MAXARCPT+5)
                             /* max. number to be used to define a triangulation
			       for a grid domain cell*/
#define PT_PAGENUM      400  /* max. number of pages containing points*/
#define PT_PAGESIZE     256  /* number of points for page*/
#define SEG_PAGENUM     800  /* max. number of pages containing segments*/
#define SEG_PAGESIZE    512  /* number of segments for page*/
#define POL_PAGENUM     800   /* max. number of pages containing polygons*/
#define POL_PAGESIZE    256  /* number of polygons for page*/

#define BGCOL           0    /* black */
#define FGCOL           90   /* grey90 */
#define FGCOL1          80   /* grey80 */
#define FGCOL2          55   /* grey55 */

