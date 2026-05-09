#include "B_tree.hpp"

typedef struct B_tree_node	node_t;
typedef struct B_tree		tree_t;

#define FINAL_CODE

static int B_tree_node_ctor(node_t **const node, bool const leaf) {
	assert(node);

	ALLOC_ELEM(*node);
	(*node)->size	= 0;
	(*node)->leaf	= leaf;

	for (size_t i = 0; i < B_TREE_MAX_CHILDREN; i++) {
		(*node)->children[i] = nullptr;
	}

	CLEAR_RESOURCES();
	return 0;
}

int B_tree_ctor(tree_t *const tree) {
	assert(tree);

	tree->root	= nullptr;
	tree->size	= 0;

	CLEAR_RESOURCES();
	return 0;
}

#undef FINAL_CODE

static int B_tree_node_dtor(node_t *node) {
	#define FINAL_CODE

	if (!node) {
		CLEAR_RESOURCES();
		return 0;
	}

	if (!node->leaf) {
		for (size_t i = 0; i <= node->size; i++) {
			CHECK_PROC(B_tree_node_dtor, node->children[i]);
		}
	}

	for (size_t i = 0; i < node->size; i++) {
		TREE_VAL_FREE(node->keys[i]);
	}
	FREE_ELEM(node);

	CLEAR_RESOURCES();
	return 0;

	#undef FINAL_CODE
}

int B_tree_dtor(tree_t *const tree) {
	#define FINAL_CODE

	assert(tree);

	CHECK_PROC(B_tree_node_dtor, tree->root);
	tree->root	= nullptr;
	tree->size	= 0;

	CLEAR_RESOURCES();
	return 0;

	#undef FINAL_CODE
}

static size_t find_key(node_t const *const node, tree_arg_val_t const val) {
	assert(node);

	size_t idx = 0;
	while (idx < node->size && TREE_VAL_LESS(node->keys[idx], val)) {
		idx++;
	}

	return idx;
}

static bool B_tree_node_contains(node_t const *node, tree_arg_val_t const val) {
	while (node) {
		size_t const idx = find_key(node, val);
		if (idx < node->size && TREE_VAL_EQUAL(node->keys[idx], val)) {
			return true;
		}
		if (node->leaf) { return false; }
		node = node->children[idx];
	}

	return false;
}

#define FINAL_CODE

static int split_child(node_t *const node, size_t const idx) {
	assert(node); assert(idx <= node->size);
	assert(node->children[idx]);
	assert(node->children[idx]->size == B_TREE_MAX_KEYS);

	node_t *const old_child = node->children[idx];
	node_t *new_child = nullptr;
	CHECK_PROC(B_tree_node_ctor, &new_child, old_child->leaf);

	new_child->size = B_TREE_MIN_DEGREE - 1;
	for (size_t i = 0; i < B_TREE_MIN_DEGREE - 1; i++) {
		TREE_VAL_COPY(new_child->keys[i], old_child->keys[i + B_TREE_MIN_DEGREE]);
	}
	if (!old_child->leaf) {
		for (size_t i = 0; i < B_TREE_MIN_DEGREE; i++) {
			new_child->children[i] = old_child->children[i + B_TREE_MIN_DEGREE];
		}
	}

	old_child->size = B_TREE_MIN_DEGREE - 1;

	for (size_t i = node->size + 1; i > idx + 1; i--) {
		node->children[i] = node->children[i - 1];
	}
	node->children[idx + 1] = new_child;

	for (size_t i = node->size; i > idx; i--) {
		TREE_VAL_COPY(node->keys[i], node->keys[i - 1]);
	}
	TREE_VAL_COPY(node->keys[idx], old_child->keys[B_TREE_MIN_DEGREE - 1]);
	node->size++;

	CLEAR_RESOURCES();
	return 0;
}

