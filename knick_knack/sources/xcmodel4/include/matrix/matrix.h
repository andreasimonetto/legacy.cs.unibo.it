/* mat_lib.h */

extern BOOLEAN MAT_equal(VEC_real_t *m1, VEC_real_t *m2);

extern void MAT_identity(VEC_matrix_4x3_t m);

extern void MAT_null(VEC_real_t *m);

extern void MAT_add(VEC_real_t *m, VEC_real_t *m1, 
		    VEC_real_t *m2);

extern void MAT_sub(VEC_real_t *m, VEC_real_t *m1, 
		    VEC_real_t *m2);

extern void MAT_copy(VEC_real_t *m1, VEC_real_t *m2);

extern void MAT_multiply(VEC_matrix_4x3_t m, 
			 VEC_matrix_4x3_t m1, 
			 VEC_matrix_4x3_t m2);

extern void MAT_x_rotate_abs(VEC_matrix_4x3_t m, VEC_real_t c, 
			     VEC_real_t s);

extern void MAT_x_rotate_rel(VEC_matrix_4x3_t m, VEC_real_t c, 
			     VEC_real_t s);

extern void MAT_y_rotate_abs(VEC_matrix_4x3_t m, VEC_real_t c, 
			     VEC_real_t s);

extern void MAT_y_rotate_rel(VEC_matrix_4x3_t m, VEC_real_t c, 
			     VEC_real_t s);

extern void MAT_z_rotate_abs(VEC_matrix_4x3_t m, VEC_real_t c, 
			     VEC_real_t s);

extern void MAT_z_rotate_rel(VEC_matrix_4x3_t m, VEC_real_t c, 
			     VEC_real_t s);

extern void MAT_scale_abs(VEC_matrix_4x3_t m, VEC_real_t a, 
			  VEC_real_t b, VEC_real_t c);

extern void MAT_scale_rel(VEC_matrix_4x3_t m, VEC_real_t a, 
			  VEC_real_t b, VEC_real_t c);

extern void MAT_translate_abs(VEC_matrix_4x3_t m, VEC_real_t a, 
			      VEC_real_t b, VEC_real_t c);

extern void MAT_translate_rel(VEC_matrix_4x3_t m, VEC_real_t a,
			      VEC_real_t b, VEC_real_t c);

extern void MAT_yz_shear_abs(VEC_matrix_4x3_t m, VEC_real_t a,
			    VEC_real_t b);

extern void MAT_yz_shear_rel(VEC_matrix_4x3_t m, VEC_real_t a, 
			    VEC_real_t b);

extern void MAT_xz_shear_abs(VEC_matrix_4x3_t m, VEC_real_t a, 
			    VEC_real_t b);

extern void MAT_xz_shear_rel(VEC_matrix_4x3_t m, VEC_real_t a,
			    VEC_real_t b);

extern void MAT_xy_shear_abs(VEC_matrix_4x3_t m, VEC_real_t a,
			    VEC_real_t b);

extern void MAT_xy_shear_rel(VEC_matrix_4x3_t m, VEC_real_t a, 
			    VEC_real_t b);

extern void MAT_convert_to_4x4(VEC_matrix_4x4_t m1, 
			       VEC_matrix_4x3_t m2);

extern void MAT_convert_to_4x3(VEC_matrix_4x3_t m1, 
			       VEC_matrix_4x4_t m2);

extern void MAT_identity_4x4(VEC_matrix_4x4_t m);

extern void MAT_null_4x4(VEC_real_t *m);

extern void MAT_transpose_4x4(VEC_matrix_4x4_t m1, 
			      VEC_matrix_4x4_t m2);

extern void MAT_copy_4x4(VEC_real_t *m1, VEC_real_t *m2);

extern void MAT_multiply_4x4(VEC_matrix_4x4_t m, 
			     VEC_matrix_4x4_t m1, 
			     VEC_matrix_4x4_t m2);

extern VEC_real_t VEC_max(VEC_vector_t start_vec,  
			  VEC_real_t *result, 
			  int *result_loc);

extern VEC_real_t VEC_min(VEC_vector_t start_vec, 
			  VEC_real_t *result, 
			  int *result_loc);

extern void VEC_triple(VEC_vector_t result_vec, 
		       VEC_vector_t a, VEC_vector_t b, 
		       VEC_vector_t c);

extern BOOLEAN MAT_inverse(VEC_matrix_4x3_t Ainv, 
			   VEC_matrix_4x3_t A);

extern BOOLEAN MAT_inverse_4x4(VEC_matrix_4x4_t Ainv, 
			       VEC_matrix_4x4_t A);

extern VEC_real_t MAT_det_4x4(VEC_matrix_4x4_t A);

extern VEC_real_t MAT_det_3x3(VEC_real_t B[3][3]);

extern void minore_4x4(VEC_real_t B[3][3], 
		       VEC_matrix_4x4_t A, 
		       int i, int j);

extern void MAT_transform_plane(VEC_real_t *result, 
				VEC_real_t *start_eq, 
				VEC_matrix_4x3_t inv_mat);




