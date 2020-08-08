#include "opn.h"

bool isoperation(char c)
{
    return c == '-' || c == '+' || c == '*' || c == '/' || c == '^';
}

static size_t len_num(long long num)
{
	size_t		len;

	len = 0;
	if (num == 0)
		return (1);
	while (num > 0)
	{
		num /= 10;
		len++;
	}
	return (len);
}

static void convert(char **str, long long num, size_t index)
{
	if (num == 0)
		(*str)[index] = num % 10 + '0';
	while (num > 0)
	{
		(*str)[index] = num % 10 + '0';
		num /= 10;
		index--;
	}
}

char *itoa(int n)
{
	long long	num;
	char		*str;
	int			sign;
	size_t		len;

	len = 0;
	sign = 0;
	num = (long long)n;
	if (num < 0)
	{
		sign--;
		num *= -1;
		len++;
	}
	len += len_num(num);
	str = (char *)malloc(sizeof(char) * (len + 1));
	if (str == NULL)
		return (NULL);
	if (sign == -1)
		str[0] = '-';
	str[len] = 0;
	convert(&str, num, len - 1);
	return (str);
}

int power(int base, int p)
{
    if (p == 0)
        return 1;
    if (p % 2 != 0)
        return base * power(base, p - 1);
    return power(base * base, p / 2);
}

int priority(char *s)
{
    if (*s == '^')
        return 3;
    if (*s == '*' || *s == '/')
        return 2;
    if (*s == '+' || *s == '-')
        return 1;
    if (*s == '(' || *s == ')')
        return 0;
    return 100;
}

void strdel(char *str)
{
    free(str);
    str = NULL;
}

State type(char *str)
{
    if (str ==  NULL)
        exit(1);
    if (isdigit(str[0]))
        return NUMBER;
    if (isalpha(str[0]))
        return LETTER;
    if (isoperation(str[0]))
        return OPERATION;
    if (str[0] == ')' || str[0] == '(')
        return BREACKET;
    return SPACE;
}

char *str_cut(char *str, int start, int end)
{
    int i = 0;
    char *new_str = (char *)malloc(sizeof(char) * (end - start + 1));
    if (new_str == NULL)
        exit(1);

    while (start < end)
        new_str[i++] = str[start++];
    new_str[i] = 0;
    return new_str;
}

void add_op(Deq *res, Deq *tmp, char *op)
{
    while (!deq_is_empty(tmp) && strcmp(op, "^") != 0 && priority(op) <= priority(deq_tail(tmp))) {
        deq_push(res, deq_tail(tmp));
        deq_pop(tmp);
    }
    deq_push(tmp, op);
    strdel(op);
}

void add_expr(Deq *res, char *str)
{
    deq_push(res, str);
    strdel(str);
}

void add_open_bracket(Deq *tmp)
{
    deq_push(tmp, "(");
}

void add_close_bracket(Deq *res, Deq *tmp)
{
    while (strcmp(deq_tail(tmp), "(") != 0) {
        deq_push(res, deq_tail(tmp));
        deq_pop(tmp);
    }
    deq_pop(tmp);
}

Deq *opn(char *str)
{
    Deq     *res;
    Deq     *tmp;
    int     start = 0,
            end = 0;
    State   s = SPACE;

    deq_create(&res);
    deq_create(&tmp);
    for (int i = 0; i <= (int)strlen(str); ++i) {
        switch (s)
        {
            case SPACE:
                if (isdigit(str[i])) {
                    s = NUMBER;
                    start = end = i;
                } else if (isalpha(str[i])) {
                    s = LETTER;
                    start = end = i;
                } else if (isoperation(str[i])) {
                    s = OPERATION;
                    add_op(res, tmp, str_cut(str, i, i + 1));
                } else if (str[i] == '(') {
                    s = BREACKET;
                    add_open_bracket(tmp);
                } else if (str[i] == ')') {
                    s = BREACKET;
                    add_close_bracket(res, tmp);
                }
            break;

            case NUMBER:
                if (isdigit(str[i])) {
                    end++;
                } else {
                    add_expr(res, str_cut(str, start, end + 1));
                    if (isoperation(str[i])) {
                        s = OPERATION;
                        add_op(res, tmp, str_cut(str, i, i + 1));
                    } else if (isspace(str[i]))
                        s = SPACE;
                    else if (str[i] == ')') {
                        s = BREACKET;
                        add_close_bracket(res, tmp);
                    }
                }
            break;

            case LETTER:
                if (isdigit(str[i]) || isalpha(str[i])) {
                    end++;
                } else {
                    add_expr(res, str_cut(str, start, end + 1));
                    if (isoperation(str[i])) {
                        s = OPERATION;
                        add_op(res, tmp, str_cut(str, i, i + 1));
                    } else if (isspace(str[i]))
                        s = SPACE;
                    else if (str[i] == ')') {
                        s = BREACKET;
                        add_close_bracket(res, tmp);
                    }
                }
            break;
            case OPERATION:
                if (isdigit(str[i])) {
                    s = NUMBER;
                    start = end = i;
                } else if (isalpha(str[i])) {
                    s = LETTER;
                    start = end = i;
                } else if (isspace(str[i]))
                    s = SPACE;
                else if (str[i] == '(') {
                    s = BREACKET;
                    add_open_bracket(tmp);
                }
            break;
            case BREACKET:
                if (isdigit(str[i])) {
                    s = NUMBER;
                    start = end = i;
                } else if (isalpha(str[i])) {
                    s = LETTER;
                    start = end = i;
                } else if (str[i] == ')') {
                    add_close_bracket(res, tmp);
                } else if (isoperation(str[i])) {
                    s = OPERATION;
                    add_op(res, tmp, str_cut(str, i, i + 1));
                } else if (isspace(str[i]))
                    s = SPACE;
                else if (str[i] == '(')
                    add_open_bracket(tmp);
            break;
        }
    }
    while (!deq_is_empty(tmp)) {
        deq_push(res, deq_tail(tmp));
        deq_pop(tmp);
    }
    deq_destroy(&tmp);
    return res;
}

