#include "sparse_matrix.h"

double machine_eps(void)
{
	double eps;

	eps = 1;
	while (1 + eps > 1)
		eps /= 2;
    return eps;
}

/* Split string cross delimetr */
vector strsplit(char *str, char delim)
{
     char buf[1024];
     vector ans;
     int j;

     j = 0;
     ans = vec_create(10);
     for (size_t i = 0; i < strlen(str); ++i) {
         if (str[i] == delim) {
             if (j != 0) {
                 buf[i] = '\0';
                 vec_push_back(&ans, STRING, buf);
                 bzero(buf, 1024);
                 j = 0;
             }
         } else {
             buf[j] = str[i];
             j++;
         }
	}
	vec_push_back(&ans, STRING, buf);
	return ans;
}

void vec_str_to_double(vector *v)
{
	double num;

	for (int i = 0; i < vec_length(v); ++i) {
		num = atof((char *)v->data[i].value);
		vec_set_value(v, i, DOUBLE, num);
	}
}
