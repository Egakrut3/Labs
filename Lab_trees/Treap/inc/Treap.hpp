#ifndef TREAP
#define TREAP

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



struct Treap_node {
	struct Treap_node	*left_ch,
				*right_ch;

	uint32_t		priority;
	tree_mem_val_t		val;
};

struct Treap {
	struct Treap_node	*root;

	size_t			size;
};



int Treap_ctor(struct Treap *tree);
int Treap_dtor(struct Treap *tree);

int Treap_insert(struct Treap *tree, tree_arg_val_t val);
int Treap_erase(struct Treap *tree, tree_arg_val_t val);

#endif
