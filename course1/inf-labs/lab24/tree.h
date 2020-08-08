#ifndef TREE_H_
#define TREE_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum {
    NUM = 0,
    STR = 1,
    OP = 2
} Node_type;

typedef struct node {
    char        *data;
    Node_type   type;
    struct node *left;
    struct node *right;
} Tree;

typedef struct {
    Tree **data;
    int size;
    int capacity;
} Tree_stack;

/* TREEE */
Tree    *node_create(char *str);
void    tree_destroy(Tree *t);
void    tree_print(Tree *t, int h);
void    tree_to_expr(Tree *t, Tree_stack *s);

/* TREE STACK */
void    tree_stack_create(Tree_stack *s);
void    tree_stack_destroy(Tree_stack *s);

void    tree_stack_push(Tree_stack *s, Tree *t);
void    tree_stack_pop(Tree_stack *s);
bool    tree_stack_is_empty(Tree_stack *s);
Tree    *tree_stack_top(Tree_stack *s);
int     tree_stack_len(Tree_stack *s);
void    tree_stack_print(Tree_stack *s);

#endif
