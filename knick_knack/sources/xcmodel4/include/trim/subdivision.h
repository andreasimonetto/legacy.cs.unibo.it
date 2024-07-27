/* subdivision.h */

#define Index(ri,co,n_co) ( (ri)*(n_co)+(co) )
#define Other(k)          ( (int)((k+1)%2) )
#define COPY_3D(a,b)  { (a).x=(b).x; (a).y=(b).y; (a).z=(b).z; }
#define testeq(p1,p2) ( fabs((p1)->x-(p2)->x)<SPT && fabs((p1)->y-(p2)->y)<SPT && \
                        fabs((p1)->z-(p2)->z)<SPT )

#define DEG_U(s)        ((s)->deg_u)
#define DEG_V(s)        ((s)->deg_v)
#define CPN_U(s)        ((s)->cpn_u)
#define CPN_V(s)        ((s)->cpn_v)
#define UKNOT_NO(s)     ((s)->uknot_no)
#define VKNOT_NO(s)     ((s)->vknot_no)
#define UKNOT(s,i)      ((s)->uknots[i])
#define VKNOT(s,i)      ((s)->vknots[i])
#define SCP(s,i,j)      ((s)->ccp[(s)->cpn_v*(i)+(j)])

typedef struct {
  int  ind;
  REAL par;
} USCITA;

typedef struct {
  REAL  par[2];
  point_r3 vet[3];
} PUNTO2;

