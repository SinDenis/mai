#include "table.h"

void    table_create(Table *t)
{
    vector_int_create(&t->keys);
    vector_str_create(&t->data);
}

void	table_destroy(Table *t)
{
    vector_int_destroy(&t->keys);
    vector_str_destroy(&t->data);
}

void	table_add_entry(Table *t, int key, char *val)
{
    vector_int_pushback(&t->keys, key);
    vector_str_pushback(&t->data, val);
}

void    swap_int(int *l, int *r)
{
    int tmp;

    tmp = *l;
    *l = *r;
    *r = tmp;
}

void    swap_str(char **l, char **r)
{
    char *tmp;

    tmp = strdup(*l);
    free(*l);
    *l = strdup(*r);
    free(*r);
    *r = strdup(tmp);
    free(tmp);
}

void    table_sift_entry(Table *t, int idx, int max)
{
    int idx_large,
        childl,
        childr;
    
    while (idx < max) {
        idx_large = idx;
        childl = 2 * idx + 1;
        childr = childl + 1;
        if (childl < max &&
        vector_int_at(&t->keys, childl) > vector_int_at(&t->keys, idx_large))
            idx_large = childl;
        if (childr < max &&
        vector_int_at(&t->keys, childr) > vector_int_at(&t->keys, idx_large))
            idx_large = childr;
        if (idx_large != idx) {
            swap_int(&t->keys.data[idx], &t->keys.data[idx_large]);
            swap_str(&t->data.data[idx], &t->data.data[idx_large]);
            idx = idx_large;
        } else
            break;
    }
}

void    table_build_heap(Table *t)
{
    int n = vector_int_length(&t->keys) / 2 - 1;

    while (n >= 0) {
        table_sift_entry(t, n, vector_int_length(&t->keys));
        --n;
    }
}

void	table_heap_sort(Table *t)
{
    int end = vector_int_length(&t->keys) - 1;

    table_build_heap(t);
    while (end >= 0) {
        swap_int(&t->keys.data[0], &t->keys.data[end]);
        swap_str(&t->data.data[0], &t->data.data[end]);
        table_sift_entry(t, 0, end);
        --end;
    }
}

char	*table_binary_search(Table *t, int key)
{
    int left = -1,
        right = vector_int_length(&t->keys),
        middle;

    while (left + 1 < right) {
        middle = (right + left) / 2;
        if (vector_int_at(&t->keys, middle) <= key)
            left = middle;
        else
            right = middle;
    }
    if (vector_int_at(&t->keys, left) != key)
        return NULL;
    return vector_str_at(&t->data, left);
}

int     table_entry_read(Table *t, FILE *f)
{
    int key;
    char val[100000];
    char buf[10000];

    bzero(buf, 10000);
    bzero(val, 100000);
    if (fgets(buf, 10000, f) == NULL)
        return -1;
    key = atoi(buf);
    bzero(buf, 10000);
    while (fgets(buf, 10000, f)) {
        if (strcmp(buf, "\n") == 0)
            break;
        strcat(val, buf);
        bzero(buf, 10000);
    }
    vector_int_pushback(&t->keys, key);
    vector_str_pushback(&t->data, val);
    return 1;
}

void     table_read(Table *t, FILE *f)
{
    int n;

    do {
        n = table_entry_read(t, f);
    } while (n != -1);
}

void	table_print(Table *t)
{
    for (int i = 0; i < vector_int_length(&t->keys); ++i) {
        fprintf(stdout, "%d:\n%s\n", vector_int_at(&t->keys, i), vector_str_at(&t->data, i));
    }
    fprintf(stdout, "\n");
}
