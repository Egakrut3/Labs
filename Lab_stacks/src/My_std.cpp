#include "My_std.hpp"

#include <assert.h>
#include <errno.h>
#include <stdlib.h>

int my_malloc(void *restrict *const restrict dst, size_t const size) {
	assert(dst);

	void *const restrict ptr = malloc(size);
	if (!ptr) { return errno; }
	*dst = ptr;
	return 0;
}

int my_calloc(void *restrict *const restrict dst, size_t const n, size_t const size) {
	assert(dst);

	void *const restrict ptr = calloc(n, size);
	if (!ptr) { return errno; }
	*dst = ptr;
	return 0;
}

int my_reallocarray(void *restrict *const restrict dst, void *const restrict buffer, size_t const n, size_t const size) {
	assert(dst);

	void *const restrict ptr = reallocarray(buffer, n, size);
	if (!ptr) { return errno; }
	*dst = ptr;
	return 0;
}
