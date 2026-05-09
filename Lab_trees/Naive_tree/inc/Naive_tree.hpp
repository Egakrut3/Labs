#ifndef NAIVE_TREE
#define NAIVE_TREE

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



struct Naive_tree_node {
	struct Naive_tree_node	*left_ch,
				*right_ch;

	tree_mem_val_t		val;
};

struct Naive_tree {
	struct Naive_tree_node	*root;

	size_t			size;
};



int Naive_tree_ctor(struct Naive_tree *tree);
int Naive_tree_dtor(struct Naive_tree *tree);

int Naive_tree_insert(struct Naive_tree *tree, tree_arg_val_t val);
int Naive_tree_erase(struct Naive_tree *tree, tree_arg_val_t val);

#endif
