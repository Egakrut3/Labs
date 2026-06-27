#include "Sparce_table.hpp"

int Long_sparce_table_ctor(struct Long_sparce_table *const __restrict dst, size_t const size, int const *const __restrict arr) {
	#define FINAL_CODE

	assert(dst);
	assert(arr);

	CALLOC_ARR(dst->lg2_arr, size);
	dst->lg2_arr -= 1;
	#undef FINAL_CODE
	#define FINAL_CODE			\
	FREE_ARR(dst->lg2_arr + 1, size);

	dst->lg2 = 1;
	while (((size_t)1 << dst->lg2) < size) {
		dst->lg2_arr[((size_t)1 << dst->lg2) + 1] = dst->lg2;
		dst->lg2 += 1;
	}
	dst->size = size;
	CALLOC_ARR(dst->buffer, size * dst->lg2);
	#undef FINAL_CODE
	#define FINAL_CODE

	for (size_t i = 0; i < size; i++) {
		dst->buffer[i] = arr[i];
	}

	for (size_t prev_pw = 0; prev_pw + 1 < dst->lg2; prev_pw++) {
		for (size_t i = 0; i + ((size_t)1 << (prev_pw + 1)) <= size; i++) {
			int	l_val = dst->buffer[size * prev_pw + i],
				r_val = dst->buffer[size * prev_pw + i + ((size_t)1 << prev_pw)];
			dst->buffer[size * (prev_pw + 1) + i] = l_val < r_val ? l_val : r_val;
		}
	}

	for (size_t i = 2; i <= size; i++) {
		if (!dst->lg2_arr[i]) {
			dst->lg2_arr[i] = dst->lg2_arr[i - 1];
		}
	}

	LEAVE(0);

	#undef FINAL_CODE
}

#define FINAL_CODE

int Long_sparce_table_get(struct Long_sparce_table const *const __restrict obj, size_t const l, size_t const r, int *const __restrict dst) {
	assert(dst);
	assert(obj);

	size_t	lg2	= obj->lg2_arr[r - l];
	int	l_val	= obj->buffer[obj->size * lg2 + l],
		r_val	= obj->buffer[obj->size * lg2 + r - ((size_t)1 << lg2)];
	*dst = l_val < r_val ? l_val : r_val;

	LEAVE(0);
}

int Long_sparce_table_dtor(struct Long_sparce_table *const obj) {
	assert(obj);

	FREE_ARR(obj->buffer, obj->size * obj->lg2);
	FREE_ARR(obj->lg2_arr + 1, obj->size);

	LEAVE(0);
}

#undef FINAL_CODE



int Tall_sparce_table_ctor(struct Tall_sparce_table *const __restrict dst, size_t const size, int const *const __restrict arr) {
	#define FINAL_CODE

	assert(dst);
	assert(arr);

	CALLOC_ARR(dst->lg2_arr, size);
	dst->lg2_arr -= 1;
	#undef FINAL_CODE
	#define FINAL_CODE			\
	FREE_ARR(dst->lg2_arr + 1, size);

	dst->lg2 = 1;
	while ((size_t)1 << dst->lg2 < size) {
		dst->lg2_arr[((size_t)1 << dst->lg2) + 1] = dst->lg2;
		dst->lg2 += 1;
	}
	dst->size = size;
	CALLOC_ARR(dst->buffer, dst->lg2 * size);
	#undef FINAL_CODE
	#define FINAL_CODE

	for (size_t i = 0; i < size; i++) {
		dst->buffer[dst->lg2 * i] = arr[i];
	}
	for (size_t prev_pw = 0; prev_pw + 1 < dst->lg2; prev_pw++) {
		for (size_t i = 0; i + ((size_t)1 << (prev_pw + 1)) <= size; i++) {
			int	l_val = dst->buffer[dst->lg2 * i + prev_pw],
				r_val = dst->buffer[dst->lg2 * (i + ((size_t)1 << prev_pw)) + prev_pw];
			dst->buffer[dst->lg2 * i + prev_pw + 1] = l_val < r_val ? l_val : r_val;
		}
	}

	for (size_t i = 2; i <= size; i++) {
		if (!dst->lg2_arr[i]) {
			dst->lg2_arr[i] = dst->lg2_arr[i - 1];
		}
	}

	LEAVE(0);

	#undef FINAL_CODE
}

#define FINAL_CODE

int Tall_sparce_table_get(struct Tall_sparce_table const *const __restrict obj, size_t const l, size_t const r, int *const __restrict dst) {
	assert(dst);
	assert(obj);

	size_t	lg2	= obj->lg2_arr[r - l];
	int	l_val	= obj->buffer[obj->lg2 * l + lg2],
		r_val	= obj->buffer[obj->lg2 * (r - ((size_t)1 << lg2)) + lg2];
	*dst = l_val < r_val ? l_val : r_val;

	LEAVE(0);
}

int Tall_sparce_table_dtor(struct Tall_sparce_table *const obj) {
	assert(obj);

	FREE_ARR(obj->buffer, obj->lg2 * obj->size);
	FREE_ARR(obj->lg2_arr + 1, obj->size);

	LEAVE(0);
}
