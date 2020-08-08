#include "deq.h"

/* Functions for Iterators */
Iterator first(Deq *d)
{
    Iterator res = { d->bar->next };
    return res;
}

Iterator last(Deq *d)
{
    Iterator res = { d->bar->prev };
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

char *fetch(const Iterator *i)
{
    return i->node->data;
}

void store(const Iterator *i, const char *elem)
{
    i->node->data = strdup(elem);
}

static Node *node_create(void)
{
    Node *new_node;

    new_node = (Node *)malloc(sizeof(Node));
    new_node->data = NULL;
    new_node->prev = NULL;
    new_node->next = NULL;
    return new_node;
}

Node *node_create_with_str(char *str)
{
    Node *new_node;

    new_node = (Node *)malloc(sizeof(Node));
    new_node->data = strdup(str);
    new_node->prev = NULL;
    new_node->next = NULL;
    return new_node;
}

void node_destroy(Node *node)
{
    free(node->data);
    node->data = NULL;
    node->prev = NULL;
    node->next = NULL;
    free(node);
    node = NULL;
}

/* Fuctions for Deq */
void deq_create(Deq **d)
{
    *d = (Deq *)malloc(sizeof(Deq));
    (*d)->bar = node_create();
	(*d)->size = 0;
}

void deq_destroy(Deq **d)
{
    size_t i = 0;
    while (i < deq_len(*d)) {
        deq_pop(*d);
    }
    node_destroy((*d)->bar);
    free(*d);
    *d = NULL;
}

void deq_push(Deq *d, char *value)
{
    Node *new_node = node_create_with_str(value);

    if (deq_is_empty(d)) {
        d->bar->next = new_node;
        d->bar->prev = new_node;
        new_node->next = d->bar;
        new_node->prev = d->bar;
    } else {
        new_node->prev = d->bar->prev;
        new_node->next = d->bar;
        new_node->prev->next = new_node;
        d->bar->prev = new_node;
    }
    d->size++;
}

void deq_shift(Deq *d, char *value)
{
    Node *new_node = node_create_with_str(value);

    if (deq_is_empty(d)) {
        d->bar->next = new_node;
        d->bar->prev = new_node;
        new_node->next = d->bar;
        new_node->prev = d->bar;
    } else {
        new_node->next = d->bar->next;
        new_node->prev = d->bar;
        new_node->next->prev = new_node;
        d->bar->next = new_node;
    }
    d->size++;
}

void deq_pop(Deq *d)
{
    if (deq_is_empty(d))
        exit(1);
    Node *del_node = d->bar->prev;

    d->bar->prev = del_node->prev;
    d->bar->prev->next = d->bar;
    node_destroy(del_node);
    d->size--;
}

void deq_unshift(Deq *d)
{
    if (deq_is_empty(d))
        exit(1);
    Node *del_node = d->bar->next;
    
    d->bar->next = del_node->next;
    d->bar->next->prev = d->bar;
    node_destroy(del_node);
    d->size--;
}


size_t deq_len(Deq *d)
{
    return (d->size);
}

int deq_is_empty(Deq *d)
{
    return (deq_len(d) == 0);
}

void deq_print(Deq *d)
{
    Iterator i = first(d);

    while (i.node->data) {
        if (i.node->next->data == NULL)
            printf("%s\n", fetch(&i));
        else
            printf("%s <=> ", fetch(&i));
        next(&i);
    }
}

char *deq_tail(Deq *d)
{
    return d->bar->prev->data;
}

char *deq_head(Deq *d)
{
    return d->bar->next->data;
}
