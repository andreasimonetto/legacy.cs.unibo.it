/* xtreeview.h */

#define XW             0 /* window X coordinate */
#define YW             0 /* window Y coordinate */
#define WW           502 /* window width */
#define HW           502 /* window height */
#define ROTSTEP       24 /* number of steps to rotate the cop */

#define SQR(a)                ((a)*(a))


/* global functions */
int main( int, char ** );

/* local functions */
static void init_X( Display **, Colormap * );
static void new_window( Display *, TWin_t *, GC *, Colormap );
static BOOL new_cop( point_r3 *, point_r3 *, int );
static void visual_do( Display *, TWin_t *, GC, Trimmed_surface_t ** );

