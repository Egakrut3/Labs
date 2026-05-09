#include "RB_tree.hpp"

typedef struct RB_tree_node	node_t;
typedef struct RB_tree		tree_t;

static bool is_red(node_t const *const node) {
	return node && node->color == RB_RED;
}

static node_t *rotate_left(node_t *const node) {
	assert(node); assert(node->right_ch);

	node_t *const r_ch = node->right_ch;
	node->right_ch = r_ch->left_ch;
	r_ch->left_ch = node;
	r_ch->color = node->color;
	node->color = RB_RED;
	return r_ch;
}

static node_t *rotate_right(node_t *const node) {
	assert(node); assert(node->left_ch);

	node_t *const l_ch = node->left_ch;
	node->left_ch = l_ch->right_ch;
	l_ch->right_ch = node;
	l_ch->color = node->color;
	node->color = RB_RED;
	return l_ch;
}

static void flip_colors(node_t *const node) {
	assert(node);

	node->color = (node->color == RB_RED ? RB_BLACK : RB_RED);
	if (node->left_ch) {
		node->left_ch->color = (node->left_ch->color == RB_RED ? RB_BLACK : RB_RED);
	}
	if (node->right_ch) {
		node->right_ch->color = (node->right_ch->color == RB_RED ? RB_BLACK : RB_RED);
	}
}

static node_t *fix_up(node_t *node) {
	assert(node);

	if (is_red(node->right_ch) && !is_red(node->left_ch)) {
		node = rotate_left(node);
	}
	if (is_red(node->left_ch) && is_red(node->left_ch->left_ch)) {
		node = rotate_right(node);
	}
	if (is_red(node->left_ch) && is_red(node->right_ch)) {
		flip_colors(node);
	}

	return node;
}

static node_t *move_red_left(node_t *node) {
	assert(node);

	flip_colors(node);
	if (node->right_ch && is_red(node->right_ch->left_ch)) {
		node->right_ch = rotate_right(node->right_ch);
		node = rotate_left(node);
		flip_colors(node);
	}

	return node;
}

static node_t *move_red_right(node_t *node) {
	assert(node);

	flip_colors(node);
	if (node->left_ch && is_red(node->left_ch->left_ch)) {
		node = rotate_right(node);
		flip_colors(node);
	}

	return node;
}

static node_t *min_node(node_t *node) {
	assert(node);

	while (node->left_ch) { node = node->left_ch; }
	return node;
}

#define FINAL_CODE

int RB_tree_ctor(tree_t *const tree) {
	assert(tree);

	tree->root	= nullptr;
	tree->size	= 0;

	CLEAR_RESOURCES();
	return 0;
}

#undef FINAL_CODE

static int RB_tree_node_dtor(node_t *node) {
	#define FINAL_CODE

	if (!node) {
		CLEAR_RESOURCES();
		return 0;
	}

	CHECK_PROC(RB_tree_node_dtor, node->left_ch);
	CHECK_PROC(RB_tree_node_dtor, node->right_ch);
	TREE_VAL_FREE(node->val);
	FREE_ELEM(node);

	CLEAR_RESOURCES();
	return 0;

	#undef FINAL_CODE
}

int RB_tree_dtor(tree_t *const tree) {
	#define FINAL_CODE

	assert(tree);

	CHECK_PROC(RB_tree_node_dtor, tree->root);
	tree->root	= nullptr;
	tree->size	= 0;

	CLEAR_RESOURCES();
	return 0;

	#undef FINAL_CODE
}

#define FINAL_CODE

static int RB_tree_node_insert(node_t **const node, tree_arg_val_t const val, bool *const inserted) {
	assert(node); assert(inserted);

	if (!*node) {
		ALLOC_ELEM(*node);
		(*node)->left_ch	= nullptr;
		(*node)->right_ch	= nullptr;
		(*node)->color		= RB_RED;
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
		CHECK_PROC(RB_tree_node_insert, &(*node)->left_ch, val, inserted);
	}
	else {
		CHECK_PROC(RB_tree_node_insert, &(*node)->right_ch, val, inserted);
	}

	*node = fix_up(*node);

	CLEAR_RESOURCES();
	return 0;
}

int RB_tree_insert(tree_t *const tree, tree_arg_val_t const val) {
	assert(tree);

	bool inserted = false;
	CHECK_PROC(RB_tree_node_insert, &tree->root, val, &inserted);
	if (tree->root) { tree->root->color = RB_BLACK; }
	if (inserted) { tree->size++; }

	CLEAR_RESOURCES();
	return 0;
}

static node_t *delete_min(node_t *node) {
	assert(node);

	if (!node->left_ch) {
		TREE_VAL_FREE(node->val);
		FREE_ELEM(node);
		return nullptr;
	}

	if (!is_red(node->left_ch) && !is_red(node->left_ch->left_ch)) {
		node = move_red_left(node);
	}

	node->left_ch = delete_min(node->left_ch);
	return fix_up(node);
}

static node_t *erase_node(node_t *node, tree_arg_val_t const val, bool *const erased) {
	assert(erased);

	if (!node) { return nullptr; }

	if (TREE_VAL_LESS(val, node->val)) {
		if (node->left_ch && !is_red(node->left_ch) && !is_red(node->left_ch->left_ch)) {
			node = move_red_left(node);
		}
		node->left_ch = erase_node(node->left_ch, val, erased);
	}
	else {
		if (is_red(node->left_ch)) {
			node = rotate_right(node);
		}
		if (TREE_VAL_EQUAL(val, node->val) && !node->right_ch) {
			TREE_VAL_FREE(node->val);
			FREE_ELEM(node);
			*erased = true;
			return nullptr;
		}
		if (node->right_ch) {
			if (!is_red(node->right_ch) && !is_red(node->right_ch->left_ch)) {
				node = move_red_right(node);
			}
			if (TREE_VAL_EQUAL(val, node->val)) {
				node_t *const next = min_node(node->right_ch);
				TREE_VAL_FREE(node->val);
				TREE_VAL_COPY(node->val, next->val);
				node->right_ch = delete_min(node->right_ch);
				*erased = true;
			}
			else {
				node->right_ch = erase_node(node->right_ch, val, erased);
			}
		}
	}

	return fix_up(node);
}

int RB_tree_erase(tree_t *const tree, tree_arg_val_t const val) {
	assert(tree);

	bool erased = false;
	tree->root = erase_node(tree->root, val, &erased);
	if (tree->root) { tree->root->color = RB_BLACK; }
	if (erased) { tree->size--; }

	CLEAR_RESOURCES();
	return 0;
}

#undef FINAL_CODE
