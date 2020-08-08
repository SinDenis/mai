#ifndef OPN_H_
#define OPN_H_

#include "deq.h"
#include <ctype.h>
#include <stdbool.h>
#include "tree.h"

typedef enum {
    SPACE = 0,
    NUMBER = 1,
    LETTER = 2,
    BREACKET = 3,
    OPERATION = 4
} State;

Deq     *opn(char *str);
Tree    *get_tree_expression(Deq *pn);
Tree    *tree_transform(Tree *t);
Tree    *tree_calc(Tree *t);

#endif
