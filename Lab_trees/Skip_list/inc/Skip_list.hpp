#ifndef SKIP_LIST
#define SKIP_LIST

#include "Common.hpp"

typedef int	tree_arg_val_t;
typedef int	tree_mem_val_t;

#define SKIP_LIST_MAX_LEVEL	32

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



struct Skip_list_node {
	struct Skip_list_node	**next;

	size_t			level;
	tree_mem_val_t		val;
};

struct Skip_list {
	struct Skip_list_node	*header;

	size_t			level;
	size_t			size;
	uint32_t		rand_state;
};



int Skip_list_ctor(struct Skip_list *list);
int Skip_list_dtor(struct Skip_list *list);

int Skip_list_insert(struct Skip_list *list, tree_arg_val_t val);
int Skip_list_erase(struct Skip_list *list, tree_arg_val_t val);

#endif
