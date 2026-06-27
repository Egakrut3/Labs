#ifndef _TREAP_TREE_HPP
#define _TREAP_TREE_HPP

#include "Common.hpp"

struct Treap_tree_node {
	struct Treap_tree_node	*left,
				*right;
	long			prior;

	int val;
};

struct Treap_tree {
	struct Treap_tree_node *root;
};

int Treap_tree_ctor(struct Treap_tree *dst);

int Treap_tree_insert(struct Treap_tree *obj, int val);
int Treap_tree_erase(struct Treap_tree *obj, int val);

int Treap_tree_dtor(struct Treap_tree *obj);

#endif
