/* define.h */

typedef int BOOLEAN;

#ifndef TRUE
#define TRUE  (1)
#define FALSE (0)
#endif

#define EPS  1.0E-128

#define IS_ZERO(x)     ( (fabs( (x) ) < EPS) ? 1 : 0)
#define IS_NOT_ZERO(x) ( (fabs( (x) ) >= EPS) ? 1 : 0)
#define EQUAL(x, y)     ( (fabs((x) - (y)) < EPS ) ? 1 : 0)
#define NOT_EQUAL(x, y) ( (fabs((x) - (y)) >= EPS ) ? 1 : 0)

typedef double VEC_real_t;
typedef VEC_real_t VEC_vector_t[3];
typedef VEC_real_t VEC_matrix_4x3_t[4][3];
typedef VEC_real_t VEC_matrix_4x4_t[4][4];

#define HUGE_DIST 1.0E+30


