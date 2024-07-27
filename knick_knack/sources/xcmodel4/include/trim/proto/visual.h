/* visual.h */

/* global functions */
void Trim_init(Display *, Colormap);
void Trim_close(void);
void Trim_draw_domain(TWin_t *, BOOL );
void Trim_draw_axes(TWin_t *, Visual_par_t *, View_par_t *);
void Trim_draw_grid(TWin_t *, Grid_par_t *);
void Trim_draw_curve(TWin_t *, Trimming_curve_t *, BOOL, BOOL, REAL, REAL, REAL, REAL);
void Trim_draw_tree(TWin_t *, Trimmed_nurbs_t *, BOOL);
void Trim_draw_triangulation(TWin_t *, Trimmed_surface_t *);
void Trim_draw_trimmed_domain(TWin_t *, Trimmed_nurbs_t *, Trimmed_surface_t *);
void Trim_persp3_par(View_par_t *, int, int, unsigned int, unsigned int);
void Trim_persp3(point_r3 *, int *, int *, REAL *);
void Trim_default_attrib(Obj_attrib_t *);
void Trim_default_visual(Visual_par_t *);
int  Trim_visual_create(TWin_t *, Visual_par_t *, Obj_attrib_t *,
			Trimmed_surface_t **, int, int *);
int  Trim_visual_do(int, View_par_t *);
void Trim_visual_destroy(int);


/* local functions */
static void make_colors(Colormap);
static void clr_window(Drawable, int, int, unsigned int, unsigned int);
static int axes_x(TWin_t *);
static int axes_y(TWin_t *);
static unsigned int axes_w(TWin_t *);
static unsigned int axes_h(TWin_t *);
static void set_view(REAL, REAL, REAL, REAL, int, int, unsigned int, unsigned int);
static void view(REAL, REAL, int *, int *);
static void get_trimming_curves(Trimming_tree_t *, Trimming_curve_ref *, int *, BOOL);
static int get_newid(void);
static void make_visual_data(TWin_t *, Visual_par_t *, Obj_attrib_t *,
			     Trimmed_surface_t **, int, int);
static void wireframe_do(TWin_t *, Visual_par_t *, View_par_t *,
			 point_r3 *, Poledge_t *, int, int);

/* extern functions */
/* HID4 */
extern void _hidline_create(TWin_t *,
			    point_r3 *, Poledge_t *, Polvert_t *, int, int, int,
			    void **);
extern void _hidline_do(Visual_par_t *, View_par_t *, void *);
extern void _hidline_destroy(void *);

/* SHADING */
extern void _shading_create(TWin_t *, Drawing_mode_t, Obj_attrib_t *,
			    point_r3 *, Polvert_t *, int *, int, int, int,
			    void **);
extern void _shading_do(Visual_par_t *, View_par_t *,  Obj_attrib_t *, void *);
extern void _shading_destroy(void *);

