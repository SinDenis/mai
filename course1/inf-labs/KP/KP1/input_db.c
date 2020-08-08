#include "input.h"


int main()
{
    db_table students;

    students = db_table_load("Students");

    db_table_print_txt(stdout, &students);
    return 0;
}
