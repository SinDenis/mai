#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include "src/str_array.h"

extern str_array_t* str_array_create(size_t size);
extern void str_array_destroy(str_array_t **arr);
extern char* str_array_get_item(str_array_t* arr, size_t idx);
extern void str_array_set_item(str_array_t* arr, size_t idx, const char* str);
extern void str_array_print(str_array_t* arr);

int main(void)
{
	str_array_t* arr = str_array_create(10);
	str_array_set_item(arr, 0, "e23e2efffaaabb232323ab");
	str_array_set_item(arr, 1, "e23e9879877fefef879793ab");
	str_array_set_item(arr, 2, "e2988389798ef8ef98e7ab7aba9b");
	str_array_set_item(arr, 3, "efef56ef5e6f5e8232738abccc32");
	str_array_set_item(arr, 4, "e9898987797fef7ef7e9fe7ababab32");
	str_array_set_item(arr, 5, "67667766fefefeabca787878b7b7b7");
	str_array_set_item(arr, 6, "e7ba6767e76e767e6e7e7e67c67c6c799");
	str_array_set_item(arr, 7, "e42352454baceceff7837273788b");
	str_array_set_item(arr, 8, "e36636efef77abc7bc77778283828");
	str_array_set_item(arr, 9, "e2832938ebfbef838283ae23888efe323");
	str_array_print(arr);
	str_array_destroy(&arr);
	return 0;
}