#include "db_table.h"

int type_size[] = {4, 4, 1, 8};

vector_field vector_field_create(size_t size)
{
    vector_field v;

    v.data = malloc(sizeof(field) * size);
    v.size = 0;
    v.capacity = size;
    return v;
}

void vector_field_destroy(vector_field *v)
{
    v->size = 0;
    v->capacity = 0;
    free(v->data);
}

void vector_field_resize(vector_field *v, size_t new_size)
{
    v->capacity = new_size;
    v->data = realloc(v->data, sizeof(field) * new_size);
}

void vector_field_push_back(vector_field *v, field elem)
{
    if (v->size == v->capacity) {
        vector_field_resize(v, v->capacity * 2);
    }
    v->data[v->size] = elem;
    v->size++;
}

void vector_field_del_elem(vector_field *v, size_t idx)
{
    if (idx > v->size - 1) {
        return ;
    }
    for (size_t i = idx; i + 1 < v->size; ++i) {
        v->data[i] = v->data[i + 1];
    }
    v->size--;
}

field vector_field_at(vector_field *v, size_t idx)
{
    return v->data[idx];
}

char *new_str(size_t size) {
    char *str;

    str = (char *)malloc(size * sizeof(char));
    bzero(str, size);
    return str;
}

vector db_table_entry_get(db_table *table, int id)
{
    vector entry;

    entry = vector_create(0);
    if (id < 1 || id > (int)table->num_entries)
        return entry;
    entry = vector_create(10);
    for (size_t i = 1; i < table->num_fields; ++i) {
        switch(table->fields.data[i].type) {
            case INTEGER:
                vector_push_back_int(&entry, table->fields.data[i].entries.data[id - 1].int_num);
                break;
            case LOGICAL:
                vector_push_back_int(&entry, table->fields.data[i].entries.data[id - 1].int_num);
                break;
            case CHAR:
                vector_push_back_str(&entry, table->fields.data[i].entries.data[id - 1].str);
                break;
            case DOUBLE:
                vector_push_back_double(&entry, table->fields.data[i].entries.data[id - 1].double_num);
                break;
        }
    }
    return entry;
}

int db_table_entry_validate(db_table *table, vector *entry)
{
    if (table->num_fields != entry->size + 1)
        return 0;

    for (size_t i = 1; i < table->num_fields; ++i) {
        if (table->fields.data[i].type == CHAR && entry->data[i - 1].type != string)
            return 0;
        if (table->fields.data[i].type == INTEGER && entry->data[i - 1].type != integer)
            return 0;
        if (table->fields.data[i].type == LOGICAL && entry->data[i - 1].type != integer)
            return 0;
        if (table->fields.data[i].type == DOUBLE && entry->data[i - 1].type != real)
            return 0;
    }
    return 1;
}

vector db_table_entry_prepare(db_table *table, vector *entry)
{
    vector prepare_entry;
    char *str;

    str = NULL;
    prepare_entry = vector_create(10);
    for (size_t i = 1; i < table->num_fields; ++i) {
        switch(table->fields.data[i].type) {
            case INTEGER:
                vector_push_back_int(&prepare_entry, entry->data[i - 1].int_num);
                break;
            case LOGICAL:
                vector_push_back_int(&prepare_entry, entry->data[i - 1].int_num);
                break;
            case CHAR:
                str = (char *)malloc(table->fields.data[i].size * sizeof(char));
                bzero(str, table->fields.data[i].size);
                strcpy(str, entry->data[i - 1].str);
                vector_push_back_str(&prepare_entry, str);
                free(str);
                break;
            case DOUBLE:
                vector_push_back_double(&prepare_entry, entry->data[i - 1].double_num);
                break;
        }
    }
    return prepare_entry;
}

