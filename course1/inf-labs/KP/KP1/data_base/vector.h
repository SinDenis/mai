#ifndef VECTOR_H_
# define VECTOR_H_
# include <stdlib.h>
# include <stdio.h>
# include <string.h>

typedef enum {
    string = 0,
    integer = 1,
    real = 2
} d_type;

typedef struct {
    d_type type;
    union {
        char *str;
        double double_num;
        int int_num;
    };
} elem;

typedef struct {
    elem *data;
    size_t size;
    size_t capacity;
} vector;

vector  vector_create(int size);
size_t  vector_len(vector *v);
void    vector_resize(vector *v, int size);
void    vector_push_back_str(vector *v, char *str);
void    vector_push_back_int(vector *v, int num);
void    vector_push_back_double(vector *v, double num);
void    vector_del_elem(vector *v, size_t idx);
void    *vector_get(vector *v, int idx);
void    vector_print(vector *v);
void    vector_destroy(vector *v);

#endif
