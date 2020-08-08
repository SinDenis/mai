#include "sort.h"

static int compare(data_type *l, data_type *r)
{
	if (l->key == r->key) {
		return strcmp(l->value, r->value); 
	}
	return l->key - r->key;
}

static void stack_move(stack *old, stack *rev)
{
	data_type *data;

	while (!stack_is_empty(old)) {
		data = stack_top(old);
		stack_push(rev, *data);
		stack_pop(old);
	}
}

static void shift(stack *dst, stack *src)
{
	data_type *tmp;

	tmp = stack_top(src);
	stack_push(dst, *tmp);
	stack_pop(src);
}

void old_sort(stack *s)
{
	size_t len;
	stack rev_stack;
	stack tmp_stack;
	data_type data;

	stack_create(&rev_stack);
	stack_create(&tmp_stack);
	stack_move(s, &rev_stack);
	len = rev_stack.size;

	for (size_t i = 0; i < len; ++i) {
		data_type_copy(&data, stack_top(&rev_stack));
		stack_pop(&rev_stack);
		while (!stack_is_empty(s) && compare(&data, stack_top(s)) < 0)
			shift(&tmp_stack, s);
		stack_push(s, data);
		while (!stack_is_empty(&tmp_stack))
			shift(s, &tmp_stack);
	}
	stack_destroy(&rev_stack);
	stack_destroy(&tmp_stack);
}

void insertion_sort(stack *s)
{
	data_type data;
	stack tmp_stack;

	stack_create(&tmp_stack);
	while (!stack_is_empty(s)) {
		if (stack_is_empty(&tmp_stack) || compare(stack_top(&tmp_stack), stack_top(s)) <= 0) {
			shift(&tmp_stack, s);
		} else {
			data_type_copy(&data, stack_top(s));
			stack_pop(s);
			while (!stack_is_empty(&tmp_stack) && compare(stack_top(&tmp_stack), &data) >= 0)
				shift(s, &tmp_stack);
			stack_push(s, data);
			while (!stack_is_empty(&tmp_stack))
				shift(s, &tmp_stack);
		}
	}
	stack_move(&tmp_stack, s);
	stack_destroy(&tmp_stack);
}

static void sort(stack *s, stack *tmp_stack)
{
	data_type data;

	if (stack_is_empty(s))
		return ;
	if (stack_is_empty(tmp_stack) || compare(stack_top(tmp_stack), stack_top(s)) <= 0) {
		shift(tmp_stack, s);
		sort(s, tmp_stack);
	} else {
		data_type_copy(&data, stack_top(s));
		stack_pop(s);
		while (!stack_is_empty(tmp_stack) && compare(stack_top(tmp_stack), &data) >= 0)
			shift(s, tmp_stack);
		stack_push(s, data);
		while (!stack_is_empty(tmp_stack))
			shift(s, tmp_stack);
		sort(s, tmp_stack);
	}
}

void rec_insertion_sort(stack *s)
{
	stack tmp_stack;

	stack_create(&tmp_stack);
	sort(s, &tmp_stack);
	stack_move(&tmp_stack, s);
	stack_destroy(&tmp_stack);
}
