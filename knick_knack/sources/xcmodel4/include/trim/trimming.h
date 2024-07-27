/* trimming.h */

#define UNMARKED      2  /* segmento o punto non marcato */

#define MAX(A,B)     ((A) > (B) ? (A) : (B))
#define MIN(A,B)     ((A) < (B) ? (A) : (B))
#define SQR(A)       ((A)*(A))
#define RINT(x)      ((int)((x)+0.5))
#define DIST_2D(P,Q) sqrt(SQR((P).x-(Q).x) + SQR((P).y-(Q).y))
#define DIST_3D(P,Q) sqrt(SQR((P).x-(Q).x) + SQR((P).y-(Q).y) + SQR((P).z-(Q).z))


typedef enum {
  CURVPT, RIGHPT, DOWNPT, VERTPT
} Point_type_t;

typedef struct {
  REAL i0,i1;
  BOOL marked;
} Param_Interval_t;

typedef struct _Bt_node_t {
  REAL key;
  int ref;
  BOOL i0;
  struct _Bt_node_t *left;
  struct _Bt_node_t *right;
} Bt_node_t;

typedef struct {
  point_r3 pts;
  int pts_ref;
} Point_ref;

typedef struct {
  REAL x, y;
  BOOL hor;
  union {
    int u,v;
  } par;
} Int_point_t;

typedef union {
  int u[2];
  int v[2];
} Int_segment_t;

typedef enum {
  INTPOINT,     /* punto di intersezione con la griglia */
  TRIMPOINT     /* punto della curva di trimming */
} Int_type_t;

typedef struct {
  int k;
  Int_type_t t;
  REAL x, y;
} Curve_point_t;

typedef struct {
  int npt;
  int pages;
  Curve_point_t *page[PT_PAGENUM];
} Int_curve_t;

typedef struct {
  int ind1;
  int ind2;
  int ref;
  point_r2 p;
  Point_type_t type;
  BOOL used;
} Poly_pt_t;

typedef struct {
  int v[2];
  BOOL used;
} Poly_sg_t;

typedef struct {
  int v[4];
  BOOL used;
} Poly_t;

typedef struct {
  unsigned vert:2;
  int *vright;
  int *vdown;
  int nright;
  int ndown;
  int vert_i;
  int right_i;
  int down_i;
} Grid_cell_t;

typedef struct {
  Grid_cell_t **cell;
  REAL *u;
  REAL *v;
  int nv;
  int nu;
} Grid_t;

typedef struct {
  int npt;
  int pages;
  point_r2 *page[PT_PAGENUM];
} Surf_points_t;

typedef struct {
  int nseg;
  int pages;
  Poledge_t *page[SEG_PAGENUM];
} Surf_seg_t;

typedef struct {
  int npol;
  int pages;
  Polvert_t *page[POL_PAGENUM];
} Surf_pol_t;

