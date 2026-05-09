#include "Treap.hpp"

typedef struct Treap_node	node_t;
typedef struct Treap		tree_t;

static uint32_t make_priority(tree_arg_val_t const val) {
	uint32_t x = (uint32_t)val + 0x9e3779b9u;
	x ^= x >> 16;
	x *= 0x85ebca6bu;
	x ^= x >> 13;
	x *= 0xc2b2ae35u;
	x ^= x >> 16;
	return x;
}

#define FINAL_CODE

int Treap_ctor(tree_t *const tree) {
	assert(tree);

	tree->root	= nullptr;
	tree->size	= 0;

	CLEAR_RESOURCES();
	return 0;
}

#undef FINAL_CODE

static int Treap_node_dtor(node_t *node) {
	#define FINAL_CODE

	if (!node) {
		CLEAR_RESOURCES();
		return 0;
	}

	CHECK_PROC(Treap_node_dtor, node->left_ch);
	CHECK_PROC(Treap_node_dtor, node->right_ch);
	TREE_VAL_FREE(node->val);
	FREE_ELEM(node);

	CLEAR_RESOURCES();
	return 0;

	#undef FINAL_CODE
}

int Treap_dtor(tree_t *const tree) {
	#define FINAL_CODE

	assert(tree);

	CHECK_PROC(Treap_node_dtor, tree->root);
	tree->root	= nullptr;
	tree->size	= 0;

	CLEAR_RESOURCES();
	return 0;

	#undef FINAL_CODE
}

static void left_rotate(node_t **const node) {
	assert(node); assert(*node); assert((*node)->right_ch);

	node_t	*const cur	= *node,
		*const r_ch	= cur->right_ch;

	cur->right_ch	= r_ch->left_ch;
	r_ch->left_ch	= cur;
	*node		= r_ch;
}

static void right_rotate(node_t **const node) {
	assert(node); assert(*node); assert((*node)->left_ch);

	node_t	*const cur	= *node,
		*const l_ch	= cur->left_ch;

	cur->left_ch	= l_ch->right_ch;
	l_ch->right_ch	= cur;
	*node		= l_ch;
}

#define FINAL_CODE

static int Treap_node_insert(node_t **const node, tree_arg_val_t const val, bool *const inserted) {
	assert(node); assert(inserted);

	if (!*node) {
		ALLOC_ELEM(*node);
		(*node)->left_ch	= nullptr;
		(*node)->right_ch	= nullptr;
		(*node)->priority	= make_priority(val);
		TREE_VAL_COPY((*node)->val, val);
		*inserted = true;

		CLEAR_RESOURCES();
		return 0;
	}

	if (TREE_VAL_EQUAL(val, (*node)->val)) {
		CLEAR_RESOURCES();
		return 0;
	}
	if (TREE_VAL_LESS(val, (*node)->val)) {
		CHECK_PROC(Treap_node_insert, &(*node)->left_ch, val, inserted);
		if ((*node)->left_ch && (*node)->left_ch->priority > (*node)->priority) {
			right_rotate(node);
		}
	}
	else {
		CHECK_PROC(Treap_node_insert, &(*node)->right_ch, val, inserted);
		if ((*node)->right_ch && (*node)->right_ch->priority > (*node)->priority) {
			left_rotate(node);
		}
	}

	CLEAR_RESOURCES();
	return 0;
}

int Treap_insert(tree_t *const tree, tree_arg_val_t const val) {
	assert(tree);

	bool inserted = false;
	CHECK_PROC(Treap_node_insert, &tree->root, val, &inserted);
	if (inserted) { tree->size++; }

	CLEAR_RESOURCES();
	return 0;
}

static node_t *merge_nodes(node_t *const left, node_t *const right) {
	if (!left)	{ return right; }
	if (!right)	{ return left; }

	if (left->priority > right->priority) {
		left->right_ch = merge_nodes(left->right_ch, right);
		return left;
	}

	right->left_ch = merge_nodes(left, right->left_ch);
	return right;
}

static int Treap_node_erase(node_t **const node, tree_arg_val_t const val, bool *const erased) {
	assert(node); assert(erased);

	if (!*node) {
		CLEAR_RESOURCES();
		return 0;
	}

	if (TREE_VAL_EQUAL(val, (*node)->val)) {
		node_t *old_node = *node;
		*node = merge_nodes((*node)->left_ch, (*node)->right_ch);
		TREE_VAL_FREE(old_node->val);
		FREE_ELEM(old_node);
		*erased = true;

		CLEAR_RESOURCES();
		return 0;
	}
	if (TREE_VAL_LESS(val, (*node)->val)) {
		CHECK_PROC(Treap_node_erase, &(*node)->left_ch, val, erased);
	}
	else {
		CHECK_PROC(Treap_node_erase, &(*node)->right_ch, val, erased);
	}

	CLEAR_RESOURCES();
	return 0;
}

int Treap_erase(tree_t *const tree, tree_arg_val_t const val) {
	assert(tree);

	bool erased = false;
	CHECK_PROC(Treap_node_erase, &tree->root, val, &erased);
	if (erased) { tree->size--; }

	CLEAR_RESOURCES();
	return 0;
}

#undef FINAL_CODE
