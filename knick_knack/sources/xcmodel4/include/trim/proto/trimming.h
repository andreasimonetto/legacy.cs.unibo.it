/* trimming.h */

/* global functions */
int Trim_nurbs_triangulation(nurbs_surface *nurbs_surf, Trim_par_t *par,
			     Trimmed_surface_t **tsurf);
int Trim_triangulation(Trimmed_nurbs_t *, Trim_par_t *par, 
		       Trimmed_surface_t **);
void Trim_new_surface(nurbs_surface *, Trimmed_surface_t *);
void Trim_delete_surface(Trimmed_surface_t *);
void Trim_surface_bbox(Trimmed_surface_t *, Bbox_t *);
void Trim_surface_list_bbox(Trimmed_surface_t **, int, Bbox_t *);
void Trim_set_params(Sub_type_t, int, int, REAL, REAL, Trim_par_t *);
void Trim_get_params(Trim_par_t *, Sub_type_t *, int *, int *, REAL *, REAL *);
void Trim_save_mbj(Trimmed_surface_t *, char *);


/* local functions */
static int intorno(REAL, REAL *);
static BOOL sulbordo(REAL, REAL);
static REAL normaeucl(REAL, REAL, REAL, REAL);
static int realcmp(const REAL *, const REAL *);
static int ptcmp(const Point_ref *, const Point_ref *);
static int param_search(REAL *, REAL, int, int);
static void param_u(REAL, REAL, Int_segment_t *, REAL *, REAL *, Sub_type_t);
static void param_v(REAL, REAL, Int_segment_t *, REAL *, REAL *, Sub_type_t);
static void insert_key(int, REAL, Bt_node_t **, BOOL);
static void build_tree(Param_Interval_t *, int, Bt_node_t **);
static void delete_tree(Bt_node_t *);
static void scan_tree(Param_Interval_t *, Bt_node_t *, REAL *, BOOL);
static void set_point(point_r2 *, REAL, REAL);
static point_r2 *new_point(int, int, int, Point_type_t);
static int *new_segment(int);
static int *new_polygon(int);
static void set_segment(int *, int, int, BOOL);
static void set_polygon(int *, int, int, int, BOOL);
static void insert_point(int);
static void insert_segment(int);
static void insert_polygon(int, int, BOOL);
static void mark_polygon(int, int, BOOL);
static BOOL check_polygon(int);
static void new_ptcurve(int, REAL, REAL, Int_type_t, Int_curve_t *);
static BOOL add_ptcurve(point_r2 *, int, int, int, int, REAL, Int_curve_t *);
static int  get_ptcurve(int, int, Int_curve_t *, REAL [MAXARCPT+3][2]);
static void quadra(int, int);
static void trimmatre(int, int, Int_curve_t *);
static void trimduestrana(int, int, Int_curve_t *);
static void trimmadue(int, int, Int_curve_t *);
static void trimmauno(int, int, Int_curve_t *);
static void intersec(int, point_r2 *, int *, BOOL *, Sub_type_t,
   	             Int_point_t [MAXGRIDINT], int *, int *, int [MAXGRIDINT][2]);
static void init_grid(Grid_t *);
static void free_grid(Grid_t *);
static void set_trim_grid(Grid_t *, Grid_par_t **); 
static BOOL mark_grid(int, point_r2 *, int *, Sub_type_t,
		      Int_point_t [MAXGRIDINT], int *, int [MAXGRIDINT][2], BOOL);
static BOOL scan_grid(Int_point_t [MAXGRIDINT], int *, int *);
static void init_curve(Int_curve_t *);
static void free_curve(Int_curve_t *);
static BOOL make_curve(int, point_r2 *, int *, Int_point_t [MAXGRIDINT], int *,
                       int [MAXGRIDINT][2], REAL, Int_curve_t *);
static void set_domain(BOOL, int, int);
static void trim_domain(Int_curve_t *, int *, int *, int *);
static void transf_domain(nurbs_surface *, Trimmed_surface_t *);
static void init_surface_data(Surf_points_t *, Surf_seg_t *, Surf_pol_t *);
static void free_surface_data(Surf_points_t *, Surf_seg_t *, Surf_pol_t *);
static void free_vects(point_r2 *, int *, int *, BOOL *);
static void new_trimmed_surface(Surf_points_t *spt, Surf_seg_t *, Surf_pol_t *,
				Trimmed_surface_t *);


/* extern functions */
/* TRIMMED-NURBS */
void _get_trimming_curves(Trimmed_nurbs_t *, point_r2 **, int **, int **, BOOL **, int *);
void _nurbs_surface_value(nurbs_surface *, REAL, REAL, point_r3 *);

/* SUBDIVISION */
BOOL _domain_subdivision(nurbs_surface *, Grid_t *, Sub_type_t, int *, int *, REAL);


