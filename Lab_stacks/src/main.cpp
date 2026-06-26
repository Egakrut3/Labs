#include "Dynamic_array.hpp"

#define FINAL_CODE

static int int_assign(void *const dst, void const *src) {
	assert(dst); assert(src);

	*(int *)dst = *(int const *)src;

	LEAVE(0);
}

static int int_free(void *const elem) {
	assert(elem);

	LEAVE(0);
}

#undef FINAL_CODE

int main() {
	#define FINAL_CODE

	struct Dynamic_array *arr = nullptr;
	NEW(Dynamic_array, arr, sizeof(int), int_assign, int_free, 1);
	#undef FINAL_CODE
	#define FINAL_CODE			\
	DELETE_UNCHECKED(Dynamic_array, arr);

	CHECK_PROC(Dynamic_array_push_back, arr, &(int){10});
	CHECK_PROC(Dynamic_array_push_back, arr, &(int){100});
	CHECK_PROC(Dynamic_array_push_back, arr, &(int){1000});

	size_t size = Dynamic_array_size(arr);
	for (size_t i = 0; i < size; i++) {
		printf("%d\n", ((int *)(arr->begin))[i]);
	}
	printf("%zu %zu\n", Dynamic_array_size(arr), Dynamic_array_capacity(arr));

	LEAVE(0);

	#undef FINAL_CODE
}
