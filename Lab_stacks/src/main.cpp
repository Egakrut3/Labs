#include "Dynamic_array.hpp"
#include "Forward_list.hpp"

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

	struct Forward_list *arr = nullptr;
	NEW(Forward_list, arr, sizeof(int), int_assign, int_free);
	#undef FINAL_CODE
	#define FINAL_CODE			\
	DELETE_UNCHECKED(Forward_list, arr);

	CHECK_PROC(Forward_list_insert_after, arr, arr->head, &(int){10});
	CHECK_PROC(Forward_list_insert_after, arr, arr->head, &(int){100});
	CHECK_PROC(Forward_list_insert_after, arr, arr->head, &(int){1000});

	for (struct Forward_list_node *cur = arr->head->next; cur != arr->head; cur = cur->next) {
		printf("%d\n", *(int *)cur->data);
	}
	printf("%zu\n", Forward_list_size(arr));

	CHECK_PROC(Forward_list_erase_after, arr, arr->head->next);
	for (struct Forward_list_node *cur = arr->head->next; cur != arr->head; cur = cur->next) {
		printf("%d\n", *(int *)cur->data);
	}
	printf("%zu\n", Forward_list_size(arr));

	CHECK_PROC(Forward_list_erase_after, arr, arr->head->next);
	for (struct Forward_list_node *cur = arr->head->next; cur != arr->head; cur = cur->next) {
		printf("%d\n", *(int *)cur->data);
	}
	printf("%zu\n", Forward_list_size(arr));

	LEAVE(0);

	#undef FINAL_CODE
}