static int insert_non_full(node_t *const node, tree_arg_val_t const val) {
	assert(node);

	size_t idx = node->size;
	if (node->leaf) {
		while (idx > 0 && TREE_VAL_LESS(val, node->keys[idx - 1])) {
			TREE_VAL_COPY(node->keys[idx], node->keys[idx - 1]);
			idx--;
		}

		TREE_VAL_COPY(node->keys[idx], val);
		node->size++;

		CLEAR_RESOURCES();
		return 0;
	}

	while (idx > 0 && TREE_VAL_LESS(val, node->keys[idx - 1])) { idx--; }
	if (node->children[idx]->size == B_TREE_MAX_KEYS) {
		CHECK_PROC(split_child, node, idx);
		if (TREE_VAL_LESS(node->keys[idx], val)) { idx++; }
	}
	CHECK_PROC(insert_non_full, node->children[idx], val);

	CLEAR_RESOURCES();
	return 0;
}

int B_tree_insert(tree_t *const tree, tree_arg_val_t const val) {
	assert(tree);

	if (B_tree_node_contains(tree->root, val)) {
		CLEAR_RESOURCES();
		return 0;
	}

	if (!tree->root) {
		CHECK_PROC(B_tree_node_ctor, &tree->root, true);
		TREE_VAL_COPY(tree->root->keys[0], val);
		tree->root->size = 1;
		tree->size++;

		CLEAR_RESOURCES();
		return 0;
	}

	if (tree->root->size == B_TREE_MAX_KEYS) {
		node_t *new_root = nullptr;
		CHECK_PROC(B_tree_node_ctor, &new_root, false);
		new_root->children[0] = tree->root;
		tree->root = new_root;
		CHECK_PROC(split_child, tree->root, 0);
	}

	CHECK_PROC(insert_non_full, tree->root, val);
	tree->size++;

	CLEAR_RESOURCES();
	return 0;
}

static tree_mem_val_t get_pred(node_t *node) {
	assert(node);

	while (!node->leaf) {
		node = node->children[node->size];
	}

	return node->keys[node->size - 1];
}

static tree_mem_val_t get_succ(node_t *node) {
	assert(node);

	while (!node->leaf) {
		node = node->children[0];
	}

	return node->keys[0];
}

static void remove_from_leaf(node_t *const node, size_t const idx) {
	assert(node); assert(idx < node->size);

	TREE_VAL_FREE(node->keys[idx]);
	for (size_t i = idx + 1; i < node->size; i++) {
		TREE_VAL_COPY(node->keys[i - 1], node->keys[i]);
	}
	node->size--;
}

static void borrow_from_prev(node_t *const node, size_t const idx) {
	assert(node); assert(idx > 0);

	node_t	*const child	= node->children[idx],
		*const sibling	= node->children[idx - 1];

	for (size_t i = child->size; i > 0; i--) {
		TREE_VAL_COPY(child->keys[i], child->keys[i - 1]);
	}
	if (!child->leaf) {
		for (size_t i = child->size + 1; i > 0; i--) {
			child->children[i] = child->children[i - 1];
		}
	}

	TREE_VAL_COPY(child->keys[0], node->keys[idx - 1]);
	if (!child->leaf) {
		child->children[0] = sibling->children[sibling->size];
	}
	TREE_VAL_COPY(node->keys[idx - 1], sibling->keys[sibling->size - 1]);

	child->size++;
	sibling->size--;
}

static void borrow_from_next(node_t *const node, size_t const idx) {
	assert(node); assert(idx < node->size);

	node_t	*const child	= node->children[idx],
		*const sibling	= node->children[idx + 1];

	TREE_VAL_COPY(child->keys[child->size], node->keys[idx]);
	if (!child->leaf) {
		child->children[child->size + 1] = sibling->children[0];
	}
	TREE_VAL_COPY(node->keys[idx], sibling->keys[0]);

	for (size_t i = 1; i < sibling->size; i++) {
		TREE_VAL_COPY(sibling->keys[i - 1], sibling->keys[i]);
	}
	if (!sibling->leaf) {
		for (size_t i = 1; i <= sibling->size; i++) {
			sibling->children[i - 1] = sibling->children[i];
		}
	}

	child->size++;
	sibling->size--;
}

