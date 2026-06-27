#ifndef _STRONG_FENWICK_TREE_HPP
#define _STRONG_FENWICK_TREE_HPP

#include "Common.hpp"

struct Strong_fenwick_tree {
	size_t	size;
	long	*buffer;

	int	*arr;
};

int Strong_fenwick_tree_ctor(struct Strong_fenwick_tree *dst, size_t size);

int Strong_fenwick_tree_set(struct Strong_fenwick_tree *obj, size_t ind, int val);
int Strong_fenwick_tree_get(struct Strong_fenwick_tree const *__restrict obj, size_t l, size_t r, long *__restrict dst);

int Strong_fenwick_tree_dtor(struct Strong_fenwick_tree *obj);

#endif
