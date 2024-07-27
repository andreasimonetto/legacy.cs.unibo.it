/* visual.h */

#ifdef __trim_visual_c
#define MAXVISUAL         100       /* max. number of active visual */

/* default parameters */
#define DEF_AMB_RFL      0.30       /* % of reflected ambient light */
#define DEF_DIFF_RFL     0.50       /* % of reflected diffuse light */
#define DEF_SPEC_RFL     0.20       /* % of reflected specular light */
#define DEF_GLOSS        10.0       /* Phong exponent */
#define DEF_VIS_MODE     _TRIM_DEPTHCUEING  /* visualization method */
#define DEF_LIGHT_SYS    _TRIM_COPSYS       /* light system of coordinate */
#define DEF_LIGHT_TETA -30.0        /* teta angle respect to XZ */
#define DEF_LIGHT_FI   -15.0        /* fi angle respect to XY */
#define DEF_RCOL         239        /* value R */
#define DEF_GCOL         239        /*  ""   G */
#define DEF_BCOL         239        /*  ""   B */

#define MAXGLOSS   100.0 /* max Phong exponent */

#define DC_START_GCOL 18 /* initial grey level for depth cueing */
#define DC_NUM_GCOL   81 /* number of levels used for depth cueing */

#define SQR(A)       ((A)*(A))
#define RINT(x)      ((int)((x)+0.5))
#define DIST_3D(P,Q) sqrt(SQR((P).x-(Q).x) + SQR((P).y-(Q).y) + SQR((P).z-(Q).z))
#define View_X(wx)   (RINT(scx*((wx)-wxmn)+vxmn))
#define View_Y(wy)   (RINT(scy*(wymn-(wy))+vymx))


typedef struct {
  Trimming_curve_t *curve;
  BOOL hole;
} Trimming_curve_ref;
#endif

typedef struct {
  int id;             /* visual id */
  int nsurf;          /* number of surfaces */
  int tpt;            /* total points */
  int tsg;            /* total segments */
  int tpl;            /* total polygons */
  int *vnpl;          /* number of polygon for surface */
  point_r3 *pt;       /* points [1..obj_tpt] */
  Poledge_t *sg;      /* segments [1..obj_tseg] */
  Polvert_t *pl;      /* polygons [1..obj_tpol] */
  Visual_par_t *vis;  /* visualization parameters */
  Obj_attrib_t *attr; /* object attributes */
  void *data;         /* shading or hidden-line data */
  TWin_t *win;
} Visual_data_t;

