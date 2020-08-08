#ifndef STR_ARRAY_H
#define STR_ARRAY_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct str_array {
	char** data_;
	size_t size_;
} str_array_t;

#endif
