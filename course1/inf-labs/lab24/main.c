#include "opn.h"

int main(void)
{
    Deq *pn;
    Tree_stack s;
    Tree *t;
    char c;
    char buf[1024];
    while (1) {
        tree_stack_create(&s);
        bzero(buf, 1000);
        fgets(buf, 1000, stdin);
        pn = opn(buf);
        deq_print(pn);
        t = get_tree_expression(pn);
        printf("Исходное дерево выражений:\n");
        tree_print(t, 3);
        t = tree_transform(t);
        t = tree_calc(t);
        printf("Преобразованное дерево выражений:\n");
        tree_print(t, 3);
        printf("Преобразованное выражение:\n");
        tree_to_expr(t, &s);
        printf("\n");
        tree_destroy(t);
        deq_destroy(&pn);
        tree_stack_destroy(&s);
        printf("exit -> @\ncontinue -> d expression\n");
        scanf("%c", &c);
        if (c == '@')
            break;
        else
            continue;
    }
    return 0;
}
