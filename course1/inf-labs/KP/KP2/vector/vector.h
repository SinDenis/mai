#ifndef VECTOR_H_
#define VECTOR_H_

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

/*
#define TYPE1 int
#define TYPE2 double
#define TYPE3 char *
*/

/* Write data types of variables of vector */

typedef struct {
	size_t column_num;
	double value;
	size_t index;
} s_matrix_elem;

typedef int				TYPE1;
typedef double			TYPE2;
typedef char *			TYPE3;
typedef s_matrix_elem	TYPE4;

/* Add data types */
/* data types */
typedef enum {
	INTEGER 	 = 0,
	DOUBLE  	 = 1,
	STRING  	 = 2,
	MATRIX_ELEM  = 3
} data_type;

/* Add variables of necessary type */
/* Element of vector */
typedef struct {
	data_type	type;
	void 		*value;
} elem;

/* Main structure vector */
typedef struct {
	elem	*data;
	size_t	size;
	size_t	capacity;
} vector;

vector	vec_create(size_t size);
void	vec_resize(vector *v, size_t size);
void	vec_set_value(vector *v, int i, data_type type, ...);
void	vec_push_back(vector *v, data_type type, ...);
void	vec_print(FILE *out, vector *v);
size_t	vec_length(vector *v);
void	vec_destroy(vector *v);

#endif
