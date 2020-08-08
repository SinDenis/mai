#include "sparse_matrix.h"

s_matrix s_matrix_create(void)
{
    s_matrix matrix;

    matrix.A = vec_create(10);
    matrix.B = vec_create(10);
	matrix.width = 0;
	matrix.height = 0;
    return matrix;
}

void s_matrix_elem_init(s_matrix_elem *sm_elem, size_t column_num, double value, size_t index)
{
	sm_elem->column_num = column_num;
	sm_elem->value = value;
	sm_elem->index = index;
}
