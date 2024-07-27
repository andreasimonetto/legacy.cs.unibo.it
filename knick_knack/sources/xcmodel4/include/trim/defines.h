/* defines.h */

#ifndef __trim_defines_h
#define __trim_defines_h

#include <X11/X.h>

#define _TRIM_MINISO     1  /* min. number of isoparam. grid*/
#define _TRIM_MAXISO   300  /* max. number of isoparam. grid*/
#define _TRIM_MNISOD 0.0001  /* min. distance between two isoparam. lines*/
#define _TRIM_MXISOD   0.1  /* max. distance between two isoparam. lines */

#define _TRIM_MINW_SIZE 300 /* min. W and H for the axes visualization */
#define _TRIM_AXES_SIZE  61 /* dim. box axes XYZ */
#define _TRIM_AXES_OFF   18 /* offset respect to axes UV */

#define _TRIM_NGREY     101 /* number of grey levels [0..100] */

typedef double REAL;
typedef long BOOL;
typedef unsigned char BYTE;

#ifndef TRUE
#define TRUE  (1)
#define FALSE (0)
#endif

typedef int Poledge_t[2];
typedef int Polvert_t[4];

#ifndef _POINT2
#define _POINT2
typedef struct {
  REAL x, y;
} point_r2;

typedef struct {
  REAL	x, y, w;
} point_p2;
#endif

#ifndef _POINT3
#define _POINT3
typedef struct {
  REAL x, y, z;
} point_r3;

typedef struct {
  REAL x, y, z, w;
} point_p3;
#endif

typedef struct {
  REAL xmin,ymin,zmin,
       xmax,ymax,zmax;
} Bbox_t;

#define _TRIM_DEFWIN TRUE
#define _TRIM_DEFPIX FALSE
#define _TRIM_NO_ID  0

typedef struct {
  int xpos;             /* x screen position */
  int ypos;             /* y screen position */
  int vxmin;            /* x window origin */
  int vymin;            /* y window origin */
  unsigned int width;   /* window width */
  unsigned int height;  /* window height */
  Window winid;         /* window id */
  Window pixid;         /* pixmap id */
  Drawable id;          /* default id to be used */
  BOOL flag;            /* set id with winid or pixid */
} TWin_t;

typedef struct {
  BYTE c[3]; /* R G B */
} Color_t;

typedef enum {
  _TRIM_ADAPTIVE,  /* adaptive grid */
  _TRIM_UNIFORM    /* uniform grid */
} Sub_type_t;

typedef struct {
  REAL *u;  /* U iso. */
  REAL *v;  /* V iso. */
  int nu;   /* U num. iso. */
  int nv;   /* V num. iso. */
} Grid_par_t;

#define _TRIM_NOCRT  -1 /* crt parameter; not used */

typedef struct {
  Sub_type_t sub_type;  /* grid type */
  union {
    struct {
      int nu;           /* U num. iso  (uniform grid) */
      int nv;           /* V num. iso  (uniform grid) */
    } niso;
    REAL srt;           /* SRT nurbs (adaptive grid) */
  } sub_par;            /* grid parameters */
  REAL crt;             /* CRT nurbs (tessellation) */
} Trim_par_t;

typedef enum {
  _TRIM_NURBS,       /* NURBS surface */
  _TRIM_TNURBS       /* trimmed NURBS surface */
} Surf_type_t;

typedef struct {
  int npt;           /* num. of points */
  int nseg;          /* num. of segments */
  int npol;          /* num. of polygons */
  struct {
    point_r2 *pt;    /* tessellation points [1..npt] */
    Poledge_t *seg;  /* tessellation segments [1..nseg] */
    Polvert_t *pol;  /* tessellation polygons [1..npol] */
  } domain;
  struct {
    point_r3 *pt;    /* tessellation points [1..npt] */
    Poledge_t *seg;  /* tessellation segments [1..nseg] */
    Polvert_t *pol;  /* tessellation polygons[1..npol] */
  } surface;
  Grid_par_t *grid;  /* grid parameters */
  Trim_par_t par;    /* trimming curve parameters*/
} Trimmed_surface_t;

typedef enum {
  _TRIM_WIREFRAME,    /* wire frame */
  _TRIM_DEPTHCUEING,  /* depth cueing */
  _TRIM_HIDDENLINE,   /* hidden line */
  _TRIM_GOURAUD,      /* gouraud shading */
  _TRIM_PHONG,        /* phong shading */
  _TRIM_FLAT          /* flat shading */  
} Drawing_mode_t;

typedef enum {
  _TRIM_OBJSYS,  /* fixed light respect to the object */
  _TRIM_COPSYS   /* fixed light respect to the viewr */
} Light_sys_t;

typedef struct {
  REAL teta;  /* teta angle respect to XZ */
  REAL fi;    /* fi angle respect to XY */
} Light_pos_t;

typedef struct {
  Color_t color;     /* colour */
  REAL Ambient_rfl;  /* % of ambient reflected ligth */
  REAL Diffuse_rfl;  /* % of diffuse reflected light */
  REAL Specular_rfl; /* % of specular reflected light */
  REAL Gloss;        /* Phong exponent */
} Obj_attrib_t;

typedef struct {
  point_r3 cop; /* center of projection */
  point_r3 cs;  /* center of the sphere circumscribed */
  REAL r;       /* ray of the sphere */
  REAL di;      /* distance projection plane/COP (0.0 automatic) */ 
  REAL da;      /* half edge window (0.0 automatic) */ 
  REAL ox;      /* x coordinate window center*/ 
  REAL oy;      /* y coordinate window center */
} View_par_t;

typedef struct {
  Drawing_mode_t mode; /* visualization method */
  Light_sys_t sys;     /* light system */
  Light_pos_t pos;     /* light position */
                       /* parameters function for projection */
  void (*persp_par)(View_par_t *view, int vxmin, int vymin,
		    unsigned int widht, unsigned int height);
                       /* function projection */
  void (*persp_pro)(point_r3 *pt, int *vx, int *vy, REAL *pz);
} Visual_par_t;

#endif

