/* transforms.h */

void _set_translate(ITEM it, VEC_real_t x, VEC_real_t y, 
		    VEC_real_t z);

void _set_translate_abs(ITEM it, VEC_real_t x, VEC_real_t y, 
			VEC_real_t z);

void _set_scale(ITEM it, VEC_real_t x, VEC_real_t y, 
		VEC_real_t z);

void _set_scale_abs(ITEM it, VEC_real_t x, VEC_real_t y, 
		    VEC_real_t z);

void _set_x_rotate(ITEM it, VEC_real_t a, int deg_fl);
void _set_x_rotate_abs(ITEM it, VEC_real_t a, int deg_fl);

void _set_y_rotate(ITEM it, VEC_real_t a, int deg_fl);
void _set_y_rotate_abs(ITEM it, VEC_real_t a, int deg_fl);

void _set_z_rotate(ITEM it, VEC_real_t a, int deg_fl);
void _set_z_rotate_abs(ITEM it, VEC_real_t a, int deg_fl);

void _set_yz_shear(ITEM it, VEC_real_t a, VEC_real_t b);
void _set_yz_shear_abs(ITEM it, VEC_real_t a, VEC_real_t b);

void _set_xz_shear(ITEM it, VEC_real_t a, VEC_real_t b);
void _set_xz_shear_abs(ITEM it, VEC_real_t a, VEC_real_t b);

void _set_xy_shear(ITEM it, VEC_real_t a, VEC_real_t b);
void _set_xy_shear_abs(ITEM it, VEC_real_t a, VEC_real_t b);

static void set_transform(ITEM it, VEC_matrix_4x3_t m);
static void set_transform_abs(ITEM it, VEC_matrix_4x3_t m);

static void init_transformation(char *routine_name, ITEM it);

static void set_transform1(Pointer_t p, VEC_matrix_4x3_t m);
static void set_transform1_abs(Pointer_t p, VEC_matrix_4x3_t m);

void set_camera(char *name, float campX, float campY, float campZ, float objX, float objY, float objZ, float viewX, float viewY, float viewZ, float zoom, float dist);

/* extern functions */
extern void descript_error(int err, char *s);

extern Pointer_t get_pointer(ITEM it);

extern BOOLEAN check_type(ITEM it, int obj_type);



