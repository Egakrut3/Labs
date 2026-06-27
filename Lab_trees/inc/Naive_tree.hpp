#ifndef _NAIVE_TREE_HPP
#define _NAIVE_TREE_HPP

#include "Common.hpp"

struct Naive_tree_node {
	struct Naive_tree_node	*left,
				*right;

	int val;
};

struct Naive_tree {
	struct Naive_tree_node *root;

	size_t size;
};

int Naive_tree_ctor(struct Naive_tree *dst);

int Naive_tree_insert(struct Naive_tree *obj, int val);
int Naive_tree_erase(struct Naive_tree *obj, int val);

int Naive_tree_dtor(struct Naive_tree *obj);

#endif
