#ifndef _SPARCE_TABLE_HPP
#define _SPARCE_TABLE_HPP

#include "Common.hpp"



struct Long_sparce_table {
	size_t	lg2,
		size;
	int	*buffer;

	size_t *lg2_arr;
};

int Long_sparce_table_ctor(struct Long_sparce_table *__restrict dst, size_t size, int const *__restrict arr);

int Long_sparce_table_get(struct Long_sparce_table const *__restrict obj, size_t l, size_t r, int *__restrict dst);

int Long_sparce_table_dtor(struct Long_sparce_table *obj);



struct Tall_sparce_table {
	size_t	size,
		lg2;
	int	*buffer;

	size_t *lg2_arr;
};

int Tall_sparce_table_ctor(struct Tall_sparce_table *__restrict dst, size_t size, int const *__restrict arr);

int Tall_sparce_table_get(struct Tall_sparce_table const *__restrict obj, size_t l, size_t r, int *__restrict dst);

int Tall_sparce_table_dtor(struct Tall_sparce_table *obj);

#endif
