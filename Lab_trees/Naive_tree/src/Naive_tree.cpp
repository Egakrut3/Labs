#include "Naive_tree.hpp"

typedef struct Naive_tree_node	node_t;
typedef struct Naive_tree	tree_t;

#define FINAL_CODE

int Naive_tree_ctor(tree_t *const tree) {
	assert(tree);

	tree->root	= nullptr;
	tree->size	= 0;

	CLEAR_RESOURCES();
	return 0;
}

#undef FINAL_CODE

int Naive_tree_dtor(tree_t *const tree) {
	#define FINAL_CODE

	assert(tree);

	struct Call_state {
		node_t	**node;

		byte_t	state;
	} *call_stk = nullptr;
	ALLOC_ARR(call_stk, tree->size + 1);
	#undef FINAL_CODE
	#define FINAL_CODE			\
	FREE_ARR(call_stk, tree->size + 1);

	size_t last_ind = 0;
	call_stk[last_ind].node		= &tree->root;
	call_stk[last_ind].state	= 0;
	last_ind++;

	while (last_ind) {
		node_t **const node = call_stk[last_ind - 1].node;
		assert(node);

		if (!*node) { last_ind--; continue; }

		switch (call_stk[last_ind - 1].state) {
			case 0:
				call_stk[last_ind - 1].state	= 1;

				call_stk[last_ind].node		= &(*node)->left_ch;
				call_stk[last_ind].state	= 0;
				last_ind++;
				break;

			case 1:
				call_stk[last_ind - 1].state	= 2;

				call_stk[last_ind].node		= &(*node)->right_ch;
				call_stk[last_ind].state	= 0;
				last_ind++;
				break;

			case 2:
				TREE_VAL_FREE((*node)->val);
				FREE_ELEM(*node);
				last_ind--;
				break;

			default:
				abort();
		}
	}

	tree->size = 0;

	CLEAR_RESOURCES();
	return 0;

	#undef FINAL_CODE
}

#define FINAL_CODE

int Naive_tree_insert(tree_t *const tree, tree_arg_val_t const val) {
	assert(tree);

	node_t **node = &tree->root;
	while (*node) {
		if (TREE_VAL_LESS(val, (*node)->val)) {
			node = &(*node)->left_ch;
			continue;
		}
		else if (!TREE_VAL_EQUAL(val, (*node)->val)) {
			node = &(*node)->right_ch;
			continue;
		}

		CLEAR_RESOURCES();
		return 0;
	}

	ALLOC_ELEM(*node);
	(*node)->left_ch	= nullptr;
	(*node)->right_ch	= nullptr;
	TREE_VAL_COPY((*node)->val, val);
	tree->size++;

	CLEAR_RESOURCES();
	return 0;
}

int Naive_tree_erase(tree_t *const tree, tree_arg_val_t const val) {
	assert(tree);

	node_t **node = &tree->root;
	while (*node) {
		if (TREE_VAL_LESS(val, (*node)->val)) {
			node = &(*node)->left_ch;
			continue;
		}
		else if (!TREE_VAL_EQUAL(val, (*node)->val)) {
			node = &(*node)->right_ch;
			continue;
		}

		tree->size--;
		node_t	*const left_ch	= (*node)->left_ch,
			*const right_ch	= (*node)->right_ch;

		if (!left_ch) {
			TREE_VAL_FREE((*node)->val);
			FREE_ELEM(*node);
			*node = right_ch;
			CLEAR_RESOURCES();
			return 0;
		}
		if (!right_ch) {
			TREE_VAL_FREE((*node)->val);
			FREE_ELEM(*node);
			*node = left_ch;
			CLEAR_RESOURCES();
			return 0;
		}

		node_t **swap_node = &(*node)->left_ch;
		while ((*swap_node)->right_ch) { swap_node = &(*swap_node)->right_ch; }

		node_t *const swap_node_left_ch = (*swap_node)->left_ch;
		TREE_VAL_FREE((*node)->val);
		TREE_VAL_COPY((*node)->val, (*swap_node)->val);
		FREE_ELEM(*swap_node);
		*swap_node = swap_node_left_ch;

		CLEAR_RESOURCES();
		return 0;
	}

	CLEAR_RESOURCES();
	return 0;
}

#undef FINAL_CODE
