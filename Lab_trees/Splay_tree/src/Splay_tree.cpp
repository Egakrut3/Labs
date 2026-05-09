#include "Splay_tree.hpp"

typedef struct Splay_tree_node	node_t;
typedef struct Splay_tree	tree_t;

#define FINAL_CODE

int Splay_tree_ctor(tree_t *const tree) {
	assert(tree);

	tree->root	= nullptr;
	tree->size	= 0;

	CLEAR_RESOURCES();
	return 0;
}

#undef FINAL_CODE

int Splay_tree_dtor(tree_t *const tree) {
	#define FINAL_CODE

	assert(tree);

	node_t **stk = nullptr;
	ALLOC_ARR(stk, tree->size + 1);
	#undef FINAL_CODE
	#define FINAL_CODE		\
	FREE_ARR(stk, tree->size + 1);

	size_t last_ind = 0;
	if (tree->root) {
		stk[last_ind++] = tree->root;
	}

	while (last_ind) {
		node_t *node = stk[--last_ind];
		if (node->left_ch)	{ stk[last_ind++] = node->left_ch; }
		if (node->right_ch)	{ stk[last_ind++] = node->right_ch; }

		TREE_VAL_FREE(node->val);
		FREE_ELEM(node);
	}

	tree->root	= nullptr;
	tree->size	= 0;

	CLEAR_RESOURCES();
	return 0;

	#undef FINAL_CODE
}

static void replace_child(tree_t *const tree, node_t *const parent, node_t *const old_child, node_t *const new_child) {
	assert(tree);

	if (!parent) {
		tree->root = new_child;
	}
	else if (parent->left_ch == old_child) {
		parent->left_ch = new_child;
	}
	else {
		assert(parent->right_ch == old_child);
		parent->right_ch = new_child;
	}

	if (new_child) {
		new_child->parent = parent;
	}
}

static void rotate_left(tree_t *const tree, node_t *const node) {
	assert(tree); assert(node); assert(node->right_ch);

	node_t	*const r_ch	= node->right_ch,
		*const parent	= node->parent;

	node->right_ch = r_ch->left_ch;
	if (node->right_ch) { node->right_ch->parent = node; }

	r_ch->left_ch = node;
	node->parent = r_ch;

	replace_child(tree, parent, node, r_ch);
}

static void rotate_right(tree_t *const tree, node_t *const node) {
	assert(tree); assert(node); assert(node->left_ch);

	node_t	*const l_ch	= node->left_ch,
		*const parent	= node->parent;

	node->left_ch = l_ch->right_ch;
	if (node->left_ch) { node->left_ch->parent = node; }

	l_ch->right_ch = node;
	node->parent = l_ch;

	replace_child(tree, parent, node, l_ch);
}

static void splay(tree_t *const tree, node_t *const node) {
	assert(tree); assert(node);

	while (node->parent) {
		node_t	*const parent	= node->parent,
			*const grand	= parent->parent;

		if (!grand) {
			if (parent->left_ch == node)	{ rotate_right(tree, parent); }
			else				{ rotate_left(tree, parent); }
		}
		else if (grand->left_ch == parent && parent->left_ch == node) {
			rotate_right(tree, grand);
			rotate_right(tree, parent);
		}
		else if (grand->right_ch == parent && parent->right_ch == node) {
			rotate_left(tree, grand);
			rotate_left(tree, parent);
		}
		else if (parent->left_ch == node) {
			rotate_right(tree, parent);
			rotate_left(tree, grand);
		}
		else {
			rotate_left(tree, parent);
			rotate_right(tree, grand);
		}
	}
}

static node_t *find_node(tree_t *const tree, tree_arg_val_t const val) {
	assert(tree);

	node_t	*node = tree->root,
		*last = nullptr;

	while (node) {
		last = node;
		if (TREE_VAL_EQUAL(val, node->val)) {
			splay(tree, node);
			return node;
		}
		if (TREE_VAL_LESS(val, node->val))	{ node = node->left_ch; }
		else					{ node = node->right_ch; }
	}

	if (last) { splay(tree, last); }
	return nullptr;
}

#define FINAL_CODE

int Splay_tree_insert(tree_t *const tree, tree_arg_val_t const val) {
	assert(tree);

	node_t	*parent = nullptr,
		*node = tree->root;

	while (node) {
		parent = node;
		if (TREE_VAL_EQUAL(val, node->val)) {
			splay(tree, node);
			CLEAR_RESOURCES();
			return 0;
		}
		if (TREE_VAL_LESS(val, node->val))	{ node = node->left_ch; }
		else					{ node = node->right_ch; }
	}

	ALLOC_ELEM(node);
	node->left_ch	= nullptr;
	node->right_ch	= nullptr;
	node->parent	= parent;
	TREE_VAL_COPY(node->val, val);

	if (!parent) {
		tree->root = node;
	}
	else if (TREE_VAL_LESS(val, parent->val)) {
		parent->left_ch = node;
	}
	else {
		parent->right_ch = node;
	}

	splay(tree, node);
	tree->size++;

	CLEAR_RESOURCES();
	return 0;
}

int Splay_tree_erase(tree_t *const tree, tree_arg_val_t const val) {
	assert(tree);

	node_t *node = find_node(tree, val);
	if (!node || !TREE_VAL_EQUAL(val, node->val)) {
		CLEAR_RESOURCES();
		return 0;
	}

	node_t	*left_ch	= node->left_ch,
		*right_ch	= node->right_ch;

	if (left_ch)	{ left_ch->parent = nullptr; }
	if (right_ch)	{ right_ch->parent = nullptr; }

	TREE_VAL_FREE(node->val);
	FREE_ELEM(node);
	tree->size--;

	if (!left_ch) {
		tree->root = right_ch;
		CLEAR_RESOURCES();
		return 0;
	}

	tree->root = left_ch;
	node_t *max_node = left_ch;
	while (max_node->right_ch) { max_node = max_node->right_ch; }
	splay(tree, max_node);

	tree->root->right_ch = right_ch;
	if (right_ch) { right_ch->parent = tree->root; }

	CLEAR_RESOURCES();
	return 0;
}

#undef FINAL_CODE
