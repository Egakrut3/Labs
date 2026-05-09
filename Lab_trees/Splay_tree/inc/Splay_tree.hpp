#ifndef SPLAY_TREE
#define SPLAY_TREE

#include "Common.hpp"

typedef int	tree_arg_val_t;
typedef int	tree_mem_val_t;

#define TREE_VAL_LESS(a, b)	((a) < (b))
#define TREE_VAL_EQUAL(a, b)	((a) == (b))
#define TREE_VAL_COPY(dest, src)	\
do {					\
	(dest) = (src);			\
} while (false)
#define TREE_VAL_FREE(val)	\
do {				\
	(void)(val);		\
} while (false)



struct Splay_tree_node {
	struct Splay_tree_node	*left_ch,
				*right_ch,
				*parent;

	tree_mem_val_t		val;
};

struct Splay_tree {
	struct Splay_tree_node	*root;

	size_t			size;
};



int Splay_tree_ctor(struct Splay_tree *tree);
int Splay_tree_dtor(struct Splay_tree *tree);

int Splay_tree_insert(struct Splay_tree *tree, tree_arg_val_t val);
int Splay_tree_erase(struct Splay_tree *tree, tree_arg_val_t val);

#endif
