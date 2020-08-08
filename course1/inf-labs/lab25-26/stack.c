#include "stack.h"

/* Functions for work width struct data_type */
void data_type_create(data_type *dt, key_type key, value_type value)
{
	dt->key = key;
	dt->value = strdup(value);
}

void data_type_copy(data_type *dst, data_type *src)
{
	dst->key = src->key;
	dst->value = strdup(src->value);
}

void data_type_destroy(data_type *data)
{
	free(data->value);
	data->value = NULL;
}

void data_type_print(data_type *data)
{
	printf("%3d: %s\n", data->key, data->value);
}


/* Functions for work width stack */
void stack_create(stack *s)
{
	s->data = (data_type *)malloc(sizeof(data_type) * INIT_SIZE);
	if (s->data == NULL) {
		exit(1);
	}
	s->size = 0;
	s->capacity = INIT_SIZE;
}

static void stack_resize(stack *s, int new_size)
{
	data_type *new_data;

	new_data = (data_type *)malloc(sizeof(data_type) * new_size);
	if (new_data == NULL)
		exit(1);
	for (size_t i = 0; i < s->size; ++i) {
		data_type_copy(&new_data[i], &s->data[i]);
	}
	for (size_t i = 0; i < s->size; ++i) {
		data_type_destroy(&s->data[i]);
	}
	free(s->data);
	s->data = new_data;
	s->capacity = new_size;
}

bool stack_is_empty(const stack *s)
{
	return s->size == 0;
}

void stack_add(stack *s, key_type key, value_type value)
{
	if (s->size == s->capacity)
		stack_resize(s, s->size * 2);
	s->data[s->size].key = key;
	s->data[s->size].value = strdup(value);
	s->size++;
}

void stack_push(stack *s, data_type data)
{
	if (s->size == s->capacity) {
		stack_resize(s, s->size * 2);
	}
	s->data[s->size].key = data.key;
	s->data[s->size].value = strdup(data.value);
	s->size++;
}

void stack_pop(stack *s)
{
	if (stack_is_empty(s)) {
		printf("Stack empty\n");
		exit(1);
	}
	data_type_destroy(&s->data[s->size - 1]);
	s->size--;
}

data_type *stack_top(stack *s)
{
	if (stack_is_empty(s))
		return NULL;
	return &s->data[s->size - 1];
}

void stack_destroy(stack *s)
{
	for (size_t i = 0; i < s->size; ++i) {
		data_type_destroy(&s->data[i]);
	}
	free(s->data);
	s->data = NULL;
	s->size = 0;
	s->capacity = 0;
}

size_t stack_length(stack *s)
{
	return s->size;
}

void stack_print(stack *s)
{
	if (s->size == 0)
		return ;
	for (int i = s->size - 1; i >= 0; --i) {
		data_type_print(&s->data[i]);
	}
}
