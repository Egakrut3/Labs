#include "Fenwick_tree.hpp"

int main() {
	#define FINAL_CODE

	struct Fenwick_tree *fnw = nullptr;
	NEW(Fenwick_tree, fnw, 5);
	#undef FINAL_CODE
	#define FINAL_CODE			\
	DELETE_UNCHECKED(Fenwick_tree, fnw);

	long sum = 0;
	CHECK_PROC(Fenwick_tree_get, fnw, 0, 5, &sum);
	printf("%ld\n", sum);

	CHECK_PROC(Fenwick_tree_increase, fnw, 2, 7);
	CHECK_PROC(Fenwick_tree_get, fnw, 0, 5, &sum);
	printf("%ld\n", sum);

	CHECK_PROC(Fenwick_tree_increase, fnw, 4, 6);
	CHECK_PROC(Fenwick_tree_get, fnw, 2, 4, &sum);
	printf("%ld\n", sum);
	CHECK_PROC(Fenwick_tree_get, fnw, 2, 5, &sum);
	printf("%ld\n", sum);
	CHECK_PROC(Fenwick_tree_get, fnw, 3, 5, &sum);
	printf("%ld\n", sum);
	CHECK_PROC(Fenwick_tree_get, fnw, 3, 4, &sum);
	printf("%ld\n", sum);

	CHECK_PROC(Fenwick_tree_increase, fnw, 2, 7);
	CHECK_PROC(Fenwick_tree_get, fnw, 0, 5, &sum);
	printf("%ld\n", sum);

	LEAVE(0);

	#undef FINAL_CODE
}
