#include "Forward_list.hpp"

#define FINAL_CODE

int Forward_list_ctor(struct Forward_list *const dst, size_t const elem_size, assign_func_t const elem_assign, free_func_t const elem_free) {
	assert(dst);

	dst->elem_size		= elem_size;
	dst->elem_assign	= elem_assign;
	dst->elem_free		= elem_free;

	ALLOC_ELEM(dst->head);
	dst->head->next = dst->head;
	dst->head->data = nullptr;

	dst->size = 0;

	LEAVE(0);
}

int Forward_list_insert_after(struct Forward_list *const obj, struct Forward_list_node *const where, void const *const val) {
	assert(obj);
	assert(where);

	struct Forward_list_node *const prev_next = where->next;
	ALLOC_ELEM(where->next);
	where->next->next = prev_next;
	ALLOC_ANY_ELEM(where->next->data, obj->elem_size);
	CHECK_PROC(obj->elem_assign, where->next->data, val);

	obj->size += 1;

	LEAVE(0);
}

int Forward_list_erase_after(struct Forward_list *const obj, struct Forward_list_node *const where) {
	assert(obj);
	assert(where);

	if (where->next == obj->head) { LEAVE(EERASE_AFTER_LAST); }
	struct Forward_list_node *const pos = where->next;
	where->next = pos->next;
	CHECK_PROC(obj->elem_free, pos->data);
	FREE_ANY_ELEM(pos->data, obj->elem_size);
	FREE_ELEM(pos);

	obj->size -= 1;

	LEAVE(0);
}

int Forward_list_dtor(struct Forward_list *const obj) {
	assert(obj);

	for (struct Forward_list_node *cur = obj->head->next, *next = cur->next; cur != obj->head; cur = next, next = cur->next) {
		assert(cur); assert(next);

		CHECK_PROC(obj->elem_free, cur->data);
		FREE_ANY_ELEM(cur->data, obj->elem_size);
		FREE_ELEM(cur);
	}
	FREE_ELEM(obj->head);

	LEAVE(0);
}

#undef FINAL_CODE

size_t Forward_list_size(struct Forward_list const *const obj) {
	assert(obj);

	return obj->size;
}
