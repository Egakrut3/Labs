#include "Tester.hpp"

#include "AVL_tree.hpp"
#include "Naive_tree.hpp"
#include <time.h>

#define NRUNS 5

static int int_cmp(void const *const a, void const *const b) {
	assert(a); assert(b);

	int	i_a = *(int const *)a,
		i_b = *(int const *)b;

	if (i_a < i_b)		{ return -1; }
	else if (i_a == i_b)	{ return  0; }
	else			{ return +1; }
}

#define TYPE Naive_tree
#include "Universal_tester.hpp"
#undef TYPE

static int Naive_tree_tester(enum Test_type const type, FILE *const __restrict insert_output, FILE *const __restrict erase_output) {
	#define FINAL_CODE

	if (type == NAIVE) {
		int *__restrict buffer = nullptr;
		CALLOC_ARR(buffer, 100'000);
		#undef FINAL_CODE
		#define FINAL_CODE		\
		FREE_ARR(buffer, 100'000);

		double	insert_time	= 0,
			erase_time	= 0;
		for (size_t it = 0; it < NRUNS; it++) {
			for (size_t i = 0; i < 100'000; i++) {
				buffer[i] = (int)random();
			}

			CHECK_PROC(Naive_tree_test_instance, 100'000, buffer, &insert_time, &erase_time);
		}
		fprintf(insert_output, "%g,", insert_time / NRUNS);
		fprintf(erase_output, "%g,", erase_time / NRUNS);

		#undef FINAL_CODE
		#define FINAL_CODE
		FREE_ARR(buffer, 100'000);

		LEAVE(0);
	}
	else {
		int *__restrict buffer = nullptr;
		CALLOC_ARR(buffer, 10'000);
		#undef FINAL_CODE
		#define FINAL_CODE		\
		FREE_ARR(buffer, 10'000);

		double	insert_time	= 0,
			erase_time	= 0;
		for (size_t it = 0; it < NRUNS; it++) {
			for (size_t i = 0; i < 10'000; i++) {
				buffer[i] = (int)random();
			}
			qsort(buffer, 10'000, sizeof(int), int_cmp);

			CHECK_PROC(Naive_tree_test_instance, 10'000, buffer, &insert_time, &erase_time);
		}
		fprintf(insert_output, "%g,", insert_time / NRUNS);
		fprintf(erase_output, "%g,", erase_time / NRUNS);

		#undef FINAL_CODE
		#define FINAL_CODE
		FREE_ARR(buffer, 10'000);

		LEAVE(0);
	}

	PRINT_LINE();
	abort();

	#undef FINAL_CODE
}

#define TYPE AVL_tree
#include "Universal_tester.hpp"
#undef TYPE

static int AVL_tester(enum Test_type const type, FILE *const __restrict insert_output, FILE *const __restrict erase_output) {
	#define FINAL_CODE

	if (type == AVL) {
		for (size_t size = 100'000; size <= 1'000'000; size += 100'000) {
			int *__restrict buffer = nullptr;
			CALLOC_ARR(buffer, size);
			#undef FINAL_CODE
			#define FINAL_CODE	\
			FREE_ARR(buffer, size);

			double	insert_time	= 0,
				erase_time	= 0;
			for (size_t it = 0; it < NRUNS; it++) {
				for (size_t i = 0; i < size; i++) {
					buffer[i] = (int)random();
				}

				CHECK_PROC(AVL_tree_test_instance, size, buffer, &insert_time, &erase_time);
			}
			fprintf(insert_output, "%g,", insert_time / NRUNS);
			fprintf(erase_output, "%g,", erase_time / NRUNS);

			#undef FINAL_CODE
			#define FINAL_CODE
			FREE_ARR(buffer, size);
		}

		LEAVE(0);
	}
	else if (type == AVL_SRTD) {
		int *__restrict buffer = nullptr;
		CALLOC_ARR(buffer, 1'000'000);
		#undef FINAL_CODE
		#define FINAL_CODE		\
		FREE_ARR(buffer, 1'000'000);

		double	insert_time	= 0,
			erase_time	= 0;
		for (size_t it = 0; it < NRUNS; it++) {
			for (size_t i = 0; i < 1'000'000; i++) {
				buffer[i] = (int)random();
			}
			qsort(buffer, 1'000'000, sizeof(int), int_cmp);

			CHECK_PROC(AVL_tree_test_instance, 1'000'000, buffer, &insert_time, &erase_time);
		}
		fprintf(insert_output, "%g,", insert_time / NRUNS);
		fprintf(erase_output, "%g,", erase_time / NRUNS);

		#undef FINAL_CODE
		#define FINAL_CODE
		FREE_ARR(buffer, 1'000'000);

		LEAVE(0);
	}

	PRINT_LINE();
	abort();

	#undef FINAL_CODE
}

#define FINAL_CODE

int Tester(enum Test_type const type, FILE *const __restrict insert_output, FILE *const __restrict erase_output) {
	switch (type) {
		case NAIVE:
		case NAIVE_SRTD:
			CHECK_PROC(Naive_tree_tester, type, insert_output, erase_output);
			break;

		case AVL:
		case AVL_SRTD:
			CHECK_PROC(AVL_tester, type, insert_output, erase_output);
			break;

		default:
			PRINT_LINE();
			abort();
	}

	LEAVE(0);
}

#undef FINAL_CODE
