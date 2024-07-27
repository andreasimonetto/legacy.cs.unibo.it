/* lights.h */

void _set_background(float r, float g, float b, 
		     BOOLEAN is_refl);

void _set_ambient_light(float r, float g, 
			float b, float i);

ITEM _create_distant_light(double dx, double dy, 
			   double dz, float r, 
			   float g, float b, float i, char *name);

void _set_distant_light(double dx, double dy, 
			   double dz, float r, 
			   float g, float b, float i, ITEM it);

ITEM _create_point_light(double cx, double cy, 
			 double cz, float r, float g, 
			 float b, float i, double max_range, char *name);

void _set_point_light(double cx, double cy, 
			 double cz, float r, float g, 
			 float b, float i, double max_range, ITEM it);

ITEM _create_warn_light(double cx, double cy, double cz,
			double dx, double dy, double dz, 
			float r, float g, float b, float i, 
			double max_range, int conc, double angle,
		   	BOOLEAN flap_on, double fa, double fb, 
			double fc, double fd, char *name);

void _set_warn_light(double cx, double cy, double cz,
			double dx, double dy, double dz, 
			float r, float g, float b, float i, 
			double max_range, int conc, double angle,
		   	BOOLEAN flap_on, double fa, double fb, 
			double fc, double fd, ITEM it);

/* extern functions */

extern void descript_error(int err, char *s);

extern void *descript_malloc(size_t size, size_t num);
