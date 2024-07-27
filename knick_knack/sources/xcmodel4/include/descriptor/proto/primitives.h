/* primitives.h */

ITEM create_nurbs(int order_u, int order_v, 
		     int num_vertices_u, 
		     int num_vertices_v,
		     VEC_vector_t **vertices,
		     VEC_real_t **coefficients,
		     VEC_real_t *knots_u,
		     VEC_real_t *knots_v,
		     BOOLEAN is_surface,
		     BOOLEAN normal_inside,
		     Trimming_tree_t trimming_tree, char *name);

ITEM read_nurbs(char *filename);

ITEM read_obj(char *filename, char *name);

static void allocate_nurbs_arrays(Nurbs_t *b);

static Thing_t *create_primitive(int shape);

static void read_trimming_tree(FILE *fp, Trimming_tree_t *t);

static void read_trimming_curve(FILE *fp, Trimming_curve_t *c);

static void nextline(FILE *fp, char *s);

static void nurbs_extent(Extent_t *e, Nurbs_t *s);

/* extern functions */
extern void descript_error(int err, char *s);

extern void *descript_malloc(size_t size, size_t num);

extern ITEM create_list(ITEM it, char *name);

extern ITEM insert_in_list(ITEM lst, ITEM it);




