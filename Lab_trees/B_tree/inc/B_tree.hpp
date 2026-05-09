#ifndef B_TREE
#define B_TREE

#include "Common.hpp"

typedef int	tree_arg_val_t;
typedef int	tree_mem_val_t;

#define B_TREE_MIN_DEGREE	64
#define B_TREE_MAX_KEYS		(2 * B_TREE_MIN_DEGREE - 1)
#define B_TREE_MAX_CHILDREN	(2 * B_TREE_MIN_DEGREE)

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



struct B_tree_node {
	struct B_tree_node	*children[B_TREE_MAX_CHILDREN];
	tree_mem_val_t		keys[B_TREE_MAX_KEYS];

	size_t			size;
	bool			leaf;
};

struct B_tree {
	struct B_tree_node	*root;

	size_t			size;
};



int B_tree_ctor(struct B_tree *tree);
int B_tree_dtor(struct B_tree *tree);

int B_tree_insert(struct B_tree *tree, tree_arg_val_t val);
int B_tree_erase(struct B_tree *tree, tree_arg_val_t val);

#endif
