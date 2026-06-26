#include "Fenwick_tree.hpp"

#define FINAL_CODE

int Fenwick_tree_ctor(struct Fenwick_tree *const dst, size_t const size) {
	assert(dst);

	dst->size = size;
	CALLOC_ARR(dst->buffer, size);
	dst->buffer -= 1;

	LEAVE(0);
}

int Fenwick_tree_increase(struct Fenwick_tree *const obj, size_t ind, int const val) {
	assert(obj);

	for (ind += 1; ind <= obj->size; ind = (ind | (ind - 1)) + 1) {
		obj->buffer[ind] += val;
	}

	LEAVE(0);
}

int Fenwick_tree_get_pref(struct Fenwick_tree const *const __restrict obj, size_t r, long *const __restrict dst) {
	assert(dst);
	assert(obj);

	*dst = 0;
	while (r) {
		*dst += obj->buffer[r];

		r &= (r - 1);
	}

	LEAVE(0);
}

int Fenwick_tree_dtor(struct Fenwick_tree *obj) {
	assert(obj);

	obj->buffer += 1;
	FREE_ARR(obj->buffer, obj->size);

	LEAVE(0);
}

int Fenwick_tree_get(struct Fenwick_tree const *const __restrict obj, size_t const l, size_t const r, long *const __restrict dst) {
	assert(dst);
	assert(obj);

	CHECK_PROC(Fenwick_tree_get_pref, obj, r, dst);
	long sum_l = 0;
	CHECK_PROC(Fenwick_tree_get_pref, obj, l, &sum_l);
	*dst -= sum_l;

	LEAVE(0);
}

#undef FINAL_CODE
