#ifndef _FORWARD_LIST_HPP
#define _FORWARD_LIST_HPP

#include "Common.hpp"

typedef int (*assign_func_t)(void *dst, void const *src);
typedef int (*free_func_t)(void *elem);

struct Forward_list_node {
	struct Forward_list_node *next;

	void	*data;
};

struct Forward_list {
	size_t		elem_size;
	assign_func_t	elem_assign;
	free_func_t	elem_free;

	struct Forward_list_node *head;

	size_t size;
};

int Forward_list_ctor(struct Forward_list *dst, size_t elem_size, assign_func_t elem_assign, free_func_t elem_free);

int Forward_list_insert_after(struct Forward_list *obj, struct Forward_list_node *where, void const *val);
int Forward_list_erase_after(struct Forward_list *obj, struct Forward_list_node *where);

size_t Forward_list_size(struct Forward_list const *obj);

int Forward_list_dtor(struct Forward_list *obj);

#define	EACCESS_NON_EXISTENT	0x100

#endif
