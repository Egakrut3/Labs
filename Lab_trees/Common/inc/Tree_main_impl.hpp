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
	assert(argv);
	if (argc != 4) {
		fprintf(stderr, "Usage: %s <insert_cnt> <random|sorted> <insert|erase>\n", argv[0]);
		return 1;
	}

	size_t insert_cnt = 0;
	if (sscanf(argv[1], "%zu", &insert_cnt) != 1) {
		fprintf(stderr, "Bad insert_cnt: %s\n", argv[1]);
		return 1;
	}

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
