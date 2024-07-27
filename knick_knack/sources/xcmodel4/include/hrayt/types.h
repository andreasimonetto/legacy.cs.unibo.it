/* types.h */

#ifdef PI
#undef PI
#endif
#define PI    3.14159265358979
#define TWOPI 6.28318530717959

#define MIN_EPS         0.0039

#define MAX_LIGHTS      64
#define MAX_ATTRIB      1024
/* #define MAX_NURBS	128 */
#define MAX_NURBS	20000
#define MAX_OBJ_ATTR    8

#define MAX_NAMES       256
#define PATHLEN         1024

#define HUGE_DIST       1.0e+30

#define NURBS           11

#define TITLE_LEN       200

typedef long code_int;
typedef long count_int;
typedef unsigned char char_type;

typedef double VEC_real_t;
typedef VEC_real_t VEC_vector_2_t[2];
typedef VEC_real_t VEC_vector_t[3];
typedef VEC_real_t Plane_t[4];
typedef VEC_real_t VEC_matrix_4x3_t[4][3];
typedef VEC_real_t VEC_matrix_4x4_t[4][4];

#define IDENTITY  0
#define ARBITRARY 1


typedef struct { /* just bidimensional point used for mouse coord */
  double x;
  double y;
} DPoint; 

typedef struct {
  unsigned int form;
  VEC_matrix_4x3_t Mat, Inv; /* Inv (hrayt use only) */
} Transform_t;

typedef struct {
  VEC_vector_t min_corner;
  VEC_vector_t max_corner;
} Extent_t;

typedef float Color_t[3];

typedef struct attr_bak_t {
  Color_t col;
  float   pa_fact, pd_fact;
  float   ps;
  float   n[2];
  float   maxt;
  float   mint;
  short   c1;
  short   tpwr;
} info_attr_t;

typedef struct {
  char    name[255];
  info_attr_t bak;        /* xcrayt use only: backup of attribute */
  BOOLEAN set;            /* xcrayt use only: to indicate if an attribute is setted */
  Color_t pa;             /* proportion of ambient reflection   (hrayt use only)  */
  Color_t pd;             /* proportion of diffuse reflection   (hrayt use only)  */

  Color_t col;	          /* color */
  float   pa_fact, pd_fact;	    
  float   ps;             /* proportion of specular reflection    */
  float   n[2];           /* index of refraction,                 */
                          /* where n[0] = n2/n1 and n[1] = n1/n2  */
			  /* n1 = index for outside world         */
			  /* n2 = index of refraction for object  */
  int     c1;             /* Phong's specular exponent            */
  float   c1_eps;
  float   c3;             /* from 0 for very shiny surfaces       */
	 	          /* to 1 for very diffuse surfaces       */
  float   maxt;           /* maximum transparency of the object   */
  float   mint;           /* minimum transparency of the object   */
  int     tpwr;           /* transparency power factor,           */
	   	          /* 0 for solid objects and increases    */
			  /* as thickness decreases               */
  float   tpwr_eps;
  double   fu,fv;	  /* gradient of bumpmap in direction u and v ([0..1])*/
} Surf_Desc_t;

#define N_T_t_UNDEF       0
#define N_T_t_LIST_HEADER 1
#define N_T_t_LIST        2
#define N_T_t_PRIMITIVE   3
#define N_T_t_PATCH       4
#define N_T_t_LIGHT       5
#define N_T_t_NONE        1000 /* xcrayt used */

typedef struct pointer_t {
  unsigned int typ;             /* vedi define sopra */
  union {
    struct thing_t  *leaf;
    struct light_t  *light;
    struct node_t   *node;
    struct patch_t  *patch;
  } ptr;
} Pointer_t;

typedef struct trimming_tree_t {
  BOOLEAN                 trim_outside;
  int                     num_curves;
  struct trimming_curve_t *curves;
} Trimming_tree_t;

typedef struct trimming_curve_t {
  unsigned        num_vertices;
  VEC_vector_2_t  *vertices;
  Trimming_tree_t tree;
} Trimming_curve_t;
	
typedef struct cell_node_t {
  Pointer_t           object;
  Extent_t            *bounds;
  BOOLEAN             *tested_fg;
  VEC_real_t          dist;
  struct cell_node_t  *next;
} Cell_node_t;

typedef struct seads_t {
  int		num_cells;
  Extent_t	v;
  VEC_vector_t	edge;
  VEC_vector_t	deltav,dv,sv;
  int		pv[3],ii,jj,kk;
  int		n[3],p[3],h;
  VEC_real_t	dist;		/* dopo il dda3d misura la distanza lungo il raggio del voxel corrente */
  Cell_node_t	**cells;
} Seads_t;

