/* textures.h */

void set_cube_texture(ITEM it, ITEM a1, ITEM a2);

void set_wood_texture(ITEM it, ITEM a1, ITEM a2);

void set_layer_texture(ITEM it, ITEM a1, ITEM a2);

void set_octant_texture(ITEM it, ITEM a1, ITEM a2);

void _set_triangle_texture(ITEM it, ITEM a1, ITEM a2, 
			   double x, double z);

void set_angle_texture(ITEM it, ITEM a1, ITEM a2, 
		       int sect);

void set_weighted_rand_texture(ITEM it, ITEM a1, ITEM a2, 
			       ITEM a3, ITEM a4, int num1,
			       int num2, int num3, int num4);

void set_gradient_rand_texture(ITEM it, ITEM a1, ITEM a2,
			       double coord, double percent, double start1, 
			       double start2);

void _set_cross_texture(ITEM it, ITEM a1, ITEM a2,
			double thick_x, double thick_z);

void set_shade_texture(ITEM it, ITEM a1, ITEM a2,
		       double coord, double start1,
		       double start2);

void _set_projection_texture(ITEM it, ITEM a1, char *im_name,
			     int ax_flag, int r_flag, float pa,
			     float pd);

void _set_domain_texture(ITEM it, ITEM a1, char *im_name, 
			 float umin, float umax, float vmin,
			 float vmax, int r_flag, float pa,
			 float pd);

void _set_border_texture(ITEM it, ITEM a1, ITEM a2,
			 double umin, double umax, 
			 double vmin, double vmax);

static void init_texture(char *routine_name, ITEM it,
			 ITEM a1, ITEM a2);

static void set_texture(ITEM it, int tix, void *tdata,
			unsigned int type,
			unsigned int size);

static void set_texture1(Pointer_t p, int tix, void *tdata,
			 unsigned int type,
			 unsigned int size);

static void set_indexed_attribute(ITEM it, ITEM attr, int ix);

static void set_indexed_attribute1(Pointer_t p, short attr, 
				   int ix);

/* extern functions */

extern void descript_error(int err, char *s);

extern void *descript_malloc(size_t size, size_t num);

extern BOOLEAN	check_type(ITEM it, int obj_type);

extern Pointer_t get_pointer(ITEM it);




