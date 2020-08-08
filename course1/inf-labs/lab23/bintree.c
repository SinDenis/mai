#include "bintree.h"

Bintree tree_create(void)
{
    Bintree tree;

    tree.root = NULL;
    tree.size = 0;
    return tree;
}

Node *node_create(data_type elem)
{
    Node *new_node;

    new_node = (Node *)malloc(sizeof(Node));
    new_node->key = elem;
    new_node->left = NULL;
    new_node->right = NULL;
    return new_node;
}

void node_destroy(Node *node)
{
    free(node);
    node = NULL;
}

bool node_is_leaf(Node *node)
{
    return node->right == NULL && node->left == NULL;
}

void    tree_insert(Bintree *tree, data_type key)
{
    Node *tmp;
    Node *leaf;
    Node *new;

    leaf = NULL;
    tmp = tree->root;
    new = node_create(key);

    while (tmp != NULL) {
        leaf = tmp;
        if (key < tmp->key)
            tmp = tmp->left;
        else
            tmp = tmp->right;
    }
    if (leaf == NULL)
        tree->root = new;
    else {
        if (key < leaf->key)
            leaf->left = new;
        else
            leaf->right = new;
    }
    tree->size++;
}

Node *search_min_node_in_subtree(Node *subtree)
{
    Node *res;

    res = subtree;
    while (res->left != NULL)
        res = res->left;
    return res;
}

Node *search_max_node_in_subtree(Node *subtree)
{
    Node *res;

    res = subtree;
    while (res->right!= NULL)
        res = res->right;
    return res;
}

Node *serch_rec_par_del(Node *node, data_type key)
{
    if (node == NULL)
        return NULL;
    if ((node->left != NULL && node->left->key == key) || (node->right != NULL && node->right->key == key))
        return node;
    if (key < node->key)
        return serch_rec_par_del(node->left, key);
    else
        return serch_rec_par_del(node->right, key);
}

Node *search_par_del(Bintree *tree, data_type key)
{
    Node *tmp;

    tmp = tree->root;
    return serch_rec_par_del(tmp, key);
}

int del_root(Bintree *tree)
{
    Node *del;
    Node *min_node;

    del = tree->root;
    if (tree->size == 1) {
        tree->root = NULL;
        node_destroy(del);
    }
    else if (del->left == NULL && del->right != NULL) {
        tree->root = del->right;
        node_destroy(del);
    } else if (del->left != NULL && del->right == NULL) {
        tree->root = del->left;
        node_destroy(del);
    } else {
        min_node = search_min_node_in_subtree(del->right);
        min_node->left = del->left;
        tree->root = del->right;
        node_destroy(del);
    }
    tree->size--;
    return (1);
}

void del_elem_with_one_subtree(Node *par, Node *del, char c)
{
    if (c == 'l') {
        if (par->left == del)
            par->left = del->left;
        else
            par->right = del->left;
    } else {
        if (par->left == del)
            par->left = del->right;
        else
            par->right = del->right;
    }
    node_destroy(del);
}

int tree_del(Bintree *tree, data_type key)
{
    Node *par;
    Node *del;
    Node *min_node;

    if (tree->size == 0)
        return -1;
    if (tree->root->key == key) {
        return del_root(tree);
    }
    par = search_par_del(tree, key);
    if (par == NULL)
        return -1;
    if (par->left != NULL && par->left->key == key)
        del = par->left;
    else
        del = par->right;
    if (node_is_leaf(del)) {
        if (par->left != NULL && par->left == del)
            par->left = NULL;
        else
            par->right = NULL;
        node_destroy(del);
    }
    else if (del->left == NULL && del->right != NULL)
        del_elem_with_one_subtree(par, del, 'r');
    else if (del->left != NULL && del->right == NULL)
        del_elem_with_one_subtree(par, del, 'l');
    else {
        min_node = search_min_node_in_subtree(del->right);
        min_node->left = del->left;
        if (par->left == del)
            par->left = del->right;
        else
            par->right = del->right;
        node_destroy(del);
    }
    tree->size--;
    return 1;
}

void rec_destroy(Node *node)
{
    if (node == NULL)
        return ;
    rec_destroy(node->left);
    rec_destroy(node->right);
    node_destroy(node);
}

void    tree_destroy(Bintree *tree)
{
    rec_destroy(tree->root);
    tree->root = NULL;
}

int max(int l, int r) {
    return l > r ? l : r;
}

int rec_height(Node *node)
{
    if (node == NULL)
        return 0;
    return max(rec_height(node->left), rec_height(node->right)) + 1;
}

int tree_height(Bintree *tree)
{
    return max(0, rec_height(tree->root) - 1);
}

void    rec_print(Node *tmp, int h)
{
    if (tmp == NULL)
        return ;
    rec_print(tmp->right, h + 10);
    printf("%*d\n", h, tmp->key);
    rec_print(tmp->left, h + 10);
}

void    tree_print(Bintree *tree)
{
    rec_print(tree->root, 0);
}

data_type tree_min(Bintree *tree)
{
    Node *node;

    node = search_min_node_in_subtree(tree->root);
    return node->key;
}

data_type tree_max(Bintree *tree)
{
    Node *node;

    node = search_max_node_in_subtree(tree->root);
    return node->key;
}
