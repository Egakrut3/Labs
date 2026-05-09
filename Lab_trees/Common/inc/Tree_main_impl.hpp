#ifndef TREE_MAIN_IMPL
#define TREE_MAIN_IMPL

#include "Bench_common.hpp"

#ifndef TREE_TEST_PREFIX
#error TREE_TEST_PREFIX must be defined before including Tree_main_impl.hpp
#endif

#define TREE_JOIN_IMPL(prefix, suffix)	prefix##suffix
#define TREE_JOIN(prefix, suffix)	TREE_JOIN_IMPL(prefix, suffix)

#define FINAL_CODE

int main(int argc, char *argv[]) {
	assert(argc == 4); assert(argv);

	size_t insert_cnt = 0;
	sscanf(argv[1], "%zu", &insert_cnt);

	enum Data_type data_type = RANDOM_DATA;
	CHECK_PROC(parse_data_type, argv[2], &data_type);

	enum Operation_type operation = INSERT_OPERATION;
	CHECK_PROC(parse_operation_type, argv[3], &operation);

	CHECK_PROC(TREE_JOIN(TREE_TEST_PREFIX, _test), insert_cnt, data_type, operation);

	CLEAR_RESOURCES();
	return 0;
}

#undef FINAL_CODE
#undef TREE_JOIN
#undef TREE_JOIN_IMPL
#undef TREE_TEST_PREFIX
#undef TREE_MAIN_IMPL

#endif
