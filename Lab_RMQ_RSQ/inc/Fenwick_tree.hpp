#ifndef _FENWICK_HPP
#define _FENWICK_HPP

#include "Common.hpp"

struct Fenwick_tree {
	size_t	size;
	long	*buffer;
};

int Fenwick_tree_ctor(struct Fenwick_tree *dst, size_t size);

int Fenwick_tree_increase(struct Fenwick_tree *obj, size_t ind, int val);
int Fenwick_tree_get_pref(struct Fenwick_tree const *__restrict obj, size_t r, long *__restrict dst);

int Fenwick_tree_dtor(struct Fenwick_tree *obj);



int Fenwick_tree_get(struct Fenwick_tree const *__restrict obj, size_t l, size_t r, long *__restrict dst);

#endif
