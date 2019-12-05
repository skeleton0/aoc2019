#include <stdlib.h>
#define generate_vector(TYPENAME)								\
												\
typedef struct TYPENAME##_vec									\
{												\
	int capacity;										\
	TYPENAME* data;										\
	int size;										\
} TYPENAME##_vec;										\
												\
TYPENAME##_vec build_##TYPENAME##_vec()								\
{												\
	TYPENAME##_vec vec;									\
	vec.capacity = 16;									\
	vec.data = malloc(sizeof(TYPENAME) * vec.capacity);					\
	vec.size = 0;										\
	return vec;										\
}												\
												\
void append_##TYPENAME##_vec(TYPENAME##_vec* vec, TYPENAME element)				\
{												\
	if (vec->size == vec->capacity)								\
	{											\
		vec->capacity *= 2;								\
		vec->data = realloc(vec->data, sizeof(TYPENAME) * vec->capacity);		\
	}											\
												\
	vec->data[vec->size] = element;								\
	++(vec->size);										\
}												\
												\
void require_semicolon()										
