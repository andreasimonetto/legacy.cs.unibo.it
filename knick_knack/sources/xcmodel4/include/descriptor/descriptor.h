/* descriptor.h */

#include "globals/define.h"

#include "hrayt/types.h"
#include "hrayt/vec_macros.h"

#include "matrix/matrix.h"

#include "descriptor/types.h"

/************************************************************************/
extern ITEM create_attribute(char *name);

extern void set_attribute(ITEM it, ITEM attr);

extern void _set_color(ITEM it,	float r, 
		       float g,float b,float a,float d);

extern void _set_reflectivity(ITEM it, float ps, int c1);

extern void _set_transparency(ITEM it, float maxt,
			      float mint,int tpwr,float n);

#define	set_color(it,r,g,b,a,d) \
_set_color(it,(float)(r),(float)(g),(float)(b),(float)(a),(float)(d))

#define set_reflectivity(it,ps,c1) \
_set_reflectivity(it,(float)(ps),c1)


#define	set_transparency(it,maxt,mint,tpwr,n) \
_set_transparency(it,(float)(maxt),(float)(mint),tpwr,(float)(n))

/************************************************************************/

extern void _set_background(float r, float g, 
			    float b, BOOLEAN is_refl);

extern void _set_ambient_light(float r, float g, 
			      float b, float i);

extern ITEM _create_distant_light(double dx, double dy, 
				  double dz, float r, 
				  float g, float b, 
				  float i, char *name);

extern void _set_distant_light(double dx, double dy, 
				  double dz, float r, 
				  float g, float b, 
				  float i, ITEM it);

extern ITEM _create_point_light(double cx, double cy, 
				double cz, float r, 
				float g, float b, 
				float i, double max_range, char *name);

extern void _set_point_light(double cx, double cy, 
				double cz, float r, 
				float g, float b, 
				float i, double max_range, ITEM it);

extern ITEM _create_warn_light(double cx, double cy, double cz, 
			       double dx,double dy,double dz, 
			       float r,float g,float b, float i, 
			       double max_range, int conc, double angle, 
			       BOOLEAN flap_on, double fa, double fb, 
			       double fc, double fd, char *name);

extern void _set_warn_light(double cx, double cy, double cz, 
			       double dx,double dy,double dz, 
			       float r,float g,float b, float i, 
			       double max_range, int conc, double angle, 
			       BOOLEAN flap_on, double fa, double fb, 
			       double fc, double fd, ITEM it);

#define	set_background(r,g,b,is_refl) \
_set_background((float)(r),(float)(g),(float)(b),is_refl)

#define	set_ambient_light(r,g,b,i) \
_set_ambient_light((float)(r),(float)(g),(float)(b),(float)(i))

#define	create_distant_light(dx,dy,dz,r,g,b,i,n) \
_create_distant_light((double)(dx),(double)(dy),\
		      (double)(dz),(float)(r),(float)(g),\
		      (float)(b),(float)(i),(char *)(n))

#define	set_distant_light(dx,dy,dz,r,g,b,i,n) \
_set_distant_light((double)(dx),(double)(dy),\
		      (double)(dz),(float)(r),(float)(g),\
		      (float)(b),(float)(i),(ITEM)(n))

#define	create_point_light(cx,cy,cz,r,g,b,i,max_range,n) \
_create_point_light((double)(cx),(double)(cy),\
		    (double)(cz),(float)(r),\
		    (float)(g),(float)(b),\
		    (float)(i),(double)(max_range), (char *)(n))

#define	set_point_light(cx,cy,cz,r,g,b,i,max_range,n) \
_set_point_light((double)(cx),(double)(cy),\
		    (double)(cz),(float)(r),\
		    (float)(g),(float)(b),\
		    (float)(i),(double)(max_range), (ITEM)(n))

#define	create_warn_light(cx,cy,cz,dx,dy,dz,r,g,b,i,\
max_range,conc,angle,flap_on,fa,fb,fc,fd, n) \
_create_warn_light((double)(cx),(double)(cy),\
		   (double)(cz),(double)(dx),\
		   (double)(dy),(double)(dz),\
		   (float)(r),(float)(g),\
		   (float)(b),(float)(i),\
		   (double)(max_range),\
		   conc,(double)(angle),\
		   flap_on,(double)(fa),\
		   (double)(fb),(double)(fc),\
		   (double)(fd), (char *)(n))

