/* attributes.h */

ITEM create_attribute(char *name);

void set_attribute(ITEM it, ITEM attr);

static void set_attribute1(Pointer_t p, short attr);

void _set_color(ITEM it, float r, float g,
		float b, float a, float d);

void _set_reflectivity(ITEM it, float ps, int c1);

void _set_transparency(ITEM it, float maxt, 
		       float mint, int tpwr, 
		       float n);

void standard_attribute(unsigned int n);

/* extern functions */

extern BOOLEAN check_type(ITEM it, int obj_type);

extern void descript_error(int err, char *s);

extern Pointer_t get_pointer(ITEM it);

extern void *descript_malloc(size_t size, size_t num);




