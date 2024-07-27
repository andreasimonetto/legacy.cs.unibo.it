/* services.h */

BOOLEAN	check_type(ITEM it, int obj_type);

Pointer_t get_pointer(ITEM it);

void descript_error(int err, char *s);

void *descript_malloc(size_t size, size_t num);

Node_t *init_list_node(Pointer_t left, unsigned int right_typ,
		       Node_t *right_node, char *name);


