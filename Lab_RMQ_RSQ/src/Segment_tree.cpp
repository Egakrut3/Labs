#include "Segment_tree.hpp"

#define FINAL_CODE

int Segment_tree_ctor(struct Segment_tree *const dst, size_t const size) {
	assert(dst);

	dst->pow2 = 1;
	while (dst->pow2 < size) {
		dst->pow2 *= 2;
	}
	CALLOC_ARR(dst->buffer, dst->pow2 * 2 - 1);
	dst->buffer -= 1;

	LEAVE(0);
}

int Segment_tree_set(struct Segment_tree *const obj, size_t ind, int const val) {
	assert(obj);

	ind += obj->pow2;
	obj->buffer[ind] = val;
	ind /= 2;
	while (ind) {
		obj->buffer[ind] = obj->buffer[ind * 2 + 0] + obj->buffer[ind * 2 + 1];

		ind /= 2;
	}

	LEAVE(0);
}

static int Segment_tree_descending(struct Segment_tree const *const __restrict obj, size_t const cur_ind, size_t const cur_l, size_t const cur_r, size_t const q_l, size_t const q_r, long *const __restrict dst) {
	assert(dst);
	assert(obj);

	if (cur_r <= q_l || q_r <= cur_l) { LEAVE(0); }
	if (q_l <= cur_l && cur_r <= q_r) { *dst += obj->buffer[cur_ind]; LEAVE(0); }

	size_t const mid = cur_l + (cur_r - cur_l) / 2;
	CHECK_PROC(Segment_tree_descending, obj, cur_ind * 2 + 0, cur_l, mid, q_l, q_r, dst);
	CHECK_PROC(Segment_tree_descending, obj, cur_ind * 2 + 1, mid, cur_r, q_l, q_r, dst);
	LEAVE(0);
}

int Segment_tree_get(struct Segment_tree const *const __restrict obj, size_t const l, size_t const r, long *const __restrict dst) {
	assert(obj);

	*dst = 0;
	CHECK_PROC(Segment_tree_descending, obj, 1, 0, obj->pow2, l, r, dst);

	LEAVE(0);
}

int Segment_tree_dtor(struct Segment_tree *const obj) {
	assert(obj);

	obj->buffer += 1;
	FREE_ARR(obj->buffer, obj->pow2 * 2 - 1);

	LEAVE(0);
}

#undef FINAL_CODE
