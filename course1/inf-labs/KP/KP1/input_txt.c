#include "input.h"

int main()
{
    FILE *table = fopen("Students.txt", "r");
    db_table table_students = db_table_create("Students");

    db_table_add_field(&table_students, "Surname", CHAR, 30);
    db_table_add_field(&table_students, "Initials", CHAR, 4);
    db_table_add_field(&table_students, "Sex", CHAR, 1);
    db_table_add_field(&table_students, "Group", CHAR, 20);
    db_table_add_field(&table_students, "Matan", INTEGER, 1);
    db_table_add_field(&table_students, "Algebra", INTEGER, 1);
    db_table_add_field(&table_students, "DM", INTEGER, 1);
    db_table_add_field(&table_students, "Informatic", INTEGER, 1);

    db_table_read_txt(table, &table_students);
    fclose(table);
    db_table_print_txt(stdout, &table_students);
    return 0;
}
