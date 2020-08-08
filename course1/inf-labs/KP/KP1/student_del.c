#include "data_base/db_table.h"

int main(int argc, char *argv[])
{
    db_table students;

    if (argc != 2)
        exit(0);
    students = db_table_load("Students");
    db_table_del_entry(&students, atoi(argv[1]));
    db_table_save(&students);
    return 0;
}
