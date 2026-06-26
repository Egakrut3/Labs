#include "Strong_fenwick_tree.hpp"

int Strong_fenwick_tree_ctor(struct Strong_fenwick_tree *dst, size_t size) {
	#define FINAL_CODE

	assert(dst);

	dst->size = size;
	CALLOC_ARR(dst->buffer, size);
	dst->buffer -= 1;
	#undef FINAL_CODE
	#define FINAL_CODE			\
	FREE_ARR(dst->buffer + 1, size);

	CALLOC_ARR(dst->arr, size);
	#undef FINAL_CODE
	#define FINAL_CODE

	LEAVE(0);

	#undef FINAL_CODE
}

#define FINAL_CODE

int Strong_fenwick_tree_set(struct Strong_fenwick_tree *const obj, size_t ind, int val) {
	assert(obj);

	val -= obj->arr[ind];
	obj->arr[ind] += val;
	for (ind += 1; ind <= obj->size; ind = (ind | (ind - 1)) + 1) {
		obj->buffer[ind] += val;
	}

	LEAVE(0);
}

static int Strong_fenwick_tree_get_pref(struct Strong_fenwick_tree const *const __restrict obj, size_t r, long *const __restrict dst) {
	assert(dst);
	assert(obj);

	*dst = 0;
	for (; r; r &= (r - 1)) {
		*dst += obj->buffer[r];
	}

	LEAVE(0);
}

int Strong_fenwick_tree_get(struct Strong_fenwick_tree const *const __restrict obj, size_t const l, size_t const r, long *const __restrict dst) {
	assert(dst);
	assert(obj);

	CHECK_PROC(Strong_fenwick_tree_get_pref, obj, r, dst);
	long sum_l = 0;
	CHECK_PROC(Strong_fenwick_tree_get_pref, obj, l, &sum_l);
	*dst -= sum_l;

	LEAVE(0);
}

int Strong_fenwick_tree_dtor(struct Strong_fenwick_tree *const obj) {
	assert(obj);

	FREE_ARR(obj->arr, obj->size);

	FREE_ARR(obj->buffer + 1, obj->size);

	LEAVE(0);
}

#undef FINAL_CODE
