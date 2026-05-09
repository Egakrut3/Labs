#include "Skip_list.hpp"

typedef struct Skip_list_node	node_t;
typedef struct Skip_list	list_t;

static uint32_t next_rand(list_t *const list) {
	assert(list);

	list->rand_state = list->rand_state * 1103515245u + 12345u;
	return list->rand_state;
}

#define FINAL_CODE

static int Skip_list_node_ctor(node_t **const node, tree_arg_val_t const val, size_t const level) {
	assert(node);
	assert(level > 0);
	assert(level <= SKIP_LIST_MAX_LEVEL);

	ALLOC_ELEM(*node);
	(*node)->level	= level;
	TREE_VAL_COPY((*node)->val, val);
	(*node)->next	= nullptr;
	#undef FINAL_CODE
	#define FINAL_CODE	\
	FREE_ELEM(*node);

	ALLOC_ARR((*node)->next, level);
	for (size_t i = 0; i < level; i++) {
		(*node)->next[i] = nullptr;
	}

	#undef FINAL_CODE
	#define FINAL_CODE

	CLEAR_RESOURCES();
	return 0;

}

static int Skip_list_node_dtor(node_t *node) {
	assert(node);

	TREE_VAL_FREE(node->val);
	FREE_ARR(node->next, node->level);
	FREE_ELEM(node);

	CLEAR_RESOURCES();
	return 0;
}

int Skip_list_ctor(list_t *const list) {
	assert(list);

	list->header		= nullptr;
	list->level		= 1;
	list->size		= 0;
	list->rand_state	= 0x51F15EEDu;

	CHECK_PROC(Skip_list_node_ctor, &list->header, 0, SKIP_LIST_MAX_LEVEL);

	CLEAR_RESOURCES();
	return 0;
}

int Skip_list_dtor(list_t *const list) {
	assert(list);

	if (list->header) {
		node_t *node = list->header->next[0];
		while (node) {
			node_t *const next = node->next[0];
			CHECK_PROC(Skip_list_node_dtor, node);
			node = next;
		}
		CHECK_PROC(Skip_list_node_dtor, list->header);
	}

	list->header	= nullptr;
	list->level	= 1;
	list->size	= 0;

	CLEAR_RESOURCES();
	return 0;
}

static size_t random_level(list_t *const list) {
	assert(list);

	size_t level = 1;
	while (level < SKIP_LIST_MAX_LEVEL && (next_rand(list) & 1u)) {
		level++;
	}

	return level;
}

int Skip_list_insert(list_t *const list, tree_arg_val_t const val) {
	assert(list); assert(list->header);

	node_t *update[SKIP_LIST_MAX_LEVEL] = {};
	node_t *node = list->header;

	for (size_t i = list->level; i > 0; i--) {
		size_t const level_ind = i - 1;
		while (node->next[level_ind] && TREE_VAL_LESS(node->next[level_ind]->val, val)) {
			node = node->next[level_ind];
		}
		update[level_ind] = node;
	}

	node = node->next[0];
	if (node && TREE_VAL_EQUAL(node->val, val)) {
		CLEAR_RESOURCES();
		return 0;
	}

	size_t const new_level = random_level(list);
	if (new_level > list->level) {
		for (size_t i = list->level; i < new_level; i++) {
			update[i] = list->header;
		}
		list->level = new_level;
	}

	node = nullptr;
	CHECK_PROC(Skip_list_node_ctor, &node, val, new_level);
	for (size_t i = 0; i < new_level; i++) {
		node->next[i] = update[i]->next[i];
		update[i]->next[i] = node;
	}
	list->size++;

	CLEAR_RESOURCES();
	return 0;
}

int Skip_list_erase(list_t *const list, tree_arg_val_t const val) {
	assert(list); assert(list->header);

	node_t *update[SKIP_LIST_MAX_LEVEL] = {};
	node_t *node = list->header;

	for (size_t i = list->level; i > 0; i--) {
		size_t const level_ind = i - 1;
		while (node->next[level_ind] && TREE_VAL_LESS(node->next[level_ind]->val, val)) {
			node = node->next[level_ind];
		}
		update[level_ind] = node;
	}

	node = node->next[0];
	if (!node || !TREE_VAL_EQUAL(node->val, val)) {
		CLEAR_RESOURCES();
		return 0;
	}

	for (size_t i = 0; i < list->level; i++) {
		if (update[i]->next[i] != node) { break; }
		update[i]->next[i] = node->next[i];
	}

	while (list->level > 1 && !list->header->next[list->level - 1]) {
		list->level--;
	}

	CHECK_PROC(Skip_list_node_dtor, node);
	list->size--;

	CLEAR_RESOURCES();
	return 0;
}

#undef FINAL_CODE
