#include "stack.h"
#include "sort.h"

void help(void)
{
    printf("PUSH: Press a\n");
    printf("POP: Press d\n");
    printf("TOP: Press t\n");
    printf("SORT: Press s\n");
    printf("PRINT: Press p\n");
    printf("SIZE: Press l\n");
    printf("EXIT: Press q\n");
}

int main(void)
{
    stack s;
    data_type *data;
    char value[10000];
    int key;
    char c;

    stack_create(&s);
    help();
    while (true) {
        scanf("%c", &c);
        switch (c) {
            case 'a':
                printf("Enter key: ");
                scanf("%d", &key);
                printf("Enter value: ");
                scanf("%s", value);
                stack_add(&s, key, value);
            break;
            case 'd':
                stack_pop(&s);
            break;
            case 't':
                data = stack_top(&s);
                if (data != NULL)
                    data_type_print(data);
                else
                    printf("Stack is empty");
            break;
            case 'p':
                stack_print(&s);
            break;
            case 's':
                rec_insertion_sort(&s);
            break;
            case 'l':
                printf("%zu\n", stack_length(&s));
            break;
            case 'h':
                help();
            break;
            case 'q':
                stack_destroy(&s);
                return 0;
            break;
        }
    }
    return 0;
}
