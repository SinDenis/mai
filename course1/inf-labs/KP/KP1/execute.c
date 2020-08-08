#include "execute.h"

int main(int argc, char *argv[])
{
    db_table students;
    vector one_entry;
    int num;

    if (argc != 2)
        exit(1);

    students = db_table_load("Students");

    num = 0;
    for (size_t i = 1; i <= students.num_entries; ++i) {
        one_entry = db_table_entry_get(&students, i);
        if ((one_entry.data[2].str[0] == 'W' || one_entry.data[2].str[0] == 'w')&&
            strcmp(one_entry.data[3].str, argv[1]) == 0 &&
            one_entry.data[4].int_num > 3 && 
            one_entry.data[5].int_num > 3 &&
            one_entry.data[6].int_num > 3 &&
            one_entry.data[7].int_num > 3)
            num++;
        vector_destroy(&one_entry);
    }
    printf("Количество студенток группы %s, которые получают стипендию: %d\n", argv[1], num);
    return 0;
}
