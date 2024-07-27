/* shading.h */

#define INFINITY        1E+37      /* infinity distance for z-buffer */
#define NUMANGLE        1024       /* number of angles in the lookup table */
#define FIXED_SHIFT     15
#define FIXED_SCALE     0x00008000
#define FIXED_HALF      0x00004000
#define ANGLE_SCALE     0x10000000
#define ANGLE_CONV      (NUMANGLE / M_PI_2)
#define ANGLE_INDEX     (1.0 / ANGLE_SCALE * ANGLE_CONV)
#define UNMARK          -1

#define FloatToFixed(X)       ((X) * FIXED_SCALE)
#define FloatToZnumber(X)     ((X))
#define AngleToFixed(A)       ((A) * ANGLE_SCALE)
#define IntToFixed(I)         ((I) << FIXED_SHIFT)
#define FixedToInt(X)         ((X) >> FIXED_SHIFT)
#define FixedToRInt(X)        (((X) + FIXED_HALF) >> FIXED_SHIFT)
#define FixedToFloat(X)       ((X) * 0.000004838022)  /* 1/FIXED_SCALE */
#define FixedToAngleIndex(X)  (int)((X) * ANGLE_INDEX)
#define FloatToAcosIndex(X)   (int)((X) * NUMANGLE)
#define SignedFloatToFixed(X) FloatToFixed(X)
#define DegToRad(A)           ((A) * M_PI/180.0)
#define Znumcmp(Z0,Z1)        ((Z0)-(Z1)<ztol)

#define SQR(X)                ((X)*(X))

typedef double ZNUMBER;
typedef int FIXED32;
typedef double FLOAT;
typedef FLOAT Vector[3];

#define DEF_GRCOL       239

typedef struct {
  int c[3]; /* R G B */
} ShColor;

typedef struct {
  int v[3];
  BOOL hasnorm;
} Facet;

typedef struct {
  int x[2];
  ZNUMBER z;
} PerspProj;

typedef struct _Facet_list {
  int f;
  struct _Facet_list *next;
} Facet_list;

typedef struct {
  Drawing_mode_t model; /* shading Gouraud/Phong */
  int nvertex;          /* number of vertices */
  point_r3 *vvertex;    /* vertices */
  int nfacet;           /* number of facets */
  Facet *vfacet;        /* link vertices/facets */
  Vector *vnorm_ver;    /* vertex normals */
  PerspProj *vproj_ver; /* vertex projections */
  REAL *valfa_ver;      /* alfa angles (normal in the vertex and view direction) */
  REAL *vbeta_ver;      /* beta angles (reflected ray and view direction) */
  ShColor *vcolor_ver;  /* vertex colours */

  unsigned int width;   /* frame-buffer width*/
  unsigned int height;  /* frame-buffer height*/
  BYTE *framebuffer;    /* pixel matrix image */
  BYTE **looky;         /* lookup table frame-buffer rows*/
  int *lookx;           /* lookup table frame-buffer clounms*/
  ZNUMBER *zbuffer;     /* z-buffer */
  ZNUMBER **zbuf_looky; /* lookup table z-buffer rows*/
  REAL *lookcos;        /* lookup table cos() */
  FIXED32 *lookacos;    /* lookup table acos() */
  REAL *lookpowcos;     /* lookup table pow(cos(),Gloss) */
  XImage *ximage;
  TWin_t *win;
  GC gc;
} ShData;
