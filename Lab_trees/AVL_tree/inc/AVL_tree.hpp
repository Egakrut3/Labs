#ifndef AVL_TREE
#define AVL_TREE

#include "Common.hpp"

typedef int	tree_arg_val_t;
typedef int	tree_mem_val_t;

#define TREE_VAL_LESS(a, b)	((a) < (b))
#define TREE_VAL_EQUAL(a, b)	((a) == (b))
#define TREE_VAL_COPY(dest, src)	\
do {					\
	(dest) = (src);			\
} while (false)
#define TREE_VAL_MOVE(dest, src)	\
do {					\
	(dest) = (src);			\
} while (false)
#define TREE_VAL_FREE(val)	\
do {				\
	(void)(val);		\
} while (false)



struct AVL_tree_node {
	struct AVL_tree_node	*left_ch,
				*right_ch;

	size_t			height;
	tree_mem_val_t		val;
};

struct AVL_tree {
	struct AVL_tree_node	*root;

	size_t			size;
};



int AVL_tree_ctor(struct AVL_tree *tree);
int AVL_tree_dtor(struct AVL_tree *tree);

int AVL_tree_insert(struct AVL_tree *tree, tree_arg_val_t val);
int AVL_tree_erase(struct AVL_tree *tree, tree_arg_val_t val);

#endif