Tree    *get_tree_expression(Deq *pn)
{
    Tree *t;
    Tree_stack ts;

    tree_stack_create(&ts);
    while (!deq_is_empty(pn)) {
        if (type(deq_head(pn)) == OPERATION) {
            t = node_create(deq_head(pn));
            t->type = OP;
            t->right = tree_stack_top(&ts);
            tree_stack_pop(&ts);
            t->left = tree_stack_top(&ts);
            tree_stack_pop(&ts);
            deq_unshift(pn);
            tree_stack_push(&ts, t);
        } else {
            t = node_create(deq_head(pn));
            if (type(deq_head(pn)) == NUMBER)
                t->type = NUM;
            else if (type(deq_head(pn)) == LETTER)
                t->type = STR;
            tree_stack_push(&ts, t);
            deq_unshift(pn);
        }
    }
    t = tree_stack_top(&ts);
    tree_stack_destroy(&ts);
    return t;
}

bool find_in(Tree *t)
{
    if (t->left == NULL || t->right == NULL)
        return false;
    if ((strcmp(t->data, "+") == 0 || strcmp(t->data, "-") == 0) && strcmp(t->left->data, "/") == 0 &&
    strcmp(t->right->data, "/") == 0)
        return true;
    return false;
}

Tree *tree_copy(Tree *t)
{
    if (t == NULL)
        return NULL;
    Tree *new_tree;
    new_tree = node_create(t->data);
    new_tree->type = t->type;
    new_tree->left = tree_copy(t->left);
    new_tree->right = tree_copy(t->right);
    return new_tree;
}

Tree *transfom(Tree *a, Tree *b, Tree *c, Tree *d, char *s)
{
    Tree *root;

    root = node_create("/");
    root->left = node_create(s);
    root->left->left = node_create("*");
    root->left->right = node_create("*");
    root->left->left->left = tree_copy(a);
    root->left->left->right = tree_copy(d);
    root->left->right->left = tree_copy(b);
    root->left->right->right = tree_copy(c);
    root->right = node_create("*");
    root->right->left = tree_copy(b);
    root->right->right = tree_copy(d);

    root->type = OP;
    root->left->type = OP;
    root->left->left->type = OP;
    root->left->right->type = OP;
    root->left->left->left->type = a->type;
    root->left->left->right->type = d->type;
    root->left->right->left->type = b->type;
    root->left->right->right->type = c->type;
    root->right->type = OP;
    root->right->left->type = b->type;
    root->right->right->type = d->type;
    return root;
}

Tree *tree_transform(Tree *t)
{
    Tree *new_tree;
    if (t == NULL)
        return NULL;
    t->left = tree_transform(t->left);
    t->right = tree_transform(t->right);
    if (find_in(t)) {
        new_tree = transfom(t->left->left, t->left->right, t->right->left, t->right->right, t->data);
        tree_destroy(t);
        return new_tree;
    }
    return t;
}

char *calculate(char *l, char *r, char *op)
{
    int a = atoi(l);
    int b = atoi(r);

    if (strcmp(op, "+") == 0)
        return itoa(a + b);
    if (strcmp(op, "-") == 0)
        return itoa(a - b);
    if (strcmp(op, "*") == 0)
        return itoa(a * b);
    if (strcmp(op, "/") == 0)
        return itoa(a / b);
    return itoa(power(a, b));
}

Tree *tree_calc(Tree *t)
{
    if (t == NULL)
        return NULL;
    t->left = tree_calc(t->left);
    t->right = tree_calc(t->right);
    if (t->left != NULL && t->right != NULL && t->left->type == t->right->type && t->left->type == NUM) {
        char *str = calculate(t->left->data, t->right->data, t->data);
        Tree *tmp = node_create(str);
        tmp->type = NUM;
        tree_destroy(t);
        strdel(str);
        t = tmp;
    }
    return t;
}

void    tree_to_expr(Tree *t, Tree_stack *s)
{
    if (t == NULL)
        return ;
    if (t->left != NULL && t->type == OP && t->left->type == OP &&
    priority(t->data) > priority(t->left->data)) {
        tree_stack_push(s, t);
        printf("( ");
    }
    tree_to_expr(t->left, s);
    if (!tree_stack_is_empty(s) && t == tree_stack_top(s)) {
        printf(") ");
        tree_stack_pop(s);
    }
    printf("%s ", t->data);
    if (t->right != NULL && t->type == OP && t->right->type == OP) {
        if((priority(t->data) > priority(t->right->data)) || (strcmp(t->data, "/") == 0 && strcmp(t->right->data, "*") == 0)) {
            tree_stack_push(s, t);
            printf("( ");
        }
    }
    tree_to_expr(t->right, s);
    if (!tree_stack_is_empty(s) && t == tree_stack_top(s)) {
        printf(") ");
        tree_stack_pop(s);
    }
}