void db_table_add_entry_vec(db_table *table, vector *entry)
{
    vector prepare_entry;

    if (db_table_entry_validate(table, entry) == 0)
        return ;
    prepare_entry = db_table_entry_prepare(table, entry);
    vector_push_back_int(&table->fields.data[0].entries, table->num_entries + 1);
    for (size_t i = 1; i < table->num_fields; ++i) {
        switch(table->fields.data[i].type) {
            case INTEGER:
                vector_push_back_int(&table->fields.data[i].entries, prepare_entry.data[i - 1].int_num);
                break;
            case LOGICAL:
                vector_push_back_int(&table->fields.data[i].entries, prepare_entry.data[i - 1].int_num);
                break;
            case CHAR:
                vector_push_back_str(&table->fields.data[i].entries, prepare_entry.data[i - 1].str);
                break;
            case DOUBLE:
                vector_push_back_double(&table->fields.data[i].entries, prepare_entry.data[i - 1].double_num);
                break;
        }
    }
    table->size += db_table_entry_size(table);
    table->num_entries++;
}

void db_table_add_entry(db_table *table, char *format, ...)
{
    va_list arg_list;
    vector entry;

    va_start(arg_list, format);
    entry = vector_create(5);

    for (size_t i = 1; i < strlen(format); i += 2) {
        switch(format[i]) {
            case 's':
                vector_push_back_str(&entry, va_arg(arg_list, char *));
                break;
            case 'd':
                vector_push_back_int(&entry, va_arg(arg_list, int));
                break;
            case 'f':
                vector_push_back_double(&entry, va_arg(arg_list, double));
                break;
        }
    }
    db_table_add_entry_vec(table, &entry);
    vector_destroy(&entry);
}

void db_table_del_entry(db_table *table, int id)
{
    for (size_t i = 0; i < table->num_fields; ++i) {
        vector_del_elem(&table->fields.data[i].entries, id - 1);
    }
    table->num_entries--;
    for (size_t i = id - 1; i < table->num_entries; ++i) {
        table->fields.data[0].entries.data[i].int_num--;
    }
}

void db_table_add_field(db_table *table, char *field_name, field_type type, int num)
{
    field new_field;

    new_field.name = new_str(20);
    strcpy(new_field.name, field_name);
    new_field.type = type;
    new_field.entries = vector_create(20);

    if (type == CHAR) {
        new_field.size = type_size[type] * num + 1;
    } else {
        new_field.size = type_size[type] * num;
    }

    vector_field_push_back(&table->fields, new_field);
    table->num_fields++;
}

size_t db_table_entry_size(db_table *table)
{
    size_t size;

    size = 0;
    for (size_t i = 0; i < table->num_fields; ++i) {
        size += table->fields.data[i].size;
    }
    return size;
}

db_table db_table_create(char *name)
{
    db_table table;

    table.name = name;
    table.fields = vector_field_create(20);
    table.num_fields = 0;
    table.num_entries = 0;
    table.size = 0;
    table.data_size = sizeof(size_t) * 4 + sizeof(char) * 20 + sizeof(field_type) + sizeof(size_t);

    db_table_add_field(&table, "ID", INTEGER, 1);
    return table;
}

void db_table_del_field(db_table *table, char *field_name)
{
    if (table->num_fields == 0) {
        return ;
    }
    for (size_t i = 0; i < table->num_fields; ++i) {
        if (strcmp(vector_field_at(&table->fields, i).name, field_name) == 0) {
            vector_destroy(&table->fields.data[i].entries);
            vector_field_del_elem(&table->fields, i);
            break;
        }
    }
    table->num_fields--;
}

