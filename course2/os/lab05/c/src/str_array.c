#include "str_array.h"

str_array_t* str_array_create(size_t size) {
	str_array_t* array = (str_array_t*)malloc(sizeof(str_array_t));
	array->data_ = (char **)malloc(sizeof(char*) * size);
	array->size_ = size;
	for (size_t i = 0; i < size; ++i) {
		array->data_[i] = NULL;
	}
	return array;
}

void str_array_destroy(str_array_t **arr) {
	for (size_t i = 0; i < (*arr)->size_; ++i) {
		free((*arr)->data_[i]);
		(*arr)->data_[i] = NULL;
	}
	free((*arr)->data_);
	(*arr)->size_ = 0;
	(*arr)->data_ = NULL;
	free(*arr);
	arr = NULL;
}

char* str_array_get_item(str_array_t* arr, size_t idx) {
	return arr->data_[idx];
}

void str_array_set_item(str_array_t* arr, size_t idx, const char* str) {
	free(arr->data_[idx]);
	arr->data_[idx] = strdup(str);
}

void str_array_print(str_array_t* arr) {
	for (size_t i = 0; i < arr->size_; ++i) {
		printf("%2zu: %s\n", i, arr->data_[i]);
	}
}
