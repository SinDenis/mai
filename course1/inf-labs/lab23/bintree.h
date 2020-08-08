#ifndef BINTREE_H_
#define BINTREE_H_

#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"

typedef int data_type;

typedef struct elem {
    data_type key;
    struct elem *left;
    struct elem *right;
} Node;

typedef struct {
    Node *root;
    int size;
} Bintree;

Bintree     tree_create(void);
Node        *node_create(data_type elem);
bool        node_is_leaf(Node *node);
void        tree_insert(Bintree *tree, data_type key);
int         tree_del(Bintree *tree, data_type key);
void        tree_destroy(Bintree *tree);
void        tree_print(Bintree *tree);
int         tree_height(Bintree *tree);

#endif