#define	set_warn_light(cx,cy,cz,dx,dy,dz,r,g,b,i,\
max_range,conc,angle,flap_on,fa,fb,fc,fd, n) \
_set_warn_light((double)(cx),(double)(cy),\
		   (double)(cz),(double)(dx),\
		   (double)(dy),(double)(dz),\
		   (float)(r),(float)(g),\
		   (float)(b),(float)(i),\
		   (double)(max_range),\
		   conc,(double)(angle),\
		   flap_on,(double)(fa),\
		   (double)(fb),(double)(fc),\
		   (double)(fd), (ITEM)(n))

/*************************************************************************/

extern ITEM create_list(ITEM it, char *name);

extern ITEM insert_in_list(ITEM lst, ITEM it);

extern ITEM create_copy(ITEM it, char *name);

/*************************************************************************/

extern ITEM create_nurbs(int order_u, int order_v, 
			 int num_vertices_u, 
			 int num_vertices_v, 
			 VEC_vector_t **vertices, 
			 VEC_real_t **coefficients, 
			 VEC_real_t *knots_u,
			 VEC_real_t *knots_v,
			 BOOLEAN is_surface,
			 BOOLEAN normal_inside,
			 Trimming_tree_t trimming_tree, char *name);

extern ITEM read_nurbs(char *filename);

extern ITEM read_obj(char *filename, char *name);

/*************************************************************************/

extern void save_scene(char *filename, char *title, ITEM it);

extern void save_scene_ani(int nframe,char *ani_name,char *name, char *title, ITEM it);

/*************************************************************************/

extern void _set_translate(ITEM it, VEC_real_t x, 
			   VEC_real_t y,VEC_real_t z);
extern void _set_translate_abs(ITEM it, VEC_real_t x, 
			       VEC_real_t y,VEC_real_t z);

extern void _set_scale(ITEM it, VEC_real_t x,
		       VEC_real_t y, VEC_real_t z);
extern void _set_scale_abs(ITEM it, VEC_real_t x,
			   VEC_real_t y, VEC_real_t z);

extern void _set_x_rotate(ITEM it, VEC_real_t a,
			  int deg_fl);
extern void _set_x_rotate_abs(ITEM it, VEC_real_t a,
			      int deg_fl);

extern void _set_y_rotate(ITEM it, VEC_real_t a,
			  int deg_fl);
extern void _set_y_rotate_abs(ITEM it, VEC_real_t a,
			      int deg_fl);

extern void _set_z_rotate(ITEM it, VEC_real_t a,
			  int deg_fl);
extern void _set_z_rotate_abs(ITEM it, VEC_real_t a,
			      int deg_fl);

extern void _set_yz_shear(ITEM it, VEC_real_t a, 
			 VEC_real_t b);
extern void _set_yz_shear_abs(ITEM it, VEC_real_t a, 
			     VEC_real_t b);

extern void _set_xz_shear(ITEM it, VEC_real_t a, 
			 VEC_real_t b);
extern void _set_xz_shear_abs(ITEM it, VEC_real_t a, 
			     VEC_real_t b);

extern void _set_xy_shear(ITEM it, VEC_real_t a, 
			 VEC_real_t b);
extern void _set_xy_shear_abs(ITEM it, VEC_real_t a, 
			     VEC_real_t b);
extern void set_camera(char *name, float campX, float campY, float campZ, float objX, float objY, float objZ, float viewX, float viewY, float viewZ, float zoom, float dist);

#define	set_translate(it,x,y,z) \
_set_translate(it,(VEC_real_t)(x),\
	       (VEC_real_t)(y),(VEC_real_t)(z))

#define	set_translate_abs(it,x,y,z) \
_set_translate_abs(it,(VEC_real_t)(x),\
		   (VEC_real_t)(y),(VEC_real_t)(z))

#define	set_scale(it,x,y,z) \
_set_scale(it,(VEC_real_t)(x),\
	   (VEC_real_t)(y),(VEC_real_t)(z))

#define	set_scale_abs(it,x,y,z) \
_set_scale_abs(it,(VEC_real_t)(x),\
	   (VEC_real_t)(y),(VEC_real_t)(z))

#define	set_x_rotate(it,a,deg_fl) \
_set_x_rotate(it,(VEC_real_t)(a),deg_fl)

#define	set_x_rotate_abs(it,a,deg_fl) \
_set_x_rotate_abs(it,(VEC_real_t)(a),deg_fl)

#define	set_y_rotate(it,a,deg_fl) \
_set_y_rotate(it,(VEC_real_t)(a),deg_fl)

#define	set_y_rotate_abs(it,a,deg_fl) \
_set_y_rotate_abs(it,(VEC_real_t)(a),deg_fl)

#define	set_z_rotate(it,a,deg_fl) \
_set_z_rotate(it,(VEC_real_t)(a),deg_fl)

