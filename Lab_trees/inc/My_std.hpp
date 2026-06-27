#ifndef _MY_STD_HPP
#define _MY_STD_HPP

#include <stddef.h>

int my_malloc(void *__restrict *__restrict dst, size_t size);
int my_calloc(void *__restrict *__restrict dst, size_t n, size_t size);
int my_reallocarray(void *__restrict *__restrict dst, void *__restrict buffer, size_t n, size_t size);

#endif
