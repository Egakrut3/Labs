#ifndef BENCH_COMMON
#define BENCH_COMMON

#include "Common.hpp"

enum Data_type : byte_t {
	RANDOM_DATA,
	SORTED_DATA,
};

enum Operation_type : byte_t {
	INSERT_OPERATION,
	ERASE_OPERATION,
};

static inline uint32_t bench_next_rand(uint32_t *const state) {
	assert(state);

	*state = *state * 1103515245u + 12345u;
	return *state;
}

static inline int parse_data_type(char const *const str, enum Data_type *const type) {
	assert(str); assert(type);

	if (!strcmp(str, "random"))	{ *type = RANDOM_DATA; return 0; }
	if (!strcmp(str, "sorted"))	{ *type = SORTED_DATA; return 0; }

	PRINT_LINE();
	fprintf(stderr, "Unknown data type: %s\n", str);
	return 1;
}

static inline int parse_operation_type(char const *const str, enum Operation_type *const operation) {
	assert(str); assert(operation);

	if (!strcmp(str, "insert"))	{ *operation = INSERT_OPERATION; return 0; }
	if (!strcmp(str, "erase"))	{ *operation = ERASE_OPERATION; return 0; }

	PRINT_LINE();
	fprintf(stderr, "Unknown operation: %s\n", str);
	return 1;
}

static inline int gen_test_array(int *const arr, size_t const size, enum Data_type const type) {
	assert(arr);

	for (size_t i = 0; i < size; i++) {
		arr[i] = (int)i;
	}

	if (type == SORTED_DATA) { return 0; }

	uint32_t state = 0xC0FFEEu + (uint32_t)size;
	for (size_t i = size; i > 1; i--) {
		size_t const swap_ind = (size_t)(bench_next_rand(&state) % (uint32_t)i);
		int const tmp = arr[i - 1];

		arr[i - 1]	= arr[swap_ind];
		arr[swap_ind]	= tmp;
	}

	return 0;
}

static inline int partial_shuffle_arr(int *const arr, size_t const size, size_t const part_size) {
	assert(arr);
	assert(part_size <= size);

	uint32_t state = 0xBAD5EEDu + (uint32_t)size;
	for (size_t i = 0; i < part_size; i++) {
		size_t const swap_ind = i + (size_t)(bench_next_rand(&state) % (uint32_t)(size - i));
		int const tmp = arr[i];

		arr[i]		= arr[swap_ind];
		arr[swap_ind]	= tmp;
	}

	return 0;
}

#endif