#define	set_z_rotate_abs(it,a,deg_fl) \
_set_z_rotate_abs(it,(VEC_real_t)(a),deg_fl)

#define	set_yz_shear(it,a,b) \
_set_yz_shear(it,(VEC_real_t)(a),(VEC_real_t)(b))

#define	set_yz_shear_abs(it,a,b) \
_set_yz_shear_abs(it,(VEC_real_t)(a),(VEC_real_t)(b))

#define	set_xz_shear(it,a,b) \
_set_xz_shear(it,(VEC_real_t)(a),(VEC_real_t)(b))

#define	set_xz_shear_abs(it,a,b) \
_set_xz_shear_abs(it,(VEC_real_t)(a),(VEC_real_t)(b))

#define	set_xy_shear(it,a,b) \
_set_xy_shear(it,(VEC_real_t)(a),(VEC_real_t)(b))

#define	set_xy_shear_abs(it,a,b) \
_set_xy_shear_abs(it,(VEC_real_t)(a),(VEC_real_t)(b))

#define 	set_camera(n,cX,cY,cZ,oX,oY,oZ,vX,vY,vZ,z,d)\
set_camera(n, (float) (cX), (float) (cY), (float) (cZ), (float) (oX), (float) (oY), (float) (oZ), (float) (vX), (float) (oY), (float) (oZ), (float) (z), (float) (d))

/************************************************************************/
	
extern void set_cube_texture(ITEM it, ITEM a1, ITEM a2);

extern void set_wood_texture(ITEM it, ITEM a1, ITEM a2);

extern void set_layer_texture(ITEM it, ITEM a1, ITEM a2);

extern void set_octant_texture(ITEM it, ITEM a1, ITEM a2);

extern void _set_triangle_texture(ITEM it, ITEM a1, ITEM a2,
				  double x, double z);

extern void set_angle_texture(ITEM it, ITEM a1, ITEM a2, int sect);

extern void set_weighted_rand_texture(ITEM it, ITEM a1, ITEM a2, 
                                      ITEM a3, ITEM a4,
				      int num1, int num2, 
                                      int num3, int num4);

extern void set_gradient_rand_texture(ITEM it, ITEM a1, ITEM a2, 
				      double coord, double percent, 
				      double start1, double start2);

extern void _set_cross_texture(ITEM it, ITEM a1, ITEM a2, 
			       double thick_x, double thick_z);

extern void set_shade_texture(ITEM it, ITEM a1, ITEM a2, 
			      double coord, double start1, double start2);

extern void _set_projection_texture(ITEM it, ITEM a1, 
				    char *im_name,
				    int ax_flag, int r_flag,
				    float pa, float pd);

extern void _set_domain_texture(ITEM it, ITEM a1, 
				char *im_name,
				float umin, float umax, 
                                float vmin, float vmax,
				int r_flag,
				float pa, float pd);

extern void _set_border_texture(ITEM it, ITEM a1, ITEM a2, 
				double umin, double umax, 
                                double vmin, double vmax);

extern void _set_bump_texture(ITEM it, ITEM a1,
				char *im_name,
				float umin, float umax,
				float vmin, float vmax,
				int r_flag,
				float pa, float pd);

#define set_triangle_texture(it,a1,a2,x,z) \
_set_triangle_texture(it,a1,a2,(double)(x),(double)(z))

#define	set_cross_texture(it,a1,a2,thick_x,thick_z) \
_set_cross_texture(it,a1,a2,(double)(thick_x),(double)(thick_z))

#define	set_projection_texture(it,a1,im_name,ax_flag,r_flag,pa,pd) \
_set_projection_texture(it,a1,im_name,ax_flag,r_flag,\
				(float)(pa),(float)(pd))

#define	set_domain_texture(it,a1,im_name,umin,umax,vmin,\
    vmax,r_flag,pa,pd) \
_set_domain_texture(it,a1,im_name,\
		    (float)umin,(float)umax,(float)vmin,\
		    (float)vmax,r_flag,(float)(pa),(float)(pd))

#define	set_border_texture(it,a1,a2,umin,umax,vmin,vmax) \
_set_border_texture(it,a1,a2,(double)(umin),(double)(umax),\
		    (double)(vmin),(double)(vmax))

/* Roberto Sottile's BUMP MAPPING */
#define	set_bump_texture(it,a1,im_name,umin,umax,vmin,vmax,r_flag,pa,pd) \
_set_bump_texture(it,a1,im_name,\
		    (float)umin,(float)umax,(float)vmin,\
		    (float)vmax,r_flag,(float)(pa),(float)(pd))

