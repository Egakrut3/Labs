#ifndef _SEGMENT_TREE
#define _SEGMENT_TREE

#include "Common.hpp"

struct Segment_tree {
	size_t	pow2;
	long	*buffer;
};

int Segment_tree_ctor(struct Segment_tree *dst, size_t size);

int Segment_tree_set(struct Segment_tree *obj, size_t ind, int val);
int Segment_tree_get(struct Segment_tree const *__restrict obj, size_t l, size_t r, long *__restrict dst);

int Segment_tree_dtor(struct Segment_tree *obj);

#endif
