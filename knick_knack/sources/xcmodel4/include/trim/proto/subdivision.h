/* subdivision.h */

/* global functions */
BOOL _domain_subdivision(nurbs_surface *, Grid_t *, Sub_type_t, int *, int *, REAL);


/* local functions */
static USCITA *molt(REAL *, int);
static REAL inter1(point_r3 *, point_r3 *, point_p3 *);
static int search(REAL *, int, REAL);
static REAL search2(REAL *, int, int, REAL);
static void inssort(int, int);
static void ricerca(REAL *, int, int *, int *, int *, USCITA *, REAL, REAL, REAL, int);
static BOOL insert (REAL, REAL *, int *, USCITA *, int *,
                    point_p3 **, int, int *, int, BOOL);
static void v_periodic(REAL *, int *, int, int, int *, USCITA *, point_p3 **);
static void u_periodic(REAL *, int *, int, int, int *, USCITA *, point_p3 **);
static BOOL subdivide(PUNTO2 *, PUNTO2 *, int, int, REAL *, USCITA *,
                      int *, int *, point_p3 **, int *, int);
static BOOL surf_subdivision(point_p3 *, REAL *, REAL *, int, int, int, int, Grid_t *);
static void unif_subdivision(REAL *, REAL *, int, int, int, int, int *, int *, Grid_t *);