static void merge_children(node_t *const node, size_t const idx) {
	assert(node); assert(idx < node->size);

	node_t	*const child	= node->children[idx],
		*sibling	= node->children[idx + 1];

	TREE_VAL_COPY(child->keys[B_TREE_MIN_DEGREE - 1], node->keys[idx]);
	for (size_t i = 0; i < sibling->size; i++) {
		TREE_VAL_COPY(child->keys[i + B_TREE_MIN_DEGREE], sibling->keys[i]);
	}
	if (!child->leaf) {
		for (size_t i = 0; i <= sibling->size; i++) {
			child->children[i + B_TREE_MIN_DEGREE] = sibling->children[i];
		}
	}

	for (size_t i = idx + 1; i < node->size; i++) {
		TREE_VAL_COPY(node->keys[i - 1], node->keys[i]);
	}
	for (size_t i = idx + 2; i <= node->size; i++) {
		node->children[i - 1] = node->children[i];
	}

	child->size += sibling->size + 1;
	node->size--;
	FREE_ELEM(sibling);
}

static void fill_child(node_t *const node, size_t const idx) {
	assert(node); assert(idx <= node->size);

	if (idx > 0 && node->children[idx - 1]->size >= B_TREE_MIN_DEGREE) {
		borrow_from_prev(node, idx);
	}
	else if (idx < node->size && node->children[idx + 1]->size >= B_TREE_MIN_DEGREE) {
		borrow_from_next(node, idx);
	}
	else if (idx < node->size) {
		merge_children(node, idx);
	}
	else {
		merge_children(node, idx - 1);
	}
}

static int B_tree_node_erase(node_t *const node, tree_arg_val_t const val, bool *const erased);

static int remove_from_non_leaf(node_t *const node, size_t const idx, bool *const erased) {
	assert(node); assert(idx < node->size); assert(erased);

	tree_mem_val_t const val = node->keys[idx];
	if (node->children[idx]->size >= B_TREE_MIN_DEGREE) {
		tree_mem_val_t const pred = get_pred(node->children[idx]);
		TREE_VAL_FREE(node->keys[idx]);
		TREE_VAL_COPY(node->keys[idx], pred);
		CHECK_PROC(B_tree_node_erase, node->children[idx], pred, erased);
	}
	else if (node->children[idx + 1]->size >= B_TREE_MIN_DEGREE) {
		tree_mem_val_t const succ = get_succ(node->children[idx + 1]);
		TREE_VAL_FREE(node->keys[idx]);
		TREE_VAL_COPY(node->keys[idx], succ);
		CHECK_PROC(B_tree_node_erase, node->children[idx + 1], succ, erased);
	}
	else {
		merge_children(node, idx);
		CHECK_PROC(B_tree_node_erase, node->children[idx], val, erased);
	}

	CLEAR_RESOURCES();
	return 0;
}

static int B_tree_node_erase(node_t *const node, tree_arg_val_t const val, bool *const erased) {
	assert(node); assert(erased);

	size_t idx = find_key(node, val);
	if (idx < node->size && TREE_VAL_EQUAL(node->keys[idx], val)) {
		if (node->leaf) {
			remove_from_leaf(node, idx);
			*erased = true;

			CLEAR_RESOURCES();
			return 0;
		}

		CHECK_PROC(remove_from_non_leaf, node, idx, erased);

		CLEAR_RESOURCES();
		return 0;
	}

	if (node->leaf) {
		CLEAR_RESOURCES();
		return 0;
	}

	bool const last_child = (idx == node->size);
	if (node->children[idx]->size < B_TREE_MIN_DEGREE) {
		fill_child(node, idx);
	}

	if (last_child && idx > node->size) {
		CHECK_PROC(B_tree_node_erase, node->children[idx - 1], val, erased);
	}
	else {
		CHECK_PROC(B_tree_node_erase, node->children[idx], val, erased);
	}

	CLEAR_RESOURCES();
	return 0;
}

int B_tree_erase(tree_t *const tree, tree_arg_val_t const val) {
	assert(tree);

	if (!tree->root) {
		CLEAR_RESOURCES();
		return 0;
	}

	bool erased = false;
	CHECK_PROC(B_tree_node_erase, tree->root, val, &erased);
	if (tree->root->size == 0) {
		node_t *old_root = tree->root;
		if (tree->root->leaf)	{ tree->root = nullptr; }
		else			{ tree->root = tree->root->children[0]; }
		FREE_ELEM(old_root);
	}
	if (erased) { tree->size--; }

	CLEAR_RESOURCES();
	return 0;
}

#undef FINAL_CODE
