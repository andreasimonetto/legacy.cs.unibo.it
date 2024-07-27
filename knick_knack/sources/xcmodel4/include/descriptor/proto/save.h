/* save.h */

void save_scene(char *name, char *title, 
	   ITEM it);
	   
void save_scene_ani(int nframe,char *ani_name, char *name, char *title, ITEM it);

static BOOLEAN save_scene1(char *name, char *title, int light_no, 
		    int att_no, int nurbs_no,
		    Light_t *light_ptr[], 
		    Surf_Desc_t *att_ptr[], 
		    Nurbs_t *nurbs_ptr[], 
		    Pointer_t obj_ptr);

static BOOLEAN save_lights(FILE *fp, int light_no, 
			Light_t *light_ptr[]);

static BOOLEAN save_attributes(FILE *fp, int att_no,
			    Surf_Desc_t *att_ptr[]);

static BOOLEAN save_hierarchy(FILE *fp, Pointer_t obj_ptr);

static BOOLEAN save_primitive(FILE *fp, Thing_t *obj);

static BOOLEAN save_nurbs(FILE *fp, int nurbs_no, 
		       Nurbs_t *nurbs_ptr[]);

/* extern functions */
extern BOOLEAN check_type(ITEM it, int obj_type);
extern void *descript_malloc(size_t size, size_t num);
extern void descript_error(int err, char *s);
extern Pointer_t get_pointer(ITEM it);
extern Node_t *init_list_node(Pointer_t left, unsigned int right_typ,
			      Node_t *right_node);

extern void standard_attribute(unsigned int n);



