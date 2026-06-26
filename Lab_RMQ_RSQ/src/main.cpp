#include "Fenwick_tree.hpp"
#include "Segment_tree.hpp"

int main() {
	#define FINAL_CODE

	struct Segment_tree *tree = nullptr;
	NEW(Segment_tree, tree, 5);
	#undef FINAL_CODE
	#define FINAL_CODE			\
	DELETE_UNCHECKED(Segment_tree, tree);

	long sum = 0;
	CHECK_PROC(Segment_tree_get, tree, 0, 5, &sum);
	printf("%ld\n", sum);

	CHECK_PROC(Segment_tree_set, tree, 2, 7);
	CHECK_PROC(Segment_tree_get, tree, 0, 5, &sum);
	printf("%ld\n", sum);

	CHECK_PROC(Segment_tree_set, tree, 4, 6);
	CHECK_PROC(Segment_tree_get, tree, 2, 4, &sum);
	printf("%ld\n", sum);
	CHECK_PROC(Segment_tree_get, tree, 2, 5, &sum);
	printf("%ld\n", sum);
	CHECK_PROC(Segment_tree_get, tree, 3, 5, &sum);
	printf("%ld\n", sum);
	CHECK_PROC(Segment_tree_get, tree, 3, 4, &sum);
	printf("%ld\n", sum);

	CHECK_PROC(Segment_tree_set, tree, 2, 100);
	CHECK_PROC(Segment_tree_get, tree, 0, 5, &sum);
	printf("%ld\n", sum);

	LEAVE(0);

	#undef FINAL_CODE
}
