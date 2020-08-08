#ifndef VECTOR_H_
#define VECTOR_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define START_SIZE 5

typedef struct {
    char **data;
    int size;
    int capacity;
} vector_str;

typedef struct {
    int *data;
    int size;
    int capacity;
} vector_int;

void        vector_str_create(vector_str *v);
void        vector_str_destroy(vector_str *v);
void        vector_str_pushback(vector_str *v, char *str);
int         vector_str_length(vector_str *v);
char		*vector_str_at(vector_str *v, int i);
void		vector_str_at_store(vector_str *v, int i, char *data);
void		vector_str_print(vector_str *v);

void        vector_int_create(vector_int *v);
void        vector_int_destroy(vector_int *v);
void        vector_int_pushback(vector_int *v, int val);
int         vector_int_length(vector_int *v);
int	        vector_int_at(vector_int *v, int i);
void		vector_int_at_store(vector_int *v, int i, int data);
void		vector_int_print(vector_int *v);

#endif
