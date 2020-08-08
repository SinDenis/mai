#include "sparse_matrix.h"

static void zero_print(FILE *out, int num)
{
	for (int i = 0; i < num; ++i)
		fprintf(out, "%10d", 0);
}

void s_matrix_scheme_print(FILE *out, s_matrix *matrix)
{
	vec_print(out, &matrix->A);
	fprintf(out, "\n");
	vec_print(out, &matrix->B);
	fprintf(out, "\n");
}

void s_matrix_print(FILE *out, s_matrix *matrix)
{
	int iter;
	int delta;
	s_matrix_elem tmp;

	iter = 0;
	for (int i = 0; i < matrix->height; ++i) {
		if (*(int *)matrix->A.data[i].value == -1) {
			zero_print(out, matrix->width);
			fprintf(out, "\n");
			continue;
		}
		zero_print(out, *(int *)matrix->A.data[i].value);
		tmp = *(s_matrix_elem *)matrix->B.data[iter].value;
		iter++;
		fprintf(out, "%10f", tmp.value);
		while (tmp.index != 0) {
			delta = tmp.column_num;
			tmp = *(s_matrix_elem *)matrix->B.data[iter].value;
			iter++;
			delta = tmp.column_num - delta - 1;
			zero_print(out, delta);
			fprintf(out, "%10f", tmp.value);
		}
		zero_print(out, matrix->width - tmp.column_num - 1);
		fprintf(out, "\n");
	}
}
