#include "Dynamic_array.hpp"

#define FINAL_CODE

int Dynamic_array_ctor(struct Dynamic_array *const dst, size_t const elem_size, assign_func_t const elem_assign, free_func_t const elem_free, size_t capacity) {
	assert(dst);

	dst->elem_size		= elem_size;
	dst->elem_assign	= elem_assign;
	dst->elem_free		= elem_free;

	CALLOC_ANY_ARR(dst->begin, capacity, elem_size);
	dst->end	= dst->begin;
	dst->buffer_end	= dst->begin + elem_size * capacity;

	LEAVE(0);
}

static int Dynamic_array_realloc(struct Dynamic_array *const obj, size_t new_capacity) {
	assert(obj);

	ptrdiff_t const byte_size = obj->end - obj->begin;
	REALLOC_ANY_ARR(obj->begin, new_capacity, obj->elem_size);	// We assume trivially relocatable type
	obj->end	= obj->begin + byte_size;
	obj->buffer_end	= obj->begin + obj->elem_size * new_capacity;

	LEAVE(0);
}

int Dynamic_array_push_back(struct Dynamic_array *const obj, void const *const val) {
	assert(obj);

	if (obj->end == obj->buffer_end) { CHECK_PROC(Dynamic_array_realloc, obj, Dynamic_array_capacity(obj) * 2); }
	CHECK_PROC(obj->elem_assign, obj->end, val);
	obj->end += obj->elem_size;

	LEAVE(0);
}

int Dynamic_array_pop_back(struct Dynamic_array *const obj) {
	assert(obj);

	if (obj->end == obj->begin) { LEAVE(EACCESS_NON_EXISTENT); }
	obj->end -= obj->elem_size;
	CHECK_PROC(obj->elem_free, obj->end);

	LEAVE(0);
}

int Dynamic_array_dtor(struct Dynamic_array *const obj) {
	assert(obj);

	for (void *ptr = obj->begin; ptr != obj->end; ptr += obj->elem_size) {
		CHECK_PROC(obj->elem_free, ptr);
	}
	FREE_ANY_ARR(obj->begin, Dynamic_array_capacity(obj), obj->elem_size);

	LEAVE(0);
}

#undef FINAL_CODE

size_t Dynamic_array_size(struct Dynamic_array const *const obj) {
	assert(obj);

	return (size_t)(obj->end - obj->begin) / obj->elem_size;
}

size_t Dynamic_array_capacity(struct Dynamic_array const *const obj) {
	assert(obj);

	return (size_t)(obj->buffer_end - obj->begin) / obj->elem_size;
}
