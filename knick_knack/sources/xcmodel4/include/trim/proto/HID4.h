/* HID4.h */

/* global function */
void _hidline_create(TWin_t *,
		     point_r3 *, Poledge_t *, Polvert_t *,
		     int, int, int,
		     void **);
void _hidline_do(Visual_par_t *, View_par_t *, void *);
void _hidline_destroy(void *);


/* local functions */
static BoolVect AllocBoolVect(int, int);
static IntVect AllocIntVect(int, int);
static IntMat AllocIntMat(int, int, int, int);
static void FreeIntMat(IntMat, int, int, int);
static PointVect AllocPointVect(int, int);
static void FreePointVect(PointVect, int, int);
static ScPointVect AllocScPointVect(int, int);
static void FreeScPointVect(ScPointVect, int, int);
static Tri AllocTri(void);
static void FreeTri(Tri);
static ScTri AllocScTri(void);
static void FreeScTri(ScTri);
static void MakeTri(Tri, Point, Point, Point);
static void MakeScTri(ScTri, ScPoint, ScPoint, ScPoint);
static void CopyPoint(Point, Point);
static void CopyScPoint(ScPoint, ScPoint);
static void MedPoint(Point, Point, Point);
static void MedScPoint(ScPoint, ScPoint, ScPoint);
static BOOLEAN EqScPoint(ScPoint, ScPoint);
static PolVect AllocPolVectPSP(int);
static void FreePolVectPSP (PolVect, int);
static int NumtrPSP(int);
static void QvPSP(Point, Point, Point, Point,
                  ScPoint, ScPoint, Point, ScPoint);
static void Class2PSP(Tri, Tri, ScTri, ScTri,  Pol, PointVect, ScPointVect);
static long TestPSP(ScPoint, ScPoint, ScPoint);
static BOOLEAN AppartenenzaPSP(ScTri, ScPoint);
static BOOLEAN DistanzaPSP(Tri, Point);
static void Class1PSP(Tri, ScTri, Pol, PointVect, ScPointVect);
static void Class3PSP(Tri, ScTri, Pol, PointVect, ScPointVect);
static void Class4PSP(Tri, ScTri, Pol, PointVect, ScPointVect);
static BOOLEAN VisibilitaPSP(Pol, Point, ScPoint, PointVect, ScPointVect);
static int IsgnPSP(long);
static long DetPSP(ScPoint, ScPoint, ScPoint);
static int Codice4PSP(ScPointVect, Pol);
static int Codice3PSP(ScPointVect, Pol);
static void MaxminPSP(Pol, ScPointVect);
static void NewmaxminPSP(Pol, ScPoint);
static REAL ZminPSP(PointVect, Pol);
static void SgmaxminPSP(int, ScPointVect, ScPoint, ScPoint);
static void BisezPSP(PointVect, ScPointVect, Pol, PolVect *, IntVect,
                     ScPoint, ScPoint, REAL, REAL, int, int);
static BOOLEAN InPolPSP(Pol, ScPoint, ScPointVect);
static BOOLEAN InPol2PSP(Pol, ScPoint, ScPoint, ScPointVect);
static int SortPolPSP(Pol *, Pol *);


/* extern functions */
/* VISUAL */
extern void _persp3z(REAL, REAL, REAL, REAL *, REAL *, REAL *);
extern void _wind_view(REAL, REAL, int *, int *);

