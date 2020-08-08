#include "vector.h"

void		vector_str_create(vector_str *v)
{
	v->data = (char **)malloc(sizeof(char *) * START_SIZE);
	if (v->data == NULL)
		exit(1);
	v->size = 0;
	v->capacity = START_SIZE;
}

void        vector_str_destroy(vector_str *v)
{
	for (int i = 0; i < vector_str_length(v); ++i) {
		free(vector_str_at(v, i));
		vector_str_at_store(v, i,  NULL);
	}
	free(v->data);
	v->data = NULL;
	v->size = 0;
	v->capacity = 0;
}

static void vector_str_resize(vector_str *v, int new_size)
{
	char **new_data;

	new_data = (char **)malloc(sizeof(char *) * new_size);
	for (int i = 0; i < vector_str_length(v); ++i) {
		new_data[i] = strdup(vector_str_at(v, i));
	}
	for (int i = 0; i < vector_str_length(v); ++i) {
		free(vector_str_at(v, i));
		vector_str_at_store(v, i, NULL);
	}
	free(v->data);
	v->data = new_data;
	v->capacity = new_size;
}

void        vector_str_pushback(vector_str *v, char *str)
{
	if (v->size == v->capacity)
		vector_str_resize(v, v->size * 2);
	vector_str_at_store(v, v->size, str);
	v->size++;		
}

void		vector_str_at_store(vector_str *v, int i, char *data)
{
	if (data == NULL)
		v->data[i] = NULL;
	else
		v->data[i] = strdup(data);
}

char		*vector_str_at(vector_str *v, int i)
{
	return v->data[i];
}

int			vector_str_length(vector_str *v)
{
    return v->size;
}

void		vector_str_print(vector_str *v)
{
	for (int i = 0; i < vector_str_length(v); ++i) {
		printf("%s ", vector_str_at(v, i));
	}
}

void		vector_set_to_zero(vector_int *v)
{
	for (int i = 0; i < vector_int_length(v); ++i) {
		vector_int_at_store(v, i, 0);
	}
}

void		vector_int_create(vector_int *v)
{
	v->data = (int *)malloc(sizeof(int) * START_SIZE);
	if (v->data == NULL)
		exit(1);
	v->size = 0;
	v->capacity = START_SIZE;
	vector_set_to_zero(v);
}

void        vector_int_destroy(vector_int *v)
{
	free(v->data);
	v->data = NULL;
	v->size = 0;
	v->capacity = 0;
}

static void	vector_int_resize(vector_int *v, int new_size)
{
	v->data = realloc(v->data, new_size * sizeof(int));
	v->capacity = new_size;
}

void        vector_int_pushback(vector_int *v, int val)
{
	if (v->size == v->capacity)
		vector_int_resize(v, v->size * 2);
	vector_int_at_store(v, v->size, val);
	v->size++;
}

int			vector_int_length(vector_int *v)
{
    return v->size;
}

void		vector_int_at_store(vector_int *v, int i, int data)
{
	v->data[i] = data;
}

int			vector_int_at(vector_int *v, int i)
{
	return v->data[i];
}

void		vector_int_print(vector_int *v)
{
	for (int i = 0; i < vector_int_length(v); ++i) {
		printf("%d ", vector_int_at(v, i));
	}
}
