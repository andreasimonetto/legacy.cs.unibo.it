/* HID4.h */

#undef TRUE
#define TRUE      (1)
#undef FALSE
#define FALSE     (0)

#define XBOX      (10)
#define YBOX      (10)

#define AllocPol()              (Pol)malloc(sizeof(POL))
#define FreePol(p)              free((void *)p)
#define AllocPoint()            (Point)malloc(sizeof(POINT))
#define FreePoint(p)            free((void *) p)
#define AllocScPoint()          (ScPoint)malloc(sizeof(SCPOINT))
#define FreeScPoint(p)          free((void *) p)

#define NewBoolVect(i)          AllocBoolVect(i,1)
#define FreeBoolVect(v,st)      free((void *)(v+st))
#define DeleteBoolVect(v)       FreeBoolVect(v,1)

#define NewIntVect(i)           AllocIntVect(i,1)
#define FreeIntVect(v,st)       free((void *)(v+st))
#define DeleteIntVect(v)        FreeIntVect(v,1)
#define NewIntMat(i,j)          AllocIntMat(i,j,1,1)
#define DeleteIntMat(m,i)       FreeIntMat(m,i,1,1)

#define NewPointVect(i)         AllocPointVect(i,1)
#define DeletePointVect(v,i)    FreePointVect(v,i,1)
#define NewScPointVect(i)       AllocScPointVect(i,1)
#define DeleteScPointVect(v,i)  FreeScPointVect(v,i,1)

typedef unsigned                   BOOLEAN     ;
typedef BOOLEAN                   *BoolVect    ;

typedef point_r3                   POINT       ;
typedef POINT                     *Point       ;
typedef Point *                    PointVect   ;

typedef struct { int x,y; }        SCPOINT     ;
typedef SCPOINT                   *ScPoint     ;
typedef ScPoint                   *ScPointVect ;

typedef struct { Point a,b,c; }    TRI         ;
typedef TRI                       *Tri         ;

typedef struct { ScPoint a,b,c; }  SCTRI       ;
typedef SCTRI                     *ScTri       ;

typedef int                       *IntVect     ;
typedef IntVect                   *IntMat      ;

typedef struct {
  int a,b,c,d,cod;
  int xmax,xmin,ymax,ymin;
  REAL zmin;
} POL;

typedef POL *Pol;
typedef Pol *PolVect;

typedef struct {
  int npt;
  int nsg;
  int npl;
  point_r3 *pt;
  Poledge_t *sg;
  Polvert_t *pl;
  PointVect ptpr;
  ScPointVect ivt;
  BoolVect vis;
  PolVect pol;
  PolVect hid;
  TWin_t *win;
  GC gc;
} HlData;
