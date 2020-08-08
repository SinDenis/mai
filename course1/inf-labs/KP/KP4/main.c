#include <stdio.h>
#include <string.h>
#include "table.h"

int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("Using: ./main file key1 key2 ... kyen\n");
        exit(1);
    }

    FILE *f = fopen(argv[1], "r");
    if (f == NULL) {
        printf("File not exist");
        exit(1);
    }
    // int i;
    // char *res;
    Table t;

    table_create(&t);
    table_read(&t, f);
    // table_print(&t);
    table_heap_sort(&t);
    // table_print(&t);
    // i = 2;
    // while (i < argc) {
    //     res = table_binary_search(&t, atoi(argv[i]));
    //     if (res == NULL)
    //         printf("%d: Key not found\n", atoi(argv[i]));
    //     else
    //         printf("%d:\n%s", atoi(argv[i]), res);
    //     ++i;
    // }
    table_destroy(&t);
    fclose(f);
    return (0);
}
