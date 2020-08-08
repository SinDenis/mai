#include "vector.h"

vector vec_create(size_t size)
{
	vector v;

	v.data = (elem *)malloc(sizeof(elem) * size);
	v.size = 0;
	v.capacity = size;
	return v;
}

void vec_resize(vector *v, size_t new_size)
{
	elem *new_data;

	new_data = (elem *)malloc(sizeof(elem) * new_size);
	for (size_t i = 0; i < v->size; ++i)
		memcpy(&new_data[i], &v->data[i], sizeof(v->data[i]));
	free(v->data);
	v->data = new_data;
	v->capacity = new_size;
}

/* Add new case if add new data type */
void vec_set_value(vector *v, int i, data_type type, ...)
{
	va_list elems;
	TYPE1 var1;
	TYPE2 var2;
	TYPE4 var4;	

	va_start(elems, type);
	if (i < 0 || i >= v->capacity)
		return ;
	v->data[i].type = type;
	free(v->data[i].value);
	switch (type) {
		case INTEGER:	
			var1 = va_arg(elems, TYPE1);
			v->data[i].value = (void *)malloc(sizeof(TYPE1));
			memcpy(v->data[i].value, (void *)&var1, sizeof(TYPE1));
		break;

		case DOUBLE:
			var2 = va_arg(elems, TYPE2);
			v->data[i].value = (void *)malloc(sizeof(TYPE2));
			memcpy(v->data[i].value, (void*)&var2, sizeof(TYPE2));
		break;

		case STRING:
			v->data[i].value = (void *)strdup(va_arg(elems, TYPE3));
		break;

		case MATRIX_ELEM:
			var4 = va_arg(elems, TYPE4);
			v->data[i].value = (void *)malloc(sizeof(TYPE4));
			memcpy(v->data[i].value, &var4, sizeof(TYPE4));
		break;
	}
	va_end(elems);
}

/* Add new case if add new data type */
void vec_push_back(vector *v, data_type type, ...)
{
	va_list elems;
	TYPE1 var1;
	TYPE2 var2;
	TYPE4 var4;

	va_start(elems, type);
	if (v->size == v->capacity)
		vec_resize(v, v->size * 2);
	v->data[v->size].type = type;
	switch (type) {
		case INTEGER:
			var1 = va_arg(elems, TYPE1);
			v->data[v->size].value = (void *)malloc(sizeof(TYPE1));
			memcpy(v->data[v->size].value, (void *)&var1, sizeof(TYPE1));
		break;
		case DOUBLE:
			var2 = va_arg(elems, TYPE2);
			v->data[v->size].value = (void *)malloc(sizeof(TYPE2));
			memcpy(v->data[v->size].value, (void*)&var2, sizeof(TYPE2));
		break;
		case STRING:
			v->data[v->size].value = (void *)strdup(va_arg(elems, TYPE3));
		break;
		case MATRIX_ELEM:
			var4 = va_arg(elems, TYPE4);
			v->data[v->size].value = (void *)malloc(sizeof(TYPE4));
			memcpy(v->data[v->size].value, &var4, sizeof(TYPE4));
		break;
	}
	va_end(elems);
	v->size++;
}

size_t vec_length(vector *v)
{
	return v->size;
}

void vec_print(FILE *out, vector *v)
{
	TYPE4 var4;

	for (int i = 0; i < v->size; ++i) {				
		switch (v->data[i].type) {
			case INTEGER:
				fprintf(out, "%d ", *(int *)v->data[i].value);
			break;
			case DOUBLE:
				fprintf(out, "%f ", *(double *)v->data[i].value);
			break;
			case STRING:
				fprintf(out, "%s ", (char *)v->data[i].value);
			break;
			case MATRIX_ELEM:
				var4 = *(TYPE4 *)v->data[i].value;
				fprintf(out, "%zu:%f:%zu ", var4.column_num, var4.value, var4.index);
			break;
		}
	}
}

void vec_destroy(vector *v)
{
	for (int i = 0; i < vec_length(v); ++i) {
		free(v->data[i].value);
		v->data[i].value = NULL;
	}
	free(v->data);
	v->data = NULL;
}
