/* trimmed-nurbs.h */

#ifndef __trim_trimmed_nurbs_h
#define __trim_trimmed_nurbs_h


#ifndef _NURBS_SURFACE
#define _NURBS_SURFACE
typedef struct {
  char *name;               /* optional nurbs normal*/
  long deg_u, deg_v,        /* u & v degrees*/
       cpn_u, cpn_v,        /* no. of control points in u & v */
       uknot_no, vknot_no,  /* no. knots in u & v */
       users;               /* no. of share */
  point_p3 *ccp;            /* control point matrix */
  REAL *uknots,             /* knot array in u */
       *vknots;             /* knot array in v */
} nurbs_surface;
#endif

typedef struct {
  BOOL holes;                       /* type of regions */
  long num_curves;                  /* no. of root curves */
  struct _Trimming_curve_t *curves;  /* root curves */
} Trimming_tree_t;

typedef struct _Trimming_curve_t {
  long num_vertices;     /* no. of vertices */
  point_r2 *vertices;    /* curve vertices */
  Trimming_tree_t tree;  /* children curves */
} Trimming_curve_t;

typedef struct {
  int tot_ptd;           /* total points */       
  int tot_curves;        /* total curves */
  nurbs_surface *nurbs;  /* nurbs surfaces*/
  Trimming_tree_t *tree; /* tree of trimming curves*/
} Trimmed_nurbs_t;

#endif
