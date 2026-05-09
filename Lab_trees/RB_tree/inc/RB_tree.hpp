#ifndef RB_TREE
#define RB_TREE

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

enum RB_tree_color : byte_t {
	RB_BLACK,
	RB_RED,
};

struct RB_tree_node {
	struct RB_tree_node	*left_ch,
				*right_ch;

	enum RB_tree_color	color;
	tree_mem_val_t		val;
};

struct RB_tree {
	struct RB_tree_node	*root;

	size_t			size;
};



int RB_tree_ctor(struct RB_tree *tree);
int RB_tree_dtor(struct RB_tree *tree);

int RB_tree_insert(struct RB_tree *tree, tree_arg_val_t val);
int RB_tree_erase(struct RB_tree *tree, tree_arg_val_t val);

#endif