void db_table_print_bin(FILE *out, db_table *table)
{
    char *str;
    int number;

    str = NULL;
    fwrite(&table->num_fields, sizeof(size_t), 1, out);
    fwrite(&table->num_entries, sizeof(size_t), 1, out);
    fwrite(&table->size, sizeof(size_t), 1, out);
    fwrite(&table->data_size, sizeof(size_t), 1, out);
    for (size_t i = 0; i < table->num_fields; ++i) {
        str = new_str(20);
        strcpy(str, table->fields.data[i].name);
        fwrite(str, sizeof(char) * 20, 1, out);
        switch(table->fields.data[i].type) {
            case INTEGER:
                number = 0;
                fwrite(&number, sizeof(int), 1, out);
                break;
            case LOGICAL:
                number = 1;
                fwrite(&number, sizeof(int), 1, out);
                break;
            case CHAR:
                number = 2;
                fwrite(&number, sizeof(int), 1, out);
                break;
            case DOUBLE:
                number = 3;
                fwrite(&number, sizeof(int), 1, out);
                break;
        }
        fwrite(&table->fields.data[i].size, sizeof(size_t), 1, out);
    }
    for (size_t i = 0; i < table->num_fields; ++i) {
        switch (table->fields.data[i].type) {
            case INTEGER:
                for (size_t j = 0; j < table->num_entries; ++j)
                    fwrite(&table->fields.data[i].entries.data[j].int_num, sizeof(int), 1, out);
                break;
            case LOGICAL:
                for (size_t j = 0; j < table->num_entries; ++j)
                    fwrite(&table->fields.data[i].entries.data[j].int_num, sizeof(int), 1, out);
                break;
            case CHAR:
                for (size_t j = 0; j < table->num_entries; ++j)
                    fwrite(table->fields.data[i].entries.data[j].str, table->fields.data[i].size, 1, out);
                break;
            case DOUBLE:
                for (size_t j = 0; j < table->num_entries; ++j)
                    fwrite(&table->fields.data[i].entries.data[j].double_num, table->fields.data[i].size, 1, out);
                break;

        }
    }
}

db_table db_table_load(char *name)
{
    db_table table;
    FILE *db;
    char *bin_file;
    char *name_field;
    int type;
    size_t size_field;
    size_t num_fields;
    int number;
    double d_number;
    char *str;

    table = db_table_create(name);
    bin_file = strdup(name);
    strcat(bin_file, ".bin");

    db = fopen(bin_file, "rb");

    fread(&num_fields, sizeof(size_t), 1, db);
    fread(&table.num_entries, sizeof(size_t), 1, db);
    fread(&table.size, sizeof(size_t), 1, db);
    fread(&table.data_size, sizeof(size_t), 1, db);
    for (size_t i = 0; i < num_fields; ++i) {
        name_field = new_str(20);
        fread(name_field, sizeof(char) * 20, 1, db);
        fread(&type, sizeof(int), 1, db);
        fread(&size_field, sizeof(size_t), 1, db);
        switch(type) {
            case 0:
                if (strcmp(name_field, "ID") != 0)
                    db_table_add_field(&table, name_field, INTEGER, size_field / sizeof(int));
                break;
            case 1:
                db_table_add_field(&table, name_field, LOGICAL, size_field / sizeof(int));
                break;
            case 2:
                db_table_add_field(&table, name_field, CHAR, size_field / sizeof(char) - 1);
                break;
            case 3:
                db_table_add_field(&table, name_field, CHAR, size_field / sizeof(char) - 1);
                break;
        }
        free(name_field);
    }
    table.num_fields = num_fields;

    for (size_t i = 0; i < table.num_fields; ++i) {
        switch (table.fields.data[i].type) {
            case INTEGER:
                for (size_t j = 0; j < table.num_entries; ++j) {
                    fread(&number, sizeof(int), 1, db);
                    vector_push_back_int(&table.fields.data[i].entries, number);
                }
                break;
            case LOGICAL:
                for (size_t j = 0; j < table.num_entries; ++j) {
                    fread(&number, sizeof(int), 1, db);
                    vector_push_back_int(&table.fields.data[i].entries, number);
                }
                break;
            case CHAR:
                for (size_t j = 0; j < table.num_entries; ++j) {
                    str = (char *)malloc(table.fields.data[i].size);
                    fread(str, table.fields.data[i].size, 1, db);
                    vector_push_back_str(&table.fields.data[i].entries, str);
                    free(str);
                }
                break;
            case DOUBLE:
                for (size_t j = 0; j < table.num_entries; ++j) {
                    fread(&d_number, sizeof(double), 1, db);
                    vector_push_back_int(&table.fields.data[i].entries, d_number);
                }
                break;
        }
    }
    fclose(db);
    return table;
}

