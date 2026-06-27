#ifndef _AVL_TREE_HPP
#define _AVL_TREE_HPP

#include "Common.hpp"

struct AVL_tree_node {
	struct AVL_tree_node	*left,
				*right;
	size_t			height;

	int val;
};

struct AVL_tree {
	struct AVL_tree_node *root;
};

int AVL_tree_ctor(struct AVL_tree *dst);

int AVL_tree_insert(struct AVL_tree *obj, int val);
int AVL_tree_erase(struct AVL_tree *obj, int val);

int AVL_tree_dtor(struct AVL_tree *obj);

#endif
