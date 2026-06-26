#include "Stack.hpp"

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

	struct DA_stack *arr_stk = nullptr;
	NEW(DA_stack, arr_stk, sizeof(int), int_assign, int_free);
	#undef FINAL_CODE
	#define FINAL_CODE			\
	DELETE_UNCHECKED(DA_stack, arr_stk);

	struct FL_stack *list_stk = nullptr;
	NEW(FL_stack, list_stk, sizeof(int), int_assign, int_free);
	#undef FINAL_CODE
	#define FINAL_CODE			\
	DELETE_UNCHECKED(FL_stack, list_stk);	\
	DELETE_UNCHECKED(DA_stack, arr_stk);



	int	arr_top		= 0,
		list_top	= 0;

	CHECK_PROC(DA_stack_push, arr_stk, &(int){10});		CHECK_PROC(FL_stack_push, list_stk, &(int){10});
	CHECK_PROC(DA_stack_top, arr_stk, &arr_top);		CHECK_PROC(FL_stack_top, list_stk, &list_top);
	printf("%d %d\n", arr_top, list_top);

	CHECK_PROC(DA_stack_push, arr_stk, &(int){100});	CHECK_PROC(FL_stack_push, list_stk, &(int){100});
	CHECK_PROC(DA_stack_top, arr_stk, &arr_top);		CHECK_PROC(FL_stack_top, list_stk, &list_top);
	printf("%d %d\n", arr_top, list_top);

	CHECK_PROC(DA_stack_push, arr_stk, &(int){1000});	CHECK_PROC(FL_stack_push, list_stk, &(int){1000});
	CHECK_PROC(DA_stack_top, arr_stk, &arr_top);		CHECK_PROC(FL_stack_top, list_stk, &list_top);
	printf("%d %d\n", arr_top, list_top);

	CHECK_PROC(DA_stack_pop, arr_stk);			CHECK_PROC(FL_stack_pop, list_stk);
	CHECK_PROC(DA_stack_top, arr_stk, &arr_top);		CHECK_PROC(FL_stack_top, list_stk, &list_top);
	printf("%d %d\n", arr_top, list_top);

	CHECK_PROC(DA_stack_pop, arr_stk);			CHECK_PROC(FL_stack_pop, list_stk);
	CHECK_PROC(DA_stack_top, arr_stk, &arr_top);		CHECK_PROC(FL_stack_top, list_stk, &list_top);
	printf("%d %d\n", arr_top, list_top);



	LEAVE(0);

	#undef FINAL_CODE
}
