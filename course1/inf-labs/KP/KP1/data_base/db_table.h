#ifndef DB_TABLE_H_
#define DB_TABLE_H_

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "vector.h"

extern int type_size[4];

typedef enum {
    INTEGER = 0,
    LOGICAL = 1,
    CHAR = 2,
    DOUBLE = 3
} field_type;

typedef struct {
    char *name;
    field_type type;
    size_t size;
    vector entries;
} field;

typedef struct {
    field   *data;
    size_t  size;
    size_t  capacity;
} vector_field;

typedef struct {
    char *name;
    vector_field fields;
    size_t num_fields;
    size_t num_entries;
    size_t size;
    size_t data_size;
} db_table;

db_table        db_table_create(char *name);
void            db_table_add_field(db_table *table, char *field_name, field_type type, int num);
void            db_table_del_field(db_table *table, char *field_name);
void            db_table_add_entry(db_table *table, char *format, ...);
void            db_table_add_entry_vec(db_table *table, vector *entry);
void            db_table_del_entry(db_table *table, int id);
void            db_table_print_txt(FILE *out, db_table *table);
void            db_table_read_txt(FILE *in, db_table *table);
void            db_table_print_bin(FILE *out, db_table *table);
void            db_table_read_bin(FILE *in, db_table *table);
db_table        db_table_load(char *name);
void            db_table_save(db_table *table);
vector          db_table_entry_get(db_table *table, int id);
int             db_table_entry_validate(db_table *table, vector *entry);
vector          db_table_entry_prepare(db_table *table, vector *entry);
size_t          db_table_entry_size(db_table *table);
char            *get_format(db_table *table);

vector_field    vector_field_create(size_t size);
void            vector_field_resize(vector_field *v, size_t new_size);
void            vector_field_push_back(vector_field *v, field elem);
void            vector_field_del_elem(vector_field *v, size_t idx);
field           vector_field_at(vector_field *v, size_t idx);
void            vector_field_destroy(vector_field *v);

int             field_is_equal(field *l, field *r);

#endif
