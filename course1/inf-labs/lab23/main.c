#include "bintree.h"
#include <time.h>

void help(void)
{
    printf("INSERT: Press a\n");
    printf("DELETE: Press d\n");
    printf("PRINT: Press p\n");
    printf("HEIGHT: Press h\n");
    printf("INSTRUCTIONs: Press i\n");
    printf("EXIT: Press q\n");
}

int main(void)
{
    Bintree tree;
    char value[10000];
    char c;

    tree = tree_create();
    help();
    while (true) {
        scanf("%c", &c);
        switch (c) {
            case 'a':
                printf("Enter key: ");
                scanf("%s", value);
                tree_insert(&tree, atoi(value));
            break;
            case 'd':
                printf("Enter key: ");
                scanf("%s", value);
                if (tree_del(&tree, atoi(value)) == -1)
                    printf("This key is not exist\n");
            break;
            case 'p':
                tree_print(&tree);
            break;
            case 'h':
                printf("%d\n", tree_height(&tree));
            break;
            case 'i':
                help();
            break;
            case 'q':
                tree_destroy(&tree);
                return 0;
            break;
        }
    }
    return 0;
}
