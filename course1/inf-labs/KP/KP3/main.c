#include "list.h"

void help(void)
{
    printf("PUSH: Press a\n");
    printf("DEL: Press d\n");
    printf("PRINT: Press p\n");
    printf("SIZE: Press l\n");
    printf("OPERATION: Press o\n");
    printf("EXIT: Press q\n");
}

int main(void)
{
    List lst;
    char value[10000];
    char c;

    list_create(&lst);
    help();
    while (true) {
        scanf("%c", &c);
        switch (c) {
            case 'a':
                printf("Enter idx: ");
                scanf("%s", value);
                int idx = atoi(value);
                printf("Enter value: ");
                scanf("%s", value);
                if ((size_t)idx == list_len(&lst)) {
                    list_push_back(&lst, atoi(value));
                } else if ((size_t)idx < list_len(&lst) && idx >= 0) {
                    list_push(&lst, idx, atoi(value));
                } else {
                    printf("not valid idx\n");
                }
            break;
            case 'd':
                printf("Enter idx: ");
                scanf("%s", value);
                int i = atoi(value);
                if (i >= 0 && (size_t)i < list_len(&lst))
                    list_del(&lst, i);
                else
                    printf("not valid idx\n");
            break;
            case 'p':
                list_print(&lst);
            break;
            case 'o':
                printf("Enter value: ");
                scanf("%s", value);
                list_del_more_less_value(&lst, atoi(value));
            break;
            case 'l':
                printf("%zu\n", list_len(&lst));
            break;
            case 'h':
                help();
            break;
            case 'q':
                list_destroy(&lst);
                return 0;
            break;
        }
    }
    return 0;
}
