#include "sparse_matrix.h"

static void divide(s_matrix *matrix, double value, int line, int column)
{
	s_matrix_elem *tmp;
	int num_line;

	num_line = 0;
	for (int i = 0; i < vec_length(&matrix->B); ++i) {
		tmp = (s_matrix_elem *)matrix->B.data[i].value;
		if (tmp->column_num == column || num_line == line) {
			tmp->value /= value;
		}
		if (tmp->index == 0)
			num_line++;
	}
}

static double get_delta(s_matrix *matrix, double value)
{
	double delta;
	s_matrix_elem *tmp;	

	tmp = (s_matrix_elem *)matrix->B.data[0].value;
	delta = fabs(value - tmp->value);
	for (int i = 1; i < vec_length(&matrix->B); ++i) {
		tmp = (s_matrix_elem *)matrix->B.data[i].value;
		if (fabs(value - tmp->value) < delta)
			delta = fabs(value - tmp->value);
	}
	return delta;
}

static vector get_positions(s_matrix *matrix, double value)
{
	vector positions;
	s_matrix_elem *tmp;
	double delta;
	double eps;
	int line;

	eps = machine_eps();
	delta = get_delta(matrix, value);
	positions = vec_create(10);
	line = 0;
	
	for (int i = 0; i < vec_length(&matrix->B); ++i) {
		tmp = (s_matrix_elem *)matrix->B.data[i].value;
		if (fabs(fabs(value - tmp->value) - delta) < eps) {
			vec_push_back(&positions, INTEGER, line);
			vec_push_back(&positions, INTEGER, tmp->column_num);
		}
		if (tmp->index == 0)
			line++;
	}
	return positions;
}

static vector get_values(s_matrix *matrix, double value)
{
	vector values;
	double delta;
	double eps;
	s_matrix_elem *tmp;

	eps = machine_eps();
	delta = get_delta(matrix, value);
	values = vec_create(10);
	for (int i = 0; i < vec_length(&matrix->B); ++i) {
		tmp = (s_matrix_elem*)matrix->B.data[i].value;
		if (fabs(fabs(value - tmp->value) - delta) < eps)
			vec_push_back(&values, DOUBLE, tmp->value);
	}
	return values;
}

vector s_matrix_operation(s_matrix *matrix, double value)
{
	vector values;
	vector positions;
	s_matrix_elem *tmp;
	int n;
	int m;
	double search_value;
	
	values = get_values(matrix, value);
	positions = get_positions(matrix, value);
	for (int i = 0; i < vec_length(&positions); i += 2) {
		search_value = *(double *)values.data[i / 2].value;
		m = *(int *)positions.data[i].value;
		n = *(int *)positions.data[i + 1].value;
		divide(matrix, search_value, m, n);
	}
	return values;
}
