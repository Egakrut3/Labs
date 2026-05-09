#include "AVL_tree.hpp"

typedef struct AVL_tree_node	node_t;
typedef struct AVL_tree		tree_t;

#define FINAL_CODE

int AVL_tree_ctor(tree_t *const tree) {
	assert(tree);

	tree->root	= nullptr;
	tree->size	= 0;

	CLEAR_RESOURCES();
	return 0;
}

#undef FINAL_CODE

static int AVL_tree_node_dtor(node_t *node) {
	#define FINAL_CODE

	if (!node) {
		CLEAR_RESOURCES();
		return 0;
	}

	CHECK_PROC(AVL_tree_node_dtor, node->left_ch);
	CHECK_PROC(AVL_tree_node_dtor, node->right_ch);
	TREE_VAL_FREE(node->val);
	FREE_ELEM(node);

	CLEAR_RESOURCES();
	return 0;

	#undef FINAL_CODE
}

int AVL_tree_dtor(tree_t *const tree) {
	#define FINAL_CODE

	assert(tree);

	CHECK_PROC(AVL_tree_node_dtor, tree->root);
	tree->root	= nullptr;
	tree->size	= 0;

	CLEAR_RESOURCES();
	return 0;

	#undef FINAL_CODE
}

static size_t get_height(node_t const *const node) {
	if (node)	{ return node->height; }
	else		{ return 0; }
}

static void update_height(node_t *const node) {
	assert(node);

	size_t const	l_h = get_height(node->left_ch),
			r_h = get_height(node->right_ch);
	node->height = (l_h > r_h ? l_h : r_h) + 1;
}

static void left_rotate(node_t **const node) {
	assert(node); assert(*node); assert((*node)->right_ch);

	node_t	*const cur	= *node,
		*const r_ch	= cur->right_ch,
		*const l_r_ch	= r_ch->left_ch;

	*node			= r_ch;
	r_ch->left_ch		= cur;
	cur->right_ch		= l_r_ch;

	update_height(cur);
	update_height(r_ch);
}

static void right_rotate(node_t **const node) {
	assert(node); assert(*node); assert((*node)->left_ch);

	node_t	*const cur	= *node,
		*const l_ch	= cur->left_ch,
		*const r_l_ch	= l_ch->right_ch;

	*node			= l_ch;
	l_ch->right_ch		= cur;
	cur->left_ch		= r_l_ch;

	update_height(cur);
	update_height(l_ch);
}

static void rebalance(node_t **const node) {
	assert(node); assert(*node);

	int64_t const h_diff = (int64_t)get_height((*node)->right_ch) - (int64_t)get_height((*node)->left_ch);
	if (h_diff < -1) {
		assert((*node)->left_ch);

		if (get_height((*node)->left_ch->left_ch) < get_height((*node)->left_ch->right_ch)) {
			left_rotate(&(*node)->left_ch);
		}
		right_rotate(node);
	}
	else if (h_diff > 1) {
		assert((*node)->right_ch);

		if (get_height((*node)->right_ch->left_ch) > get_height((*node)->right_ch->right_ch)) {
			right_rotate(&(*node)->right_ch);
		}
		left_rotate(node);
	}
	else {
		update_height(*node);
	}
}

#define FINAL_CODE

static int AVL_tree_node_insert(node_t **const node, tree_arg_val_t const val, bool *const inserted) {
	assert(node); assert(inserted);

	if (!*node) {
		ALLOC_ELEM(*node);
		(*node)->left_ch	= nullptr;
		(*node)->right_ch	= nullptr;
		(*node)->height		= 1;
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
		CHECK_PROC(AVL_tree_node_insert, &(*node)->left_ch, val, inserted);
	}
	else {
		CHECK_PROC(AVL_tree_node_insert, &(*node)->right_ch, val, inserted);
	}

	rebalance(node);

	CLEAR_RESOURCES();
	return 0;
}

int AVL_tree_insert(tree_t *const tree, tree_arg_val_t const val) {
	assert(tree);

	bool inserted = false;
	CHECK_PROC(AVL_tree_node_insert, &tree->root, val, &inserted);
	if (inserted) { tree->size++; }

	CLEAR_RESOURCES();
	return 0;
}

static int extract_min(node_t **const node, tree_mem_val_t *const dest) {
	assert(node); assert(*node); assert(dest);

	if (!(*node)->left_ch) {
		node_t *const r_ch = (*node)->right_ch;
		TREE_VAL_MOVE(*dest, (*node)->val);
		FREE_ELEM(*node);
		*node = r_ch;

		CLEAR_RESOURCES();
		return 0;
	}

	CHECK_PROC(extract_min, &(*node)->left_ch, dest);
	if (*node) { rebalance(node); }

	CLEAR_RESOURCES();
	return 0;
}

static int AVL_tree_node_erase(node_t **const node, tree_arg_val_t const val, bool *const erased) {
	assert(node); assert(erased);

	if (!*node) {
		CLEAR_RESOURCES();
		return 0;
	}

	if (TREE_VAL_EQUAL(val, (*node)->val)) {
		*erased = true;
		if (!(*node)->right_ch) {
			node_t *const l_ch = (*node)->left_ch;
			TREE_VAL_FREE((*node)->val);
			FREE_ELEM(*node);
			*node = l_ch;

			CLEAR_RESOURCES();
			return 0;
		}

		TREE_VAL_FREE((*node)->val);
		CHECK_PROC(extract_min, &(*node)->right_ch, &(*node)->val);
	}
	else if (TREE_VAL_LESS(val, (*node)->val)) {
		CHECK_PROC(AVL_tree_node_erase, &(*node)->left_ch, val, erased);
	}
	else {
		CHECK_PROC(AVL_tree_node_erase, &(*node)->right_ch, val, erased);
	}

	if (*node) { rebalance(node); }

	CLEAR_RESOURCES();
	return 0;
}

int AVL_tree_erase(tree_t *const tree, tree_arg_val_t const val) {
	assert(tree);

	bool erased = false;
	CHECK_PROC(AVL_tree_node_erase, &tree->root, val, &erased);
	if (erased) { tree->size--; }

	CLEAR_RESOURCES();
	return 0;
}

#undef FINAL_CODE
