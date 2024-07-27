/* trim.h */

#ifndef __trim_h
#define __trim_h

#include "trim/error.h"
#include "trim/defines.h"
#include "trim/trimmed-nurbs.h"

extern Display *_trim_disp;
extern Visual *_trim_vis;
extern unsigned _trim_depth;
extern unsigned long _trim_grey[_TRIM_NGREY];

/* TRIMMED-NURBS */
extern void Trim_free_nurbs(nurbs_surface *nurbs);
extern int Trim_read_nurbs(FILE *f, nurbs_surface **nurbs);
extern void Trim_nurbs_bbox(nurbs_surface *nurbs, Bbox_t *bb);
extern int  Trim_read_trimmed_nurbs(FILE *f, Trimmed_nurbs_t **tnurbs);
extern void Trim_free_trimmed_nurbs(Trimmed_nurbs_t *tnurbs);

/* TRIMMING */
extern int  Trim_nurbs_triangulation(nurbs_surface *nurbs, Trim_par_t *par,
				     Trimmed_surface_t **tsurf);
extern int  Trim_triangulation(Trimmed_nurbs_t *tnurbs, Trim_par_t *par, 
			       Trimmed_surface_t **tsurf);
extern void Trim_new_surface(nurbs_surface *nurbs, Trimmed_surface_t *tsurf);
extern void Trim_delete_surface(Trimmed_surface_t *tsurf);
extern void Trim_surface_bbox(Trimmed_surface_t *tsurf, Bbox_t *bb);
extern void Trim_surface_list_bbox(Trimmed_surface_t **vtsurf, int nsurf, Bbox_t *bb);
extern void Trim_set_params(Sub_type_t sub, int nu, int nv, REAL srt, REAL crt,
			    Trim_par_t *par);
extern void Trim_get_params(Trim_par_t *par, Sub_type_t *sub, int *nu, int *nv,
			    REAL *srt, REAL *crt);

/* VISUAL */
extern void Trim_init(Display *disp, Colormap map);
extern void Trim_close(void);
extern void Trim_draw_domain(TWin_t *win, BOOL fill);
extern void Trim_draw_axes(TWin_t *win, Visual_par_t *vis, View_par_t *view);
extern void Trim_draw_grid(TWin_t *win, Grid_par_t *grid);
extern void Trim_draw_curve(TWin_t *win, Trimming_curve_t *curve, BOOL hole, BOOL fill,
			    REAL umin, REAL umax, REAL vmin, REAL vmax);
extern void Trim_draw_tree(TWin_t *win, Trimmed_nurbs_t *tnurbs, BOOL fill);
extern void Trim_draw_triangulation(TWin_t *win, Trimmed_surface_t *tsurf);
extern void Trim_draw_trimmed_domain(TWin_t *win, Trimmed_nurbs_t *tnurbs,
				     Trimmed_surface_t *tsurf);
extern void Trim_persp3_par(View_par_t *view, int vxmin, int vymin,
			    unsigned int width, unsigned int height);
extern void Trim_persp3(point_r3 *pt, int *vx, int *vy, REAL *pz);
extern void Trim_default_attrib(Obj_attrib_t *attr);
extern void Trim_default_visual(Visual_par_t *vis);
extern int  Trim_visual_create(TWin_t *win, Visual_par_t *vis, Obj_attrib_t *attr,
			       Trimmed_surface_t **vtsurf, int nsurf, int *id);
extern int  Trim_visual_do(int id, View_par_t *view);
extern void Trim_visual_destroy(int id);

#endif
