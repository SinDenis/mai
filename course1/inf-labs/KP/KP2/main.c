#include "srcs/sparse_matrix.h"
#include <stdio.h>


int main(int argc, char *argv[])
{
	s_matrix matrix;
	vector values;
	FILE *fin;

	matrix = s_matrix_create();
	fprintf(stdout, "%s\n", argv[1]);
	if (argc != 3) {
		fprintf(stdout, "Usage: ./sparse_matrix filename value\n");
		exit(1);
	}
 	fin = fopen(argv[1], "r");
	if (fin == NULL) {
		fprintf(stdout, "File does not exist\n");
		exit(1);
	}
	if (s_matrix_read(fin, &matrix) == -1) {
		fprintf(stdout, "Not valid matrix\n");
		exit(1);
	}

	fprintf(stdout, "Original matrix\n");
	s_matrix_print(stdout, &matrix);
	fprintf(stdout, "\n");

	fprintf(stdout, "Original Scheme of matrix\n");
	s_matrix_scheme_print(stdout, &matrix);
	fprintf(stdout, "\n");

	fprintf(stdout, "Processed matrix\n");
	values = s_matrix_operation(&matrix, atof(argv[2]));
	s_matrix_print(stdout, &matrix);
	fprintf(stdout, "\n");

	fprintf(stdout, "Processed scheme of matrix\n");
	s_matrix_scheme_print(stdout, &matrix);
	fprintf(stdout, "\n");

	fprintf(stdout, "Processed values:\n");
	vec_print(stdout, &values);
	fprintf(stdout, "\n\n");
	return 0;
}
