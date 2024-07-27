/* trimmed-nurbs.h */

/* global functions */
void Trim_free_nurbs(nurbs_surface *);
int Trim_read_nurbs(FILE *, nurbs_surface **);
void Trim_nurbs_bbox(nurbs_surface *, Bbox_t *);
int Trim_read_trimmed_nurbs(FILE *, Trimmed_nurbs_t **);
void Trim_free_trimmed_nurbs(Trimmed_nurbs_t *);
void _get_trimming_curves(Trimmed_nurbs_t *, point_r2 **, int **, int **, BOOL **, int *);
void _nurbs_surface_value(nurbs_surface *, REAL, REAL, point_r3 *);


/* local functions */
static void init_nurbs_surface(nurbs_surface *, const char *, long, long, long, long, long, long);
static void free_trimming_tree(Trimming_tree_t *);
static int read_trimming_tree(FILE *, Trimming_tree_t *, int *, int *);
static void get_trimming_curve(Trimming_tree_t *, point_r2 *, int *, int *, BOOL *, int *);
