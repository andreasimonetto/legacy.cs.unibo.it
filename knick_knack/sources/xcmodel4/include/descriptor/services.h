/* services.h */

#define	INIT_ITEM(it,type,ref_type,ref) { \
		it.obj_type = type; 		\
		it.reference.ref_type = ref; 	\
	} \

#define INVALID_RGB(r,g,b) \
	( \
		(r < 0.0) || (r > 1.0) || 	\
		(g < 0.0) || (g > 1.0) || 	\
		(b < 0.0) || (b > 1.0)		\
	) 		

#define INVALID_PRO(v) \
	( \
		(v < 0.0) || (v > 1.0) 	\
	) 

#define INVALID_VEC(x,y,z) \
	( \
		( IS_ZERO(x) ) && ( IS_ZERO(y) ) && ( IS_ZERO(z) ) 	\
	) 

#define INVALID_ANG(a) \
	( \
		(a < 0.0) || (a > 360) 	\
	)

