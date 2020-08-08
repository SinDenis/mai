#ifndef LIST_H_
#define LIST_H_

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

typedef int data_type;

typedef struct elem {
    data_type data;
    struct elem *next;
    struct elem *prev;
} Node;

typedef struct {
    Node *head;
    Node *tail;
    size_t size;
} List;

typedef struct {
	Node *node;
} Iterator;

/* Iterators */
Iterator    first(List *lst);
Iterator    last(List *lst);
Iterator	next(Iterator *i);
Iterator	prev(Iterator *i);
int			fetch(const Iterator *i);
void		store(const Iterator *i, const data_type elem);

/* List */
void		list_create(List *lst);
void	    list_insert(List *lst, Iterator *i, data_type num);
void        list_push_back(List *lst, data_type value);
void	    list_delite(List *lst, Iterator *i);
void		list_print(List *lst);
void		list_destroy(List *lst);
size_t		list_len(List *lst);
bool        list_is_empty(List *lst);
void        list_push(List *lst, int idx, int elem);
void        list_del_more_less_value(List *lst, data_type value);
void        list_del(List *lst, int idx);

#endif
