/* types.h */

#define DENOMEM         0
#define DEZERPAR        1
#define DECONSTR        2
#define DEDOMAIN        3
#define DETYPE          4
#define DEIO            5

#define DERROR_NUM	6
#define	DERROR_MSG	{\
				"DENOMEM",	\
				"DEZERPAR",	\
				"DECONSTR",	\
				"DEDOMAIN",	\
				"DETYPE",	\
				"DEIO"		\
			}

#define STD_AMBIENT_COL	{ 0.0, 0.0, 0.0 }
#define STD_BKG_COL	{ 0.0, 0.0, 0.0 }
	
#define LIGHT           1
#define ATTRIBUTE       2
#define THING           4
#define NODE            8

#define ATTRIBUTABLE	(NODE | THING)
#define TRANSFORMABLE	(NODE | THING | LIGHT)
#define	ALL_ITEM	(NODE | THING | LIGHT | ATTRIBUTE)

typedef struct item_t {
  short obj_type;
  union {
    char    *univ;
    Light_t *lp;
    Surf_Desc_t *ap;
    short   ai;
    Thing_t *tp;
    Node_t  *np;
  } reference;
} ITEM;

