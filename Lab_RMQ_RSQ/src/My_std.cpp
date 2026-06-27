#include "My_std.hpp"

#include <assert.h>
#include <errno.h>
#include <stdlib.h>

int my_malloc(void *__restrict *const __restrict dst, size_t const size) {
	assert(dst);

	void *const __restrict ptr = malloc(size);
	if (!ptr) { return errno; }
	*dst = ptr;
	return 0;
}

int my_calloc(void *__restrict *const __restrict dst, size_t const n, size_t const size) {
	assert(dst);

	void *const __restrict ptr = calloc(n, size);
	if (!ptr) { return errno; }
	*dst = ptr;
	return 0;
}

int my_reallocarray(void *__restrict *const __restrict dst, void *const __restrict buffer, size_t const n, size_t const size) {
	assert(dst);

	void *const __restrict ptr = reallocarray(buffer, n, size);
	if (!ptr) { return errno; }
	*dst = ptr;
	return 0;
}
