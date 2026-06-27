#ifndef _MY_STD_HPP
#define _MY_STD_HPP

#include <stddef.h>

int my_malloc(void *restrict *restrict dst, size_t size);
int my_calloc(void *restrict *restrict dst, size_t n, size_t size);
int my_reallocarray(void *restrict *restrict dst, void *restrict buffer, size_t n, size_t size);

#endif
