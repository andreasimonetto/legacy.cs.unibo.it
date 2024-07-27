/* globals.h */

extern Surf_Desc_t	*attribute_list[];
extern int		ind;

extern Light_t		*lights[];
extern int 		num_lights;

extern Nurbs_t		*nurbs[MAX_NURBS];
extern int 		num_nurbs;

extern Color_t 		ambient_color;
extern float 		ambient_int;
extern Color_t 		bkg_color;
extern BOOLEAN		bkg_is_refl;

extern Surf_Desc_t 	std_attribute;

extern char		*error_msg[DERROR_NUM];
extern char		*routine;

