#include "list.h"

/* Functions for Iterators */
Iterator first(List *lst)
{
    Iterator res = { lst->head };
    return res;
}

Iterator last(List *lst)
{
    Iterator res = { lst->tail };
    return res;
}

Iterator prev(Iterator *i)
{
    i->node = i->node->prev;
    return *i;
}

Iterator next(Iterator *i)
{
    i->node = i->node->next;
    return *i;
}

data_type fetch(const Iterator *i)
{
    return i->node->data;
}

void store(const Iterator *i, const int elem)
{
    i->node->data = elem;
}


/* Fuctions for list */
void list_create(List *lst)
{
    lst->head = NULL;
    lst->tail = NULL;
	lst->size = 0;
}

void list_insert(List *lst, Iterator *i, data_type value)
{
    if (lst->head == i->node && i->node == NULL) {
        list_push_back(lst, value);
        return ;
    }
    if (i->node == NULL)
        exit(1);
	Iterator res = { (Node *)malloc(sizeof(Node)) };
    if (res.node == NULL)
        exit(1);
    res.node->next = NULL;
    res.node->prev = NULL;
    store(&res, value);
    res.node->prev = i->node->prev;
    res.node->next = i->node;
    if (res.node->prev != NULL)
        res.node->prev->next = res.node;
    i->node->prev = res.node;
    if (i->node == lst->head)
        lst->head = res.node;
    lst->size++;
}

void list_push_back(List *lst, data_type value)
{
    Iterator res = { (Node *)malloc(sizeof(Node)) };
    if (res.node == NULL)
        exit(1);
    res.node->next = NULL;
    res.node->prev = NULL;
    store(&res, value);
    if (lst->size == 0) {
        lst->head = res.node;
        lst->tail = res.node;
    } else {
        lst->tail->next = res.node;
        res.node->prev = lst->tail;
        res.node->next = NULL;
        lst->tail =  res.node;
    }
    lst->size++;
}

void list_delite(List *lst, Iterator *i)
{
    if (i->node == NULL)
        exit(1);
    if (lst->head == i->node)
        lst->head = lst->head->next;
    else if (lst->tail == i->node)
        lst->tail = lst->tail->prev;
    if (i->node->next != NULL)
        i->node->next->prev = i->node->prev;
    if (i->node->prev != NULL)
        i->node->prev->next = i->node->next;
    free(i->node);
    i->node = NULL;
    lst->size--;
}

Iterator search(List *lst, int n)
{
    Iterator i;

    i = first(lst);
    for (int j = 0; j < n && i.node != NULL; ++j) {
        next(&i);
    }
    return i;
}

void list_del(List *lst, int idx)
{
    Iterator i;

    i = search(lst, idx);
    list_delite(lst, &i);
}

void list_push(List *lst, int idx, int elem)
{
    Iterator i;

    i = search(lst, idx);
    if (lst->size != 0 && idx == (int)lst->size) {
        list_push_back(lst, elem);
    }

    list_insert(lst, &i, elem);
}

size_t list_len(List *lst)
{
    return lst->size;
}

bool list_is_empty(List *lst)
{
    return list_len(lst) == 0;
}

void list_print(List *lst)
{
    Iterator i = first(lst);

    while (i.node != NULL) {
        printf(i.node != lst->tail ? "%d <=> " : "%d\n", fetch(&i));
        next(&i);
    }
}

void list_destroy(List *lst)
{
    Iterator i = first(lst);
    Iterator i_del;

    while (i.node) {
        i_del = i;
        next(&i);
        list_delite(lst, &i_del);
    }
}

void list_del_more_less_value(List *lst, data_type value)
{
    Iterator i = first(lst);
    Iterator i_del;

    while (i.node) {
        i_del = i;
        next(&i);
        if (fetch(&i_del) != value)
            list_delite(lst, &i_del);
    }
}
