#include <stdlib.h>
#define generate_vector(TYPENAME)								\
												\
typedef struct TYPENAME##_vec									\
{												\
	int capacity;										\
	TYPENAME* elements;									\
	int size;										\
} TYPENAME##_vec;										\
												\
TYPENAME##_vec build_##TYPENAME##_vec()								\
{												\
	TYPENAME##_vec vec;									\
	vec.capacity = 16;									\
	vec.elements = malloc(sizeof(TYPENAME) * vec.capacity);					\
	vec.size = 0;										\
	return vec;										\
}												\
												\
void append_##TYPENAME##_vec(TYPENAME##_vec* vec, TYPENAME element)				\
{												\
	if (vec->size == vec->capacity)								\
	{											\
		vec->capacity *= 2;								\
		vec->elements = realloc(vec->elements, sizeof(TYPENAME) * vec->capacity);	\
	}											\
												\
	vec->elements[vec->size] = element;							\
	++(vec->size);										\
}												\
												\
void require_semicolon()										
