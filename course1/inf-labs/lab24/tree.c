#include "tree.h"

/* TREE */
Tree *node_create(char *str)
{
    Tree *t;

    t = (Tree *)malloc(sizeof(Tree));
    t->data = strdup(str);
    t->left = NULL;
    t->right = NULL;
    return t;
}

void    tree_destroy(Tree *t)
{
    if (t == NULL)
        return ;
    tree_destroy(t->left);
    tree_destroy(t->right);
    free(t->data);
    t->data = NULL;
    free(t);
    t = NULL;
}

void    tree_print(Tree *t, int h)
{
    Tree *i;

    i = t;
    if (i == NULL)
        return ;
    tree_print(i->right, h + 10);
    printf("%*s\n", h, i->data);
    tree_print(i->left, h + 10);
}

/* TREE STACK */

void    tree_stack_create(Tree_stack *s)
{
    s->data = (Tree **)malloc(sizeof(Tree *) * 5);
    s->size = 0;
    s->capacity = 5;
}

void    tree_stack_destroy(Tree_stack *s)
{
    free(s->data);
    s->data = NULL;
    s->size = 0;
    s->capacity = 0;
}

void    tree_stack_resize(Tree_stack *s, int new_size)
{
    Tree **new_data;

    new_data = (Tree **)malloc(sizeof(Tree *) * new_size);
    for (int i = 0; i < s->size; ++i)
        new_data[i] = s->data[i];
    free(s->data);
    s->data = new_data;
    s->capacity = new_size;
}

void    tree_stack_push(Tree_stack *s, Tree *t)
{
    if (s->size == s->capacity)
        tree_stack_resize(s, s->size * 2);
    s->data[s->size] = t;
    s->size++;
}

void    tree_stack_pop(Tree_stack *s)
{
    if (tree_stack_is_empty(s))
        exit(1);
    s->size--;
}

bool    tree_stack_is_empty(Tree_stack *s)
{
    return s->size == 0;
}

Tree    *tree_stack_top(Tree_stack *s)
{
    return s->data[s->size - 1];
}

int     tree_stack_len(Tree_stack *s)
{
    return s->size;
}

void    tree_stack_print(Tree_stack *s)
{
    for (int i = 0;  i < tree_stack_len(s); ++i) {
        printf("%3s", s->data[i]->data);
    }
}
