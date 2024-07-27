/* structures.h */

ITEM create_list(ITEM it, char *name);

ITEM insert_in_list(ITEM lst, ITEM it);

ITEM create_copy(ITEM it, char *name);

static Pointer_t create_copy1(Pointer_t p);

/* extern functions */

extern BOOLEAN	check_type(ITEM it, int obj_type);

extern void descript_error(int err, char *s);

extern Node_t *init_list_node(Pointer_t left, 
			      unsigned int right_typ,
			      Node_t *right_node, char *name);

extern Pointer_t get_pointer(ITEM it);

extern void *descript_malloc(size_t size, size_t num);



