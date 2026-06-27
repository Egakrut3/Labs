#ifndef _STACK_HPP
#define _STACK_HPP

#include "Dynamic_array.hpp"
#include "Forward_list.hpp"



struct DA_stack {
	struct Dynamic_array arr;
};

int DA_stack_ctor(struct DA_stack *dest, size_t elem_size, assign_func_t elem_assign, free_func_t elem_free);

int DA_stack_push(struct DA_stack *restrict obj, void const *restrictval);
int DA_stack_top(struct DA_stack const *restrict obj, void *restrict dest);
int DA_stack_pop(struct DA_stack *obj);
size_t DA_stack_size(struct DA_stack const *obj);

int DA_stack_dtor(struct DA_stack *obj);



struct FL_stack {
	struct Forward_list list;
};

int FL_stack_ctor(struct FL_stack *dest, size_t elem_size, assign_func_t elem_assign, free_func_t elem_free);

int FL_stack_push(struct FL_stack *restrict obj, void const *restrict val);
int FL_stack_top(struct FL_stack const *restrict obj, void *restrict dest);
int FL_stack_pop(struct FL_stack *obj);
size_t FL_stack_size(struct FL_stack const *obj);

int FL_stack_dtor(struct FL_stack *obj);

#endif
