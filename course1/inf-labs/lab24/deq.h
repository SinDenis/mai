#ifndef Deq_H_
#define Deq_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef struct elem {
    char        *data;
    struct elem *next;
    struct elem *prev;
} Node;

typedef struct {
    Node *bar;
    int  size;
} Deq;

typedef struct {
	Node *node;
} Iterator;

/* Iterators */
Iterator            first(Deq *d);
Iterator            last(Deq *d);
Iterator	        next(Iterator *i);
Iterator	        prev(Iterator *i);
char			    *fetch(const Iterator *i);
void		        store(const Iterator *i, const char *elem);

/* Create and destroy */
void		        deq_create(Deq **d);
void		        deq_destroy(Deq **d);

/* Insert and delete */
void                deq_push(Deq *d, char *value);
void                deq_shift(Deq *d, char *value);
void                deq_pop(Deq *d);
void                deq_unshift(Deq *d);

char                *deq_tail(Deq *d);
char                *deq_head(Deq *d);

/* Additional */
int                 deq_is_empty(Deq *d);
size_t		        deq_len(Deq *d);
void		        deq_print(Deq *d);

#endif
