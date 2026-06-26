#include "Tester.hpp"

#include "Segment_tree.hpp"
#include "Sparce_table.hpp"
#include "Strong_fenwick_tree.hpp"
#include <time.h>



#define N ((size_t)1'000'000)

static void generate_segment(size_t *const __restrict l, size_t *const __restrict r) {
	*l = (size_t)random() % (N + 1);
	*r = (size_t)random() % N;
	if (*l > *r)	{ size_t tmp = *r; *r = *l; *l = tmp; }
	else		{ *r += 1; }
}

#define NRUNS ((size_t)5)

static int Tester1(FILE *const __restrict output) {
	#define FINAL_CODE

	assert(output);

	struct Query {
		size_t	fir,
			sec;
		byte_t	is_q;
	};

	double	fnw_time = 0,
		seg_time = 0;
	for (size_t it = 0; it < NRUNS; it++) {
		struct Strong_fenwick_tree *__restrict fnw = nullptr;
		NEW(Strong_fenwick_tree, fnw, N);
		#undef FINAL_CODE
		#define FINAL_CODE				\
		DELETE_UNCHECKED(Strong_fenwick_tree, fnw);

		struct Segment_tree *__restrict seg = nullptr;
		NEW(Segment_tree, seg, N);
		#undef FINAL_CODE
		#define FINAL_CODE				\
		DELETE_UNCHECKED(Segment_tree, seg);		\
		DELETE_UNCHECKED(Strong_fenwick_tree, fnw);

		struct Query *__restrict queries = nullptr;
		CALLOC_ARR(queries, N);
		#undef FINAL_CODE
		#define FINAL_CODE				\
		FREE_ARR(queries, N);				\
		DELETE_UNCHECKED(Segment_tree, seg);		\
		DELETE_UNCHECKED(Strong_fenwick_tree, fnw);

		for (size_t i = 0; i < N; i++) {
			int val = (int)random();
			CHECK_PROC(Strong_fenwick_tree_set, fnw, i, val);
			CHECK_PROC(Segment_tree_set, seg, i, val);

			queries[i].is_q = (byte_t)random() & 1;
			if (queries[i].is_q) {
				generate_segment(&queries[i].fir, &queries[i].sec);
			}
			else {
				queries[i].fir = (size_t)random() % N;
				queries[i].sec = (size_t)random();
			}
		}



		struct timespec	beg_tm = {},
				end_tm = {};
		long		result = 0;
		
		if (clock_gettime(CLOCK_MONOTONIC, &beg_tm)) { LEAVE(errno); }

		for (size_t i = 0; i < N; i++) {
			if (queries[i].is_q) {
				CHECK_PROC(Strong_fenwick_tree_get, fnw, queries[i].fir, queries[i].sec, &result);
			}
			else {
				CHECK_PROC(Strong_fenwick_tree_set, fnw, queries[i].fir, (int)queries[i].sec);
			}
		}

		if (clock_gettime(CLOCK_MONOTONIC, &end_tm)) { LEAVE(errno); }
		fnw_time +=	(double)end_tm.tv_sec + (double)end_tm.tv_nsec / 1'000'000'000 -
				(double)beg_tm.tv_sec - (double)beg_tm.tv_nsec / 1'000'000'000;



		if (clock_gettime(CLOCK_MONOTONIC, &beg_tm)) { LEAVE(errno); }

		for (size_t i = 0; i < N; i++) {
			if (queries[i].is_q) {
				CHECK_PROC(Segment_tree_get, seg, queries[i].fir, queries[i].sec, &result);
			}
			else {
				CHECK_PROC(Segment_tree_set, seg, queries[i].fir, (int)queries[i].sec);
			}
		}

		if (clock_gettime(CLOCK_MONOTONIC, &end_tm)) { LEAVE(errno); }
		seg_time +=	(double)end_tm.tv_sec + (double)end_tm.tv_nsec / 1'000'000'000 -
				(double)beg_tm.tv_sec - (double)beg_tm.tv_nsec / 1'000'000'000;

		

		FREE_ARR(queries, N);
		#undef FINAL_CODE
		#define FINAL_CODE				\
		DELETE_UNCHECKED(Strong_fenwick_tree, fnw);
		DELETE_CHECKED(Segment_tree, seg);

		#undef FINAL_CODE
		#define FINAL_CODE
		DELETE_CHECKED(Strong_fenwick_tree, fnw);
	}

	fprintf(output, "%g,%g,", fnw_time / NRUNS, seg_time / NRUNS);
	LEAVE(0);

	#undef FINAL_CODE
}

