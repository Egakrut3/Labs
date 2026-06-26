#ifndef _DYNAMIC_ARRAY_HPP
#define _DYNAMIC_ARRAY_HPP

#include "Common.hpp"

typedef int (*assign_func_t)(void *dst, void const *src);
typedef int (*free_func_t)(void *elem);

struct Dynamic_array {
	size_t		elem_size;
	assign_func_t	elem_assign;
	free_func_t	elem_free;

	void	*begin,
		*end,
		*buffer_end;
};

int Dynamic_array_ctor(struct Dynamic_array *dst, size_t elem_size, assign_func_t elem_assign, free_func_t elem_free, size_t capacity);

int Dynamic_array_push_back(struct Dynamic_array *obj, void const *val);
int Dynamic_array_pop_back(struct Dynamic_array *obj);

size_t Dynamic_array_size(struct Dynamic_array const *obj);
size_t Dynamic_array_capacity(struct Dynamic_array const *obj);

int Dynamic_array_dtor(struct Dynamic_array *obj);

#define	EPOP_FROM_EMPTY	0x100

#endif
