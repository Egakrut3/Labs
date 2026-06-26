#include "My_std.hpp"

#include <assert.h>
#include <errno.h>
#include <stdlib.h>

int my_malloc(void **const dst, size_t const size) {
	assert(dst);

	void *const ptr = malloc(size);
	if (!ptr) { return errno; }
	*dst = ptr;
	return 0;
}

int my_calloc(void **const dst, size_t const n, size_t const size) {
	assert(dst);

	void *const ptr = calloc(n, size);
	if (!ptr) { return errno; }
	*dst = ptr;
	return 0;
}

int my_reallocarray(void **const dst, void *const buffer, size_t const n, size_t const size) {
	assert(dst);

	void *const ptr = reallocarray(buffer, n, size);
	if (!ptr) { return errno; }
	*dst = ptr;
	return 0;
}