static int Tester2(FILE *const __restrict output) {
	#define FINAL_CODE

	assert(output);

	struct Query {
		size_t	fir,
			sec;
	};

	double	long_time = 0,
		tall_time = 0;
	for (size_t it = 0; it < NRUNS; it++) {
		int *__restrict arr = nullptr;
		CALLOC_ARR(arr, N);
		#undef FINAL_CODE
		#define FINAL_CODE	\
		FREE_ARR(arr, N);

		struct Query *__restrict queries = nullptr;
		CALLOC_ARR(queries, N);
		#undef FINAL_CODE
		#define FINAL_CODE	\
		FREE_ARR(queries, N);	\
		FREE_ARR(arr, N);

		for (size_t i = 0; i < N; i++) {
			arr[i] = (int)random();

			generate_segment(&queries[i].fir, &queries[i].sec);
			if (queries[i].fir >= queries[i].sec) {
				printf("%zu\n", i);
				abort();
			}
		}

		struct Long_sparce_table *__restrict l_sp = nullptr;
		NEW(Long_sparce_table, l_sp, N, arr);
		#undef FINAL_CODE
		#define FINAL_CODE				\
		DELETE_UNCHECKED(Long_sparce_table, l_sp);	\
		FREE_ARR(queries, N);				\
		FREE_ARR(arr, N);

		struct Tall_sparce_table *__restrict t_sp = nullptr;
		NEW(Tall_sparce_table, t_sp, N, arr);
		FREE_ARR(arr, N);
		#undef FINAL_CODE
		#define FINAL_CODE				\
		DELETE_UNCHECKED(Tall_sparce_table, t_sp);	\
		DELETE_UNCHECKED(Long_sparce_table, l_sp);	\
		FREE_ARR(queries, N);



		struct timespec	beg_tm = {},
				end_tm = {};
		int		result = 0;
		
		if (clock_gettime(CLOCK_MONOTONIC, &beg_tm)) { LEAVE(errno); }

		for (size_t i = 0; i < N; i++) {
			CHECK_PROC(Long_sparce_table_get, l_sp, queries[i].fir, queries[i].sec, &result);
		}

		if (clock_gettime(CLOCK_MONOTONIC, &end_tm)) { LEAVE(errno); }
		long_time +=	(double)end_tm.tv_sec + (double)end_tm.tv_nsec / 1'000'000'000 -
				(double)beg_tm.tv_sec - (double)beg_tm.tv_nsec / 1'000'000'000;



		if (clock_gettime(CLOCK_MONOTONIC, &beg_tm)) { LEAVE(errno); }

		for (size_t i = 0; i < N; i++) {
			CHECK_PROC(Tall_sparce_table_get, t_sp, queries[i].fir, queries[i].sec, &result);
		}

		if (clock_gettime(CLOCK_MONOTONIC, &end_tm)) { LEAVE(errno); }
		tall_time +=	(double)end_tm.tv_sec + (double)end_tm.tv_nsec / 1'000'000'000 -
				(double)beg_tm.tv_sec - (double)beg_tm.tv_nsec / 1'000'000'000;



		FREE_ARR(queries, N);
		#undef FINAL_CODE
		#define FINAL_CODE				\
		DELETE_UNCHECKED(Long_sparce_table, l_sp);
		DELETE_CHECKED(Tall_sparce_table, t_sp);

		#undef FINAL_CODE
		#define FINAL_CODE
		DELETE_CHECKED(Long_sparce_table, l_sp);
	}

	fprintf(output, "%g,%g,", long_time / NRUNS, tall_time / NRUNS);
	LEAVE(0);

	#undef FINAL_CODE
}

#define FINAL_CODE

int Tester(enum Test_type const test, FILE *const output) {
	switch (test) {
		case TEST1:
			CHECK_PROC(Tester1, output);

			break;

		case TEST2:
			CHECK_PROC(Tester2, output);

			break;

		default:
			PRINT_LINE();
			abort();
	}

	LEAVE(0);
}

#undef FINAL_CODE
