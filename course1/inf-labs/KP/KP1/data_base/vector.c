#include "vector.h"

vector vector_create(int size)
{
    vector v;
    
    v.data = (elem *)malloc(sizeof(elem) * size);
    v.size = 0;
    v.capacity = size;

    return v;
}

size_t vector_len(vector *v)
{
    return v->size;
}

void vector_resize(vector *v, int size)
{
    vector tmp;

    tmp = vector_create(size);

    for (size_t i = 0; i < vector_len(v); ++i) {
        switch (v->data[i].type) {
            case string:
                vector_push_back_str(&tmp, v->data[i].str);
                break;
            case integer:
                vector_push_back_int(&tmp, v->data[i].int_num);
                break;
            case real:
                vector_push_back_double(&tmp, v->data[i].double_num);
                break;
        }
    }
    vector_destroy(v);
    *v = tmp;
}

void vector_push_back_str(vector *v, char *str)
{
    elem t;

    t.str = (char *)malloc(sizeof(char) * (strlen(str) + 1));
    strcpy(t.str, str);
    t.type = string;

    if (v->size == v->capacity)
        vector_resize(v, v->capacity * 2);
    v->data[v->size] = t;
    v->size++;
}

void vector_push_back_int(vector *v, int num)
{
    elem t;

    t.int_num = num;
    t.type = integer;

    if (v->size == v->capacity)
        vector_resize(v, v->capacity * 2);
    v->data[v->size] = t;
    v->size++;
}

void vector_push_back_double(vector *v, double num)
{
    elem t;

    t.double_num = num;
    t.type = real;

    if (v->size == v->capacity)
        vector_resize(v, v->capacity * 2);
    v->data[v->size] = t;
    v->size++;
}

void *vector_get(vector *v, int idx)
{
    int *int_num;
    double *double_num;

    int_num = NULL;
    double_num = NULL;

    switch (v->data[idx].type) {
        case string:
            return v->data[idx].str;
        case integer:
            int_num = &v->data[idx].int_num;
            return int_num;
        case real:
            double_num = &v->data[idx].double_num;
            return double_num;
    }
}

void vector_print(vector *v)
{
    for (size_t i = 0; i < v->size; ++i) {
        switch (v->data[i].type) {
            case string:
                printf("%s\n", v->data[i].str);
                break;
            case integer:
                printf("%d\n", v->data[i].int_num);
                break;
            case real:
                printf("%f\n", v->data[i].double_num);
                break;
        }
    }
}

void *vector_at(vector *v, size_t idx)
{
    if (idx >= vector_len(v))
        return NULL;
    switch (v->data[idx].type) {
        case string:
            return v->data[idx].str;
        case integer:
            return &v->data[idx].int_num;
        case real:
            return &v->data[idx].double_num;
    }
}

void vector_del_elem(vector *v, size_t idx)
{
    for (size_t i = idx; i + 1 < v->size; ++i) {
        v->data[i] = v->data[i + 1];
    }
    v->size--;
    if (v->size * 2 <= v->capacity) {
        vector_resize(v, v->capacity / 2);
    }
}

void vector_destroy(vector *v)
{
    for (size_t i = 0; i < vector_len(v); ++i)
        if(v->data[i].type == string)
            free(v->data[i].str);
    free(v->data);
    v->data = NULL;
}
