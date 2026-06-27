#include "AVL_tree.hpp"

#define FINAL_CODE

typedef struct AVL_tree_node	node_t;
typedef struct AVL_tree		tree_t;

int AVL_tree_ctor(tree_t *const dst) {
	assert(dst);

	dst->root = nullptr;

	LEAVE(0);
}

#undef FINAL_CODE

static size_t get_height(node_t const *const obj) {
	if (obj)	{ return obj->height; }
	else		{ return 0; }
}

static void update_height(node_t *const *const dst) {
	assert(dst);

	size_t const	l_h = get_height((*dst)->left),
			r_h = get_height((*dst)->right);
	(*dst)->height = (l_h > r_h ? l_h : r_h) + 1;
}

static void left_rotate(node_t **const dst) {
	assert(dst);
	assert((*dst)->right);

	node_t	*const cur	= *dst,
		*const r_ch	= cur->right,
		*const l_r_ch	= r_ch->left;
	*dst			= r_ch;
	(*dst)->left		= cur;
	(*dst)->left->right	= l_r_ch;

	update_height(&(*dst)->left);
	update_height(dst);
}

static void right_rotate(node_t **const dst) {
	assert(dst);
	assert((*dst)->left);

	node_t	*const cur	= *dst,
		*const l_ch	= cur->left,
		*const r_l_ch	= l_ch->right;
	*dst			= l_ch;
	(*dst)->right		= cur;
	(*dst)->right->left	= r_l_ch;

	update_height(&(*dst)->right);
	update_height(dst);
}

static void rebalance(node_t **const dst) {
	assert(dst);

	ssize_t const h_diff = (ssize_t)get_height((*dst)->right) - (ssize_t)get_height((*dst)->left);
	if (h_diff < -1) {
		assert((*dst)->left);

		if (get_height((*dst)->left->left) < get_height((*dst)->left->right)) {
			left_rotate(&(*dst)->left);
		}
		right_rotate(dst);
	}
	else if (h_diff > 1) {
		assert((*dst)->right);

		if (get_height((*dst)->right->left) > get_height((*dst)->right->right)) {
			right_rotate(&(*dst)->right);
		}
		left_rotate(dst);
	}
	else { update_height(dst); }
}

#define FINAL_CODE

static int AVL_tree_node_ctor(node_t *const dst, int val) {
	assert(dst);

	dst->left = nullptr;
	dst->right = nullptr;
	dst->height = 1;

	dst->val = val;

	LEAVE(0);
}

static int AVL_tree_node_insert(node_t **const dst, int const val) {
	assert(dst);

	if (!*dst) { NEW(AVL_tree_node, *dst, val); LEAVE(0); }

	if (val < (*dst)->val) {
		CHECK_PROC(AVL_tree_node_insert, &(*dst)->left, val);
	}
	else {
		CHECK_PROC(AVL_tree_node_insert, &(*dst)->right, val);
	}
	rebalance(dst);

	LEAVE(0);
}

int AVL_tree_insert(tree_t *const obj, int const val) {
	assert(obj);

	CHECK_PROC(AVL_tree_node_insert, &obj->root, val);

	LEAVE(0);
}

static int AVL_tree_node_dtor(node_t *const obj) {
	assert(obj);

	LEAVE(0);
}

static int extract_min(node_t **const node_dst, int *const val_dst) {
	assert(node_dst); assert(val_dst);
	assert(*node_dst);

	if ((*node_dst)->left) {
		CHECK_PROC(extract_min, &(*node_dst)->left, val_dst);
		rebalance(node_dst);

		LEAVE(0);
	}

	node_t *const save_ptr = (*node_dst)->right;
	DELETE_CHECKED(AVL_tree_node, *node_dst);
	*node_dst = save_ptr;

	LEAVE(0);
}

static int AVL_tree_node_erase(node_t **const dst, int const val) {
	assert(dst);

	if (!*dst) { LEAVE(0); }

	if (val < (*dst)->val) {
		CHECK_PROC(AVL_tree_node_erase, &(*dst)->left, val);
		rebalance(dst);

		LEAVE(0);
	}
	else if (val > (*dst)->val) {
		CHECK_PROC(AVL_tree_node_erase, &(*dst)->right, val);
		rebalance(dst);

		LEAVE(0);
	}

	if (!(*dst)->right) {
		node_t *const save_ptr = (*dst)->left;
		DELETE_CHECKED(AVL_tree_node, *dst);
		*dst = save_ptr;

		LEAVE(0);
	}

	CHECK_PROC(extract_min, &(*dst)->right, &(*dst)->val);
	rebalance(dst);

	LEAVE(0);
}

int AVL_tree_erase(tree_t *const obj, int const val) {
	assert(obj);

	CHECK_PROC(AVL_tree_node_erase, &obj->root, val);

	LEAVE(0);
}

static int AVL_tree_recursive_dtor(node_t *const obj) {
	if (!obj) { LEAVE(0); }

	CHECK_PROC(AVL_tree_recursive_dtor, obj->left);
	CHECK_PROC(AVL_tree_recursive_dtor, obj->right);

	DELETE_CHECKED(AVL_tree_node, obj);

	LEAVE(0);
}

int AVL_tree_dtor(tree_t *const obj) {
	assert(obj);

	CHECK_PROC(AVL_tree_recursive_dtor, obj->root);

	LEAVE(0);
}

#undef FINAL_CODE
