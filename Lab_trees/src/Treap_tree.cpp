#include "Treap_tree.hpp"

typedef struct Treap_tree_node	node_t;
typedef struct Treap_tree	tree_t;

#define FINAL_CODE

int Treap_tree_ctor(tree_t *const dst) {
	assert(dst);

	dst->root = nullptr;

	LEAVE(0);
}

#undef FINAL_CODE

static node_t *merge(node_t *const __restrict left, node_t *const __restrict right) {
	if (!left) 	{ return right; }
	if (!right)	{ return left; }

	if (left->prior > right->prior) {
		left->right = merge(left->right, right);
		return left;
	}
	else {
		right->left = merge(left, right->left);
		return right;
	}
}

static void split(node_t *const __restrict obj, int val, node_t **const __restrict l_dst, node_t **const __restrict r_dst) {
	assert(l_dst); assert(r_dst);

	if (!obj) {
		*l_dst = nullptr;
		*r_dst = nullptr;

		return;
	}

	if (obj->val < val) {
		split(obj->right, val, l_dst, r_dst);
		obj->right = *l_dst;
		*l_dst = obj;
	}
	else {
		split(obj->left, val, l_dst, r_dst);
		obj->left = *r_dst;
		*r_dst = obj;
	}
}

#define FINAL_CODE

static int Treap_tree_node_ctor(node_t *const dst, int val) {
	assert(dst);

	dst->left = nullptr;
	dst->right = nullptr;
	dst->prior = random();

	dst->val = val;

	LEAVE(0);
}

int Treap_tree_insert(tree_t *const obj, int const val) {
	assert(obj);

	node_t	*left = nullptr,
		*mid = nullptr,
		*right = nullptr;
	split(obj->root, val, &left, &right);
	NEW(Treap_tree_node, mid, val);

	obj->root = merge(merge(left, mid), right);

	LEAVE(0);
}

static int Treap_tree_node_dtor(node_t *const obj) {
	assert(obj);

	LEAVE(0);
}

static int Treap_tree_tree_node_erase(node_t **const dst, int const val) {
	assert(dst);

	if (!*dst) { LEAVE(0); }

	if (val < (*dst)->val) {
		CHECK_PROC(Treap_tree_tree_node_erase, &(*dst)->left, val);

		LEAVE(0);
	}
	else if (val > (*dst)->val) {
		CHECK_PROC(Treap_tree_tree_node_erase, &(*dst)->right, val);

		LEAVE(0);
	}

	node_t *const save_ptr = merge((*dst)->left, (*dst)->right);
	DELETE_CHECKED(Treap_tree_node, *dst);
	*dst = save_ptr;

	LEAVE(0);
}

int Treap_tree_erase(tree_t *const obj, int const val) {
	assert(obj);

	CHECK_PROC(Treap_tree_tree_node_erase, &obj->root, val);

	LEAVE(0);
}

static int Treap_tree_recursive_dtor(node_t *const obj) {
	if (!obj) { LEAVE(0); }

	CHECK_PROC(Treap_tree_recursive_dtor, obj->left);
	CHECK_PROC(Treap_tree_recursive_dtor, obj->right);

	DELETE_CHECKED(Treap_tree_node, obj);

	LEAVE(0);
}

int Treap_tree_dtor(tree_t *const obj) {
	assert(obj);

	CHECK_PROC(Treap_tree_recursive_dtor, obj->root);

	LEAVE(0);
}

#undef FINAL_CODE
