#ifndef TREE_TEST_IMPL
#define TREE_TEST_IMPL

#include "Bench_common.hpp"

#ifndef TREE_TEST_PREFIX
#error TREE_TEST_PREFIX must be defined before including Tree_test_impl.hpp
#endif

#ifndef TREE_TEST_STRUCT
#error TREE_TEST_STRUCT must be defined before including Tree_test_impl.hpp
#endif

#define TREE_JOIN_IMPL(prefix, suffix)	prefix##suffix
#define TREE_JOIN(prefix, suffix)	TREE_JOIN_IMPL(prefix, suffix)

typedef TREE_TEST_STRUCT tree_t;

int TREE_JOIN(TREE_TEST_PREFIX, _test)(size_t const insert_cnt, enum Data_type const data_type, enum Operation_type const operation) {
	#define FINAL_CODE

	tree_t tree = {};
	CHECK_PROC(TREE_JOIN(TREE_TEST_PREFIX, _ctor), &tree);
	#undef FINAL_CODE
	#define FINAL_CODE	\
	TREE_JOIN(TREE_TEST_PREFIX, _dtor)(&tree);

	int *arr = nullptr;
	ALLOC_ARR(arr, insert_cnt);
	#undef FINAL_CODE
	#define FINAL_CODE		\
	FREE_ARR(arr, insert_cnt);	\
	TREE_JOIN(TREE_TEST_PREFIX, _dtor)(&tree);

	CHECK_PROC(gen_test_array, arr, insert_cnt, data_type);

	for (size_t i = 0; i < insert_cnt; i++) {
		CHECK_PROC(TREE_JOIN(TREE_TEST_PREFIX, _insert), &tree, arr[i]);
	}

	if (operation == INSERT_OPERATION) {
		CLEAR_RESOURCES();
		return 0;
	}

	size_t const erase_cnt = insert_cnt / 2;
	CHECK_PROC(partial_shuffle_arr, arr, insert_cnt, erase_cnt);
	for (size_t i = 0; i < erase_cnt; i++) {
		CHECK_PROC(TREE_JOIN(TREE_TEST_PREFIX, _erase), &tree, arr[i]);
	}

	CLEAR_RESOURCES();
	return 0;

	#undef FINAL_CODE
}

#undef TREE_JOIN
#undef TREE_JOIN_IMPL
#undef TREE_TEST_PREFIX
#undef TREE_TEST_STRUCT
#undef TREE_TEST_IMPL

#endif
