#ifndef SPARSE_MATRIX_H
#define SPARSE_MATRIX_H

#include "../vector/vector.h"
#include <math.h>
#define BUF_SIZE 10000

typedef struct {
	vector A;
	vector B;
	int width;
	int height;	
} s_matrix;

void 		s_matrix_elem_init(s_matrix_elem *sm_elem, size_t column_num, double value, size_t index);
s_matrix    s_matrix_create(void);
int         s_matrix_read(FILE *in, s_matrix *matrix);
void        s_matrix_print(FILE *out, s_matrix *matrix);
void		s_matrix_scheme_print(FILE *out, s_matrix *matrix);
vector      s_matrix_operation(s_matrix *matrix, double value);

/*Supporting functions*/
double		machine_eps(void);
vector		strsplit(char *str, char delim);
void		vec_str_to_double(vector *v);

#endif
