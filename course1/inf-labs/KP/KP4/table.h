#ifndef TABLE_H_
#define TABLE_H_

#include "vector.h"

typedef struct {
    vector_int keys;
    vector_str data;
} Table;

void    table_create(Table *t);
void	table_destroy(Table *t);
void	table_add_entry(Table *t, int key, char *val);
void    table_build_heap(Table *t);
void	table_heap_sort(Table *t);
char	*table_binary_search(Table *t, int key);
void	table_print(Table *t);
void     table_read(Table *t, FILE *f);

#endif
