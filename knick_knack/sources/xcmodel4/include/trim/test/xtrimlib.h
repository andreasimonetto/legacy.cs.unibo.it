/* xtrimlib.h */

#define XWDOM          0 /* domain window X coordinate */
#define YWDOM          0 /* domain window Y coordinate */
#define WWDOM        502 /* domain window width */
#define HWDOM        502 /* domain window height */

#define XWTRIM       513 /* object window X coordinate */
#define YWTRIM         0 /* object window Y coordinate */
#define WWTRIM       502 /* object window width */
#define HWTRIM       502 /* object window height */

#define MAXSURF       50 /* max. number of surfaces to be visualized */
#define ROTSTEP       24 /* number of steps to rotate the cop */

#define NOPIX          0
#define PIX            1

#define SQR(a)       ((a)*(a))
#define DIST_3D(A,B) sqrt(SQR((A).x-(B).x) + SQR((A).y-(B).y) + SQR((A).z-(B).z))

typedef struct {
  Pixmap pix[4];
  XPoint min_corner[4],
         max_corner[4];
} Cursor_t;


/* global functions */
int main(int, char **);

/* locale functions */
static void init_X(Display **, Colormap *);
static void open_window(Display *, TWin_t *, GC *, Colormap, int);
static void close_window(Display *, TWin_t *, GC, int);
static void clr_window(Display *, TWin_t *, GC);
static void make_color(Display *, Colormap, char *, unsigned long *);
static void make_colors(Display *, Colormap);
static void make_button(Display *, GC, Drawable,
                        unsigned int, unsigned int, int, int, int, int);
static void create_S_arrow(Display *, Pixmap, GC, int, int, int, int);
static void create_N_arrow(Display *, Pixmap, GC, int, int, int, int);
static void create_E_arrow(Display *, Pixmap, GC, int, int, int, int);
static void create_W_arrow(Display *, Pixmap, GC, int, int, int, int);
static void create_cursorPix(Display *, Window, GC, int, int,
                             unsigned int, unsigned int);
static void show_cursor(Display *, Window, GC, int);
static int selected_cursor(int, int);
static char scegli(char *, char, char);
static void eval_cmdline(int, char **);
static void read_surf(void);
static void input_params(Sub_type_t, REAL, REAL, int *, int *, REAL *, REAL *);
static BOOL rot_cop(point_r3 *, point_r3 *, int);
static BOOL new_cop(point_r3 *, point_r3 *, REAL, int);
static void delete_trimmed_surfaces(Trimmed_surface_t **, int);
static void draw_trimmed_surfaces(Trimmed_surface_t **, int);

