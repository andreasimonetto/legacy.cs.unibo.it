/* mat_lib.h */


BOOLEAN MAT_equal(VEC_real_t *m1, VEC_real_t *m2);

void MAT_identity(VEC_matrix_4x3_t m);

void MAT_null(VEC_real_t *m);

void MAT_add(VEC_real_t *m, VEC_real_t *m1, 
	     VEC_real_t *m2);

void MAT_sub(VEC_real_t *m, VEC_real_t *m1, 
	     VEC_real_t *m2);

void MAT_copy(VEC_real_t *m1, VEC_real_t *m2);

void MAT_multiply(VEC_matrix_4x3_t m, 
		  VEC_matrix_4x3_t m1, 
		  VEC_matrix_4x3_t m2);

void MAT_x_rotate_abs(VEC_matrix_4x3_t m, VEC_real_t c, 
		 VEC_real_t s);

void MAT_x_rotate_rel(VEC_matrix_4x3_t m, VEC_real_t c, 
		 VEC_real_t s);

void MAT_y_rotate_abs(VEC_matrix_4x3_t m, VEC_real_t c, 
		      VEC_real_t s);

void MAT_y_rotate_rel(VEC_matrix_4x3_t m, VEC_real_t c, 
		 VEC_real_t s);

void MAT_z_rotate_abs(VEC_matrix_4x3_t m, VEC_real_t c, 
		      VEC_real_t s);

void MAT_z_rotate_rel(VEC_matrix_4x3_t m, VEC_real_t c, 
		 VEC_real_t s);

void MAT_scale_abs(VEC_matrix_4x3_t m, VEC_real_t a, 
		   VEC_real_t b, VEC_real_t c);

void MAT_scale_rel(VEC_matrix_4x3_t m, VEC_real_t a, 
		   VEC_real_t b, VEC_real_t c);

void MAT_translate_abs(VEC_matrix_4x3_t m, VEC_real_t a, 
		       VEC_real_t b, VEC_real_t c);

void MAT_translate_rel(VEC_matrix_4x3_t m, VEC_real_t a,
		       VEC_real_t b, VEC_real_t c);

void MAT_yz_shear_abs(VEC_matrix_4x3_t m, VEC_real_t a,
		     VEC_real_t b);

void MAT_yz_shear_rel(VEC_matrix_4x3_t m, VEC_real_t a, 
		     VEC_real_t b);

void MAT_xz_shear_abs(VEC_matrix_4x3_t m, VEC_real_t a, 
		     VEC_real_t b);

void MAT_xz_shear_rel(VEC_matrix_4x3_t m, VEC_real_t a,
		     VEC_real_t b);

void MAT_xy_shear_abs(VEC_matrix_4x3_t m, VEC_real_t a,
		     VEC_real_t b);

void MAT_xy_shear_rel(VEC_matrix_4x3_t m, VEC_real_t a, 
		     VEC_real_t b);

void MAT_convert_to_4x4(VEC_matrix_4x4_t m1, 
			VEC_matrix_4x3_t m2);

void MAT_convert_to_4x3(VEC_matrix_4x3_t m1, 
			VEC_matrix_4x4_t m2);

void MAT_identity_4x4(VEC_matrix_4x4_t m);

void MAT_null_4x4(VEC_real_t *m);

void MAT_transpose_4x4(VEC_matrix_4x4_t m1, 
		       VEC_matrix_4x4_t m2);

void MAT_copy_4x4(VEC_real_t *m1, VEC_real_t *m2);

void MAT_multiply_4x4(VEC_matrix_4x4_t m, 
		      VEC_matrix_4x4_t m1, 
		      VEC_matrix_4x4_t m2);

VEC_real_t VEC_max(VEC_vector_t start_vec,  
		   VEC_real_t *result, 
		   int *result_loc);

VEC_real_t VEC_min(VEC_vector_t start_vec, 
		   VEC_real_t *result, 
		   int *result_loc);

void VEC_triple(VEC_vector_t result_vec, 
		VEC_vector_t a, VEC_vector_t b, 
		VEC_vector_t c);

BOOLEAN MAT_inverse(VEC_matrix_4x3_t Ainv, 
		    VEC_matrix_4x3_t A);

BOOLEAN MAT_inverse_4x4(VEC_matrix_4x4_t Ainv, 
		     VEC_matrix_4x4_t A);

VEC_real_t MAT_det_4x4(VEC_matrix_4x4_t A);

VEC_real_t MAT_det_3x3(VEC_real_t B[3][3]);

void minore_4x4(VEC_real_t B[3][3], 
		       VEC_matrix_4x4_t A, 
		       int i, int j);

void MAT_transform_plane(VEC_real_t *result, 
		    VEC_real_t *start_eq, 
		    VEC_matrix_4x3_t inv_mat);




