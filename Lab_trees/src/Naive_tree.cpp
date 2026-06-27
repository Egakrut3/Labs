#include "Naive_tree.hpp"

#define FINAL_CODE

typedef struct Naive_tree_node	node_t;
typedef struct Naive_tree	tree_t;

int Naive_tree_ctor(tree_t *const dst) {
	assert(dst);

	dst->root = nullptr;

	dst->size = 0;

	LEAVE(0);
}

static int Naive_tree_node_ctor(node_t *const dst, int val) {
	assert(dst);

	dst->left = nullptr;
	dst->right = nullptr;

	dst->val = val;

	LEAVE(0);
}

int Naive_tree_insert(tree_t *const obj, int val) {
	assert(obj);

	node_t **node = &obj->root;
	while (*node) {
		if (val < (*node)->val) {
			node = &(*node)->left;
		}
		else {
			node = &(*node)->right;
		}
	}

	NEW(Naive_tree_node, *node, val);
	obj->size += 1;

	LEAVE(0);
}

static int Naive_tree_node_dtor(node_t *const obj) {
	assert(obj);

	LEAVE(0);
}

int Naive_tree_erase(tree_t *const obj, int val) {
	assert(obj);

	node_t **node = &obj->root;
	while (*node) {
		if (val < (*node)->val) {
			node = &(*node)->left;
			continue;
		}
		else if (val > (*node)->val) {
			node = &(*node)->right;
			continue;
		}

		if (!(*node)->left) {
			node_t *const save_ptr = (*node)->right;
			DELETE_CHECKED(Naive_tree_node, *node);
			obj->size -= 1;
			*node = save_ptr;
			break;
		}

		node_t *save_ptr = *node;
		node = &(*node)->left;
		while ((*node)->right) { node = &(*node)->right; }
		save_ptr->val = (*node)->val;
		save_ptr = (*node)->left;
		DELETE_CHECKED(Naive_tree_node, *node);
		obj->size -= 1;
		*node = save_ptr;
		break;
	}

	LEAVE(0);
}

int Naive_tree_dtor(tree_t *const obj) {
	assert(obj);

	enum State {
		FIRST_TIME,	// means we hadn't called with left yet
		SECOND_TIME,	// means we already called with left, but not with right yet
		THIRD_TIME,	// means we already called with both left and right
	};
	struct Call_state {
		node_t **node;

		enum State state;
	} *call_stk = nullptr;
	CALLOC_ARR(call_stk, obj->size + 1);
	#undef FINAL_CODE
	#define FINAL_CODE			\
	FREE_ARR(call_stk, obj->size + 1);

	ptrdiff_t last_ind = 0;
	call_stk[last_ind].node		= &obj->root;
	call_stk[last_ind].state	= FIRST_TIME;
	while (last_ind != -1) {
		node_t **node = call_stk[last_ind].node;
		assert(node);

		if (!*node) { last_ind -= 1; continue; }

		switch (call_stk[last_ind].state) {
			case FIRST_TIME:
				call_stk[last_ind].state = SECOND_TIME;

				last_ind += 1;
				call_stk[last_ind].node		= &(*node)->left;
				call_stk[last_ind].state	= FIRST_TIME;
				break;

			case SECOND_TIME:
				call_stk[last_ind].state = THIRD_TIME;

				last_ind += 1;
				call_stk[last_ind].node		= &(*node)->right;
				call_stk[last_ind].state	= FIRST_TIME;
				break;

			case THIRD_TIME:
				DELETE_CHECKED(Naive_tree_node, *node);
				last_ind -= 1;
				break;

			default:
				PRINT_LINE();
				abort();
		}
	}

	LEAVE(0);
}

#undef FINAL_CODE
