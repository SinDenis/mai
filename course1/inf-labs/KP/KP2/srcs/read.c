#include "sparse_matrix.h"

static void s_matrix_read_line(vector *v, s_matrix *matrix)
{
	double eps;
	double value;
	int num_line;
	int prev;
	int indent;
	s_matrix_elem *tmp;
	s_matrix_elem m_elem;

	eps = machine_eps();
	num_line = vec_length(&matrix->A);
	prev = -1;
	indent = (int)vec_length(&matrix->B);
	for (int i = 0; i < vec_length(v); ++i) {
		value = *(double *)v->data[i].value;
		if (fabs(value) < eps)
			continue;
		if (vec_length(&matrix->A) == num_line)
			vec_push_back(&matrix->A, INTEGER, i);
		s_matrix_elem_init(&m_elem, i, value, 0);
		if (prev != -1) {
			tmp = (s_matrix_elem *)matrix->B.data[prev + indent].value;
			tmp->index = i;
		}
		vec_push_back(&matrix->B, MATRIX_ELEM, m_elem);
		prev++;
	}
	
	if (vec_length(&matrix->A) == num_line)
		vec_push_back(&matrix->A, INTEGER, -1);		
}

/* Return -1 if error */
int	s_matrix_read(FILE *in, s_matrix *matrix)
{
    double num;
	int state;
    char line[BUF_SIZE];
    vector values;

	state = 0;
	bzero(line, BUF_SIZE);
    while (fgets(line, BUF_SIZE, in) != NULL) {
		line[strlen(line) - 1] = '\0';
        values = strsplit(line, ' ');
		if (state++ == 0)
			matrix->width = vec_length(&values);
		else if (matrix->width != vec_length(&values))
			return -1;
		vec_str_to_double(&values);
		s_matrix_read_line(&values, matrix);
		vec_destroy(&values);
		matrix->height++;
    }
	if (state == 0)
		return -1;
	return 1;
}