typedef struct {
  char    	name[255];        
  char    	*srcdir;  
  BOOLEAN 	is_local;
  BOOLEAN 	deleted;
  int  		num_vertices_u, num_vertices_v;
  int  		num_knots_u, num_knots_v;
  VEC_real_t  	*knots_u,*knots_v;
  int         	order_u,order_v;
  VEC_vector_t  **vertices;
  VEC_real_t    **coefficients;
  struct pointer_t patches_list;
  BOOLEAN  	is_surface;
  BOOLEAN  	normal_inside;
  Trimming_tree_t trimming_tree;
  Seads_t  	*seads;
  Extent_t 	extent;
} Nurbs_t;

typedef struct patch_t {
  Extent_t 	   extent;      /* extent nel sistema di rif della NURBS */
  Extent_t 	   true_extent; /* extent nel sistema di riferimento legato alla trasformazione m	     */
  VEC_matrix_4x3_t m;
  BOOLEAN 	   tested_fg;
  int              order_u,order_v;
  Plane_t     	   *cp;
  VEC_real_t       zero_u,zero_v,one_u,one_v;
  Trimming_tree_t  trimming_tree;
} Patch_t;

typedef unsigned char RGB_BYTE[3];

typedef struct {
  RGB_BYTE **no_image_plane;
} image_plane_t;

typedef struct {
  int           width,height,r_flag,ax_flag;
  float         pa,pd;
  float         umin,umax,vmin,vmax;
  char          im_name[MAX_NAMES];
  image_plane_t image_plane;
} image_table_t;

/* tipi di dati possibili in type */
#define NOTHING 0
#define DBL     1
#define INT     2
#define IMG     3

typedef struct tex_data_t {
  unsigned int size;
  unsigned int type; /* vedi sopra */ 
  union {
    int           *i;
    double        *d;
    image_table_t *img;
  } tdata;
} Tex_data_t;

typedef struct thing_t {
  char 		   name[255];
  BOOLEAN 	   mark;  /* xcrayt: to de/select an object in a rappresentation */
  VEC_vector_t     baricenter;
  Extent_t         extent;
  short int        geo_shape;
  BOOLEAN          tested_fg;
  Transform_t      transform, tback, origTransfByMouse, origTexTransf; /* tback: xcrayt use only to perform trasformation backup for undo.   origTransfByMouse: usata in xcrayt per applicare le trasformazioni durante il movimento del mouse sempre alla matrice originale onde evitare di sommare le trasformazioni. OrigTexTransf: backup usata quando si effettuano trasformazioni nello spazio texture per ripristinare la matrice originale al termine della modalita' spazio texture */
  BOOLEAN          set; /* xcrayt use only to know obj is setted */     
  VEC_vector_t     last_origin, last_delta;
  short int        texture_ix;
  short int        world_point_fg;
  unsigned int     texture_matrix_form;
  VEC_matrix_4x3_t texture_matrix;
  Tex_data_t td;
  union {
    int         i;
    Surf_Desc_t *p;
  } attributes[MAX_OBJ_ATTR];
  union {
    int		i;
    Nurbs_t	*p;
  } nurbs;
  unsigned int 	   objindx; /* xcrayt use only: to identify the data objects */
} Thing_t;

#define DISTANT_LT 0 /* define per il campo type */
#define POINT_LT   1
#define WARN_LT    2

typedef struct lt_bak_t {
  char 	       name[255];
  unsigned short int type; /* vedi define sopra */
  VEC_vector_t location;
  VEC_vector_t direction;
  VEC_vector_t object_point; /* xcrayt use only to set view parameters */
  VEC_vector_t view_up; /* xcrayt use only to set view parameters */
  BOOLEAN      vwupset; /* xcrayt use only to set view parameters */
  Color_t      light_color;
  float        intensity;
  short int    concentration_exp;
  VEC_real_t   max_range;
  VEC_real_t   cone_cosine;
  int          flap_on;
  Plane_t      flap;
} info_lt_t;

typedef struct light_t {
  char 	       name[255];
  BOOLEAN      set; /* xcrayt: lightset: TRUE/FALSE */
  unsigned short int type; /* vedi define sopra */
  VEC_vector_t location;
  VEC_vector_t direction;
  VEC_vector_t object_point; /* xcrayt use only to set view parameters */
  VEC_vector_t view_up; /* xcrayt use only to set view parameters */
  BOOLEAN      vwupset; /* xcrayt use only to set view parameters */
  BOOLEAN      viewset; /* xcrayt use only */
  Color_t      light_color;
  float        intensity;
  short int    concentration_exp;
  float        conc_eps;
  VEC_real_t   max_range;
  VEC_real_t   cone_cosine;
  int          flap_on;
  Plane_t      flap;
  info_lt_t    lightbak; /* xcrayt: per fare undo sulle luci */
  void	       *object_cache;
} Light_t;

