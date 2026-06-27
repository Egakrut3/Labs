#include "Stack.hpp"

#define FINAL_CODE

int DA_stack_ctor(struct DA_stack *const dest, size_t const elem_size, assign_func_t const elem_assign, free_func_t const elem_free) {
	assert(dest);

	CHECK_PROC(Dynamic_array_ctor, &dest->arr, elem_size, elem_assign, elem_free, 1);

	LEAVE(0);
}

int DA_stack_push(struct DA_stack *const restrict obj, void const *const restrict val) {
	assert(obj);

	CHECK_PROC(Dynamic_array_push_back, &obj->arr, val);

	LEAVE(0);
}

int DA_stack_top(struct DA_stack const *const restrict obj, void *const restrict dest) {
	assert(obj);

	if (!DA_stack_size(obj)) { LEAVE(EACCESS_NON_EXISTENT); }
	CHECK_PROC(obj->arr.elem_assign, dest, obj->arr.end - obj->arr.elem_size);

	LEAVE(0);
}

int DA_stack_pop(struct DA_stack *obj) {
	assert(obj);

	CHECK_PROC(Dynamic_array_pop_back, &obj->arr);

	LEAVE(0);
}

int DA_stack_dtor(struct DA_stack *obj) {
	assert(obj);

	CHECK_PROC(Dynamic_array_dtor, &obj->arr);

	LEAVE(0);
}



int FL_stack_ctor(struct FL_stack *const dest, size_t const elem_size, assign_func_t const elem_assign, free_func_t const elem_free) {
	assert(dest);

	CHECK_PROC(Forward_list_ctor, &dest->list, elem_size, elem_assign, elem_free);

	LEAVE(0);
}

int FL_stack_push(struct FL_stack *const restrict obj, void const *const restrict val) {
	assert(obj);

	CHECK_PROC(Forward_list_insert_after, &obj->list, obj->list.head, val);

	LEAVE(0);
}

int FL_stack_top(struct FL_stack const *const restrict obj, void *const restrict dest) {
	assert(obj);
	
	if (!FL_stack_size(obj)) { LEAVE(EACCESS_NON_EXISTENT); }



	assert(obj->list.head); assert(obj->list.head->next);

	CHECK_PROC(obj->list.elem_assign, dest, obj->list.head->next->data);



	LEAVE(0);
}

int FL_stack_pop(struct FL_stack *obj) {
	assert(obj);

	CHECK_PROC(Forward_list_erase_after, &obj->list, obj->list.head);

	LEAVE(0);
}

int FL_stack_dtor(struct FL_stack *obj) {
	assert(obj);

	CHECK_PROC(Forward_list_dtor, &obj->list);

	LEAVE(0);
}

#undef FINAL_CODE

size_t DA_stack_size(struct DA_stack const *const obj) {
	assert(obj);

	return Dynamic_array_size(&obj->arr);
}

size_t FL_stack_size(struct FL_stack const *const obj) {
	assert(obj);

	return Forward_list_size(&obj->list);
}