void db_table_save(db_table *table)
{
    char *bin_file;

    bin_file = strdup(table->name);
    strcat(bin_file, ".bin");

    FILE *db = fopen(bin_file, "wb");
    db_table_print_bin(db, table);
    fclose(db);
}

vector strsplit(char *str, char delim)
{
    char buf[100];
    vector ans;
    int j;

    j = 0;
    ans = vector_create(10);
    for (size_t i = 0; i < strlen(str); ++i) {
        if (str[i] == delim) {
            if (j != 0) {
                buf[i] = '\0';
                vector_push_back_str(&ans, buf);
                bzero(buf, 100);
                j = 0;
            }
        } else {
            buf[j] = str[i];
            j++;
        }
    }
    vector_push_back_str(&ans, buf);
    return ans;
}

char *get_format(db_table *table)
{
    char *str;
    int j;

    j = 0;
    str = (char *)malloc((sizeof(char) * (table->num_fields - 1) * 2));
    for (size_t i = 1; i < table->num_fields; ++i) {
        str[j] = '%';
        switch (table->fields.data[i].type)
        {
            case INTEGER:
                str[j + 1] = 'd';
                break;
            case LOGICAL:
                str[j + 1] = 'd';
                break;
            case CHAR:
                str[j + 1] = 's';
                break;
            case DOUBLE:
                str[j + 1] = 'f';
                break;
        }
        j += 2;
    }
    return str;
}

void db_table_print_txt(FILE *out, db_table *table)
{
    for (size_t i = 0; i < table->num_entries; ++i) {
        for (size_t j = 0; j < table->num_fields; ++j) {
            switch (table->fields.data[j].type) {
                case INTEGER:
                    fprintf(out, "%4d", table->fields.data[j].entries.data[i].int_num);
                    break;
                case LOGICAL:
                    fprintf(out, "%4d", table->fields.data[j].entries.data[i].int_num);
                    break;
                case CHAR:
                    fprintf(out, "%20s", table->fields.data[j].entries.data[i].str);
                    break;
                case DOUBLE:
                    fprintf(out, "%4.2f", table->fields.data[j].entries.data[i].double_num);
                    break;
            }
        }
        fprintf(out, "\n");
    }
}

void db_table_read_txt(FILE *in, db_table *table)
{
    char line[1024];
    vector data;
    vector entry;

    while (fgets(line, 1024, in) != NULL)
    {
        data = strsplit(line, ' ');
        entry = vector_create(10);
        for (size_t j = 0; j < table->num_fields; ++j) {
            switch (table->fields.data[j].type) {
                case INTEGER:
                    if (j > 0)
                        vector_push_back_int(&entry, atoi(data.data[j].str));
                    break;
                case LOGICAL:
                    vector_push_back_int(&entry, atoi(data.data[j].str));
                    break;
                case CHAR:
                    vector_push_back_str(&entry, data.data[j].str);
                    break;
                case DOUBLE:
                    vector_push_back_double(&entry, atof(data.data[j].str));
                    break;
            }
        }
        db_table_add_entry_vec(table, &entry);
        vector_destroy(&data);
        vector_destroy(&entry);
    }
}

char *strslice(char *str, int start, int end)
{
    int size;
    int j;
    char *new_str;

    size = end - start + 1;
    j = 0;
    new_str = (char *)malloc(sizeof(char) * size);
    for (int i = start; i < end; ++i) {
        new_str[j] = str[i];
        ++j;
    }
    new_str[j] = '\0';
    return new_str;
}

int field_is_equal(field *l, field *r)
{
    if (strcmp(l->name, r->name) == 0 && l->type == r->type) {
        return 1;
    }
    return 0;
}