typedef struct node_t {
  char 	       name[255];
  VEC_vector_t baricenter;
  BOOLEAN      mark; /* xcrayt use only to select and deselct an object */
  BOOLEAN      set;  /* xcrayt use only to know if obj is setted */
  unsigned int id;
  int          tested_fg;
  Extent_t     bounds;
  Pointer_t    left,right;
} Node_t;

typedef struct {
  VEC_real_t   distance;
  int          ray_direction;
  int          index_fg;
  Thing_t      *prim_intersected;
  Patch_t      *patch;
  VEC_real_t   u,v;
  VEC_vector_t point;
} Intersection_t;

#define MAX_ORDER       9

typedef VEC_real_t      Base_Bezier_vector_t[MAX_ORDER];

#define MAX_DIM	250
typedef VEC_real_t      Base_Nurbs_vector_t[MAX_DIM];

#define MAX_SURFACES	100
typedef struct Spline_surface_t {
  int            order_u,order_v;
  VEC_vector_2_t vertices[MAX_ORDER][MAX_ORDER];
  VEC_real_t     zero_u,zero_v,one_u,one_v;
  BOOLEAN	 inters_inside;
  VEC_vector_2_t x;
} Spline_surface_t;

typedef struct _texture_list_t {
  char                   im_name[MAX_NAMES];
  image_plane_t          image_plane;
  int                    width,height;
  struct _texture_list_t *next_texture;
  struct _texture_list_t *prev_texture;
} texture_list_t;

typedef struct view_parameters_t {
  VEC_vector_t camera_point;   
  VEC_vector_t object_point;
  VEC_vector_t camvec;    /* xcrayt: to speed up calcolous */
  VEC_real_t   modcamvec; /* xcrayt: to speed up calcolous */
  VEC_vector_t view_up;
  VEC_real_t   view_angle;
  VEC_real_t   ratio;
} View_parameters_t;

typedef struct shadow_item_t {
  void		*p,*o;
  int		pix;
} Shadow_Item_t;

#define TOTH_SPEED 1
#define BEZIER     2
#define TOTH       3

typedef struct param_t {
  int	     lines;
  int	     pixels;
  BOOLEAN    asp_lines;
  BOOLEAN    asp_pixels;
  int	     max_depth;
  float	     epsilon;
  int	     lighting_model;
  VEC_real_t reach;
  BOOLEAN    shadows_on;
  BOOLEAN    shadow_cache_on;
  int	     num_cell_scene;
  int	     num_cell_nurbs;
  int	     max_subdivision_depth;
  float	     min_flat_ratio;
  char	     stat_file[PATHLEN];
  char	     output_file[PATHLEN];
  unsigned int intersecator; /* vedi define sopra */
              /* parametri per l'Antialiasing */
  int alg;    /* algoritmo utilizzato per il campionamento*/
  float tol;  /* tolleranza per il confronto dei colori: 0 <= tol <= 3 */
  int max_sl; /* massimo livello di ricorsione per il campionamento */
  int dim_sm; /* dimensione della matrice tampone dei campioni: 
                 dim_sm = 2^max_sl + 1 */
} Param_t;


enum Antialias_alg{AA_NONE, AA_ADAPTIVE_BASE, AA_ADAPTIVE_SLOW, AA_ADAPTIVE_FAST};

typedef struct seads_level_t {
  int  **tested_array;
  int  tested_num;
  int  (*intersect)(Pointer_t,
	            VEC_vector_t,
		    VEC_vector_t,
		    Intersection_t *,
		    VEC_real_t,
		    BOOLEAN);
  long num_call_close;
  long num_call_test;
  long num_box_close;		/* numero di test di intersezione con
				   il bounding box degli oggetti nella
				   seads per l'attraversamento di
				   raggi non ombra			*/
  long num_box_test;		/* come precedente per raggi ombra	*/
  long num_inters_close;	/* numero di test di intersezione con
				   gli oggetti nella seads per 
				   l'attraversamento di raggi non ombra	*/
  long num_inters_test;	        /* come precedente per raggi ombra	*/
  long num_noempty_voxel;
  long num_voxel;	
  long num_object;	
} Seads_level_t;


/* Antialiasins types */
typedef struct Sample_t {
	int valid;
	Color_t color;
} Sample_t;
