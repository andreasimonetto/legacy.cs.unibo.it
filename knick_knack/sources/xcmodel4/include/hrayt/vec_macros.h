/* vec_macros.h */


#define VEC_false      0
#define VEC_true       1

#define VEC_is_zero(vec)\
  ( IS_ZERO(vec[0]) )\
    && ( IS_ZERO(vec[1]) ) && ( IS_ZERO(vec[2]) )

#define VEC_add_constant(out_vec,in_vec,const) \
  { \
    out_vec[0] = in_vec[0] + const; \
    out_vec[1] = in_vec[1] + const; \
    out_vec[2] = in_vec[2] + const; \
  }

#define VEC_add_vector(out_vec,vec1,vec2) \
  { \
    out_vec[0] = vec1[0] + vec2[0]; \
    out_vec[1] = vec1[1] + vec2[1]; \
    out_vec[2] = vec1[2] + vec2[2]; \
  }

#define VEC_copy(out_vec,in_vec) \
  { \
    out_vec[0] = in_vec[0]; \
    out_vec[1] = in_vec[1]; \
    out_vec[2] = in_vec[2]; \
  }

#define VEC_dot(vec1,vec2) \
  (vec1[0] * vec2[0] + vec1[1] \
   * vec2[1] + vec1[2] * vec2[2])

#define VEC_init(out_vec,x,y,z) \
  { \
    out_vec[0] = x; \
    out_vec[1] = y; \
    out_vec[2] = z; \
  }

#define VEC_mult_add(out_vec,add_vec,mult_vec,const) \
  { \
    out_vec[0] = add_vec[0] + const * mult_vec[0]; \
    out_vec[1] = add_vec[1] + const * mult_vec[1]; \
    out_vec[2] = add_vec[2] + const * mult_vec[2]; \
  }

#define VEC_mult_constant(out_vec,in_vec,const) \
  { \
    out_vec[0] = const * in_vec[0]; \
    out_vec[1] = const * in_vec[1]; \
    out_vec[2] = const * in_vec[2]; \
  }

#define VEC_sub(out_vec,in_vec,sub_vec) \
  { \
    out_vec[0] = in_vec[0] - sub_vec[0]; \
    out_vec[1] = in_vec[1] - sub_vec[1]; \
    out_vec[2] = in_vec[2] - sub_vec[2]; \
  }

#define VEC_sum(in_vec) \
    (in_vec[0] + in_vec[1] + in_vec[2])

#define VEC_zero(in_vec) \
    in_vec[0] = in_vec[1] = in_vec[2] = 0.0;

#define VEC_change_sign(out_vec,in_vec) \
  { \
    out_vec[0] = -in_vec[0]; \
    out_vec[1] = -in_vec[1]; \
    out_vec[2] = -in_vec[2]; \
  }

#define VEC_cross(out_vec,vec1,vec2) \
  { \
    out_vec[0] = vec1[1] * vec2[2] - vec1[2] * vec2[1]; \
    out_vec[1] = vec2[0] * vec1[2] - vec2[2] * vec1[0]; \
    out_vec[2] = vec1[0] * vec2[1] - vec1[1] * vec2[0]; \
  }

#define VEC_sum_squares(vec) \
    VEC_dot(vec,vec)

#define VEC_length(vec) \
    (sqrt(VEC_sum_squares(vec)))

#define VEC_normalize(out_vec,in_vec,tmp) \
    tmp = VEC_length(in_vec); \
    if( IS_NOT_ZERO(tmp) ) { \
      tmp = 1.0 / tmp; \
      VEC_mult_constant(out_vec,in_vec,tmp) \
    }

#define VEC_pt_premult(out_vec,in_vec,mat) \
  { \
    out_vec[0] = in_vec[0] * mat[0][0] + in_vec[1] * mat[1][0] + \
      in_vec[2] * mat[2][0] + mat[3][0]; \
    out_vec[1] = in_vec[0] * mat[0][1] + in_vec[1] * mat[1][1] + \
      in_vec[2] * mat[2][1] + mat[3][1]; \
    out_vec[2] = in_vec[0] * mat[0][2] + in_vec[1] * mat[1][2] + \
      in_vec[2] * mat[2][2] + mat[3][2]; \
  }

#define VEC_nm_premult(out_vec,in_vec,mat) \
  { \
    out_vec[0] = in_vec[0] * mat[0][0] + in_vec[1] * mat[1][0] + in_vec[2] * mat[2][0]; \
    out_vec[1] = in_vec[0] * mat[0][1] + in_vec[1] * mat[1][1] + in_vec[2] * mat[2][1]; \
    out_vec[2] = in_vec[0] * mat[0][2] + in_vec[1] * mat[1][2] + in_vec[2] * mat[2][2];\
  }

#define VEC_nm_postmult(out_vec,in_vec,mat) \
  { \
    out_vec[0] = VEC_dot(mat[0],in_vec); \
    out_vec[1] = VEC_dot(mat[1],in_vec); \
    out_vec[2] = VEC_dot(mat[2],in_vec); \
  }

#define VEC_mult_vector(out_vec,vec1,vec2) \
  { \
    out_vec[0] = vec1[0] * vec2[0]; \
    out_vec[1] = vec1[1] * vec2[1]; \
    out_vec[2] = vec1[2] * vec2[2]; \
  }

#define VEC_sub_constant(out_vec,in_vec,const) \
  { \
    out_vec[0] = in_vec[0] - const; \
    out_vec[1] = in_vec[1] - const; \
    out_vec[2] = in_vec[2] - const;\
  }

#define VEC_line_interpol(out_vec,vec1,vec2,frac) \
  { \
    out_vec[0] = vec1[0] + ((vec2[0]-vec1[0])*frac); \
    out_vec[1] = vec1[1] + ((vec2[1]-vec1[1])*frac); \
    out_vec[2] = vec1[2] + ((vec2[2]-vec1[2])*frac); \
  }

#define VEC_abs_max(v) \
  ( (v[0]>0.0?v[0]:-v[0])>(v[1]>0.0?v[1]:-v[1])? \
   ((v[0]>0.0?v[0]:-v[0])>(v[2]>0.0?v[2]:-v[2])?0:2): \
   ((v[1]>0.0?v[1]:-v[1])>(v[2]>0.0?v[2]:-v[2])?1:2))
