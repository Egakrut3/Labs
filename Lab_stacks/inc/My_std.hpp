#ifndef _MY_STD_HPP
#define _MY_STD_HPP

#include <stddef.h>

int my_malloc(void **dst, size_t size);
int my_calloc(void **dst, size_t n, size_t size);
int my_reallocarray(void **dst, void *buffer, size_t n, size_t size);

#endif
