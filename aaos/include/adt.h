/** \file adt.h
	Abstract Data Type module.
*/
#ifndef _ADT_H
#define _ADT_H

/** \defgroup adt Abstract Data Types
	adt.h: The macros below are sets of template functions and structures
	that implements specific ADTs.
	
	Because in C doesn't exist the concept of \em overloading, each function
	must have a unique name. So, in order to obtain a sort of "template",
	the functions must be builded changing their name. This is the role
	of the \em domain parameter, present in each of these macros.
	
	The parameter \em T is the real template parameter and stands for
	"generic type".
	
	Other parameters are ADT-dependent.
	
	Sets of functions are generated without their prototype, that will
	be written by hand by the programmer, if needed. This permit a better
	documentation phase, writing what eachs single function do.
\{ */

/** Static Memory Manager.
	The Static Memory Manager ADT encapsulate a fixed memory area to
	store generic elements, and the functions to allocate/deallocate
	them. An addictional amount \em C of memory will be allocated when a
	SMM is instantiated, where:
	- C = (size + 1) * sizeof(int)
	
	\param T The type of the elements to manage.
	\param domain A single word that will be used in the name of the
	generated functions.
	\param size The maximum numbers of elements in the (statically
	allocated) memory area (size_bytes = sizeof(T) * size).
	
	\include adt_smm.example
	
	After the preprocessing, the body of the functions prototypes 1, 2, 3
	and 4 are present at the bottom of the file.
	
	In the example, if \em sizeof(int) == 4bytes:
	- C = (100 + 1) * 4 = 404byte
	- total amount of memory used = 
	(sizeof(struct info) * size) + C = (4+50+50) * 100 + C = 10804bytes
*/
#define DEF_ADT_SMM(T, domain, size) \
static T _ ## domain ## _mem_block[size]; \
static T* _ ## domain ## _free_stack[size]; \
static int _ ## domain ## _free_stack_top; \
\
void init ## domain ## s() \
{ \
int i; \
\
	_ ## domain ## _free_stack_top = 0; \
	for(i = size - 1; i >= 0; i--) { \
		_ ## domain ## _free_stack[(_ ## domain ## _free_stack_top)++] = \
			_ ## domain ## _mem_block + i; \
	} \
} \
\
void free ## domain(T *m) { \
	_ ## domain ## _free_stack[(_ ## domain ## _free_stack_top)++] = m; \
} \
\
T* alloc ## domain() \
{ \
T *m; \
\
	if(!(_ ## domain ## _free_stack_top)) \
		return 0; \
\
	m = _ ## domain ## _free_stack[--(_ ## domain ## _free_stack_top)]; \
	return m; \
} \
\
int get  ## domain ## ID(const T *m) \
{ \
const int id = (m - _ ## domain ## _mem_block); \
\
	if(id < 0 || id >= size) \
		return -1; \
\
	return id; \
}

/** Queue.
	This is a queue ADT implemented with a double-linked, circular,
	tail-pointed list.

	\em field_next and \em field_prev parameters are self structure pointers.

	\param T The type of the elements to manage.
	\param domain A single word that will be used in the name of the
	generated functions.
	\param field_next The name of the "next element pointer" in the
	user-defined structure.
	\param field_prev The name of the "previous element pointer" in the
	user-defined structure.
	
	\include adt_queue.example
	
	After the preprocessing, the body of the functions prototypes from 1 to
	7 are present at the bottom of the file.
*/
#define DEF_ADT_QUEUE(T, domain, field_next, field_prev) \
T* mkEmpty ## domain ## Q () { \
	return NULL; \
} \
\
int empty ## domain ## Q (const T *q) { \
	return (q == NULL); \
} \
\
void insert ## domain (T **q, T *v) { \
	if(*q) { \
		/* Are there any element in the queue? */ \
		v -> field_next = (*q) -> field_next; \
		v -> field_next -> field_prev = v; \
		v -> field_prev = *q; \
		(*q) -> field_next = v; \
		*q = v; \
	} \
	else { \
		v -> field_prev = v -> field_next = v; \
		*q = v; \
	} \
} \
\
T* out ## domain (T **q, T *v) { \
	if(!q || !*q || !v) \
		/* Queue is empty or invalid pointer */ \
		return NULL; \
	\
	if((*q) -> field_next == *q) { \
		/* Queue has a single element */ \
		if(v != *q) \
			return NULL; \
		\
		*q = NULL; \
		v -> field_prev = v -> field_next = NULL; \
		return v; \
	} \
	\
	if(!(v -> field_prev) || !(v -> field_next)) \
		/* v has no prev or next field */ \
		return NULL; \
	\
	v -> field_prev -> field_next = v -> field_next; \
	v -> field_next -> field_prev = v -> field_prev; \
	\
	if(v == *q) \
		/* Is v the tail of the queue? */ \
		*q = (*q) -> field_prev; \
	\
	v -> field_prev = v -> field_next = NULL; \
	return v; \
} \
\
T* remove ## domain (T **q) { \
	return ((!q || !*q) ? NULL : out ## domain (q, (*q) -> field_next)); \
} \
\
T* head ## domain (const T *q) { \
	if(!q) \
		return NULL; \
	else \
		return q -> field_next; \
} \
\
T* search ## domain (const T *q, const T *v) \
{ \
const T *it; \
\
	if(!empty ## domain ## Q(q)) { \
        	it = q -> field_next; \
        	do { \
                	if(it == v) \
                		return (T*)v; \
			\
                	it = it -> field_next; \
        	} while(it != q -> field_next); \
	} \
	\
	return NULL; \
}
/** \} */

#endif
