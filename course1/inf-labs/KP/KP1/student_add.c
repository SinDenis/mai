#include "data_base/db_table.h"

int main(int argc, char *argv[])
{
    vector entry;
    db_table students;

    if (argc < 2)
        exit(1);
    students = db_table_load("Students");
    entry = vector_create(10);
    for (size_t i = 1; i < students.num_fields; ++i) {
        switch (students.fields.data[i].type)
        {
            case INTEGER:
                vector_push_back_int(&entry, atoi(argv[i]));
                break;
            case LOGICAL:
                vector_push_back_int(&entry, atoi(argv[i]));
                break;
            case CHAR:
                vector_push_back_str(&entry, argv[i]);
                break;
            case DOUBLE:
                vector_push_back_double(&entry, atof(argv[i]));
                break;
        }
    }
    db_table_add_entry_vec(&students, &entry);
    vector_destroy(&entry);
    db_table_save(&students);
    return (0);
}
