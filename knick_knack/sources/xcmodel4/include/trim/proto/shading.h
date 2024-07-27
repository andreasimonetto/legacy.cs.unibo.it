/* visual.h */

/* global functions */
void _shading_create(TWin_t *, Drawing_mode_t, Obj_attrib_t *,
		     point_r3 *, Polvert_t *, int *, int, int, int, void **);
void _shading_do(Visual_par_t *, View_par_t *, Obj_attrib_t *, void *);
void _shading_destroy(void *);


/* local functions */
static BOOL check_facet(Facet *);
static BOOL check_angle(Vector, Vector);
static REAL dist_point_plane(point_r3 *, Facet *); 
static void calc_norm(Drawing_mode_t model);
static void reset_frame(ShData *data);
static void fill_gouraud_gs(Facet *, PerspProj *, ShColor *);
static void fill_phong_gs(Facet *, PerspProj *, REAL *, REAL *);
static void fill_gouraud_col(Facet *, PerspProj *, ShColor *);
static void fill_phong_col(Facet *, PerspProj *, REAL *, REAL *);
static void shading_init(TWin_t *, ShData *);

