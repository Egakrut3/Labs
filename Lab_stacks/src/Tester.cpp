#include "Tester.hpp"

#include "Stack.hpp"
#include <time.h>

#define FINAL_CODE

static int int_assign(void *const restrict dst, void const *const restrict src) {
	assert(dst); assert(src);

	*(int *)dst = *(int const *)src;

	LEAVE(0);
}

static int int_free(void *const elem) {
	assert(elem);

	LEAVE(0);
}

#undef FINAL_CODE

static int DA_Tester(enum Test_type const test, FILE *const restrict output) {
	#define FINAL_CODE

	assert(output);

	typedef struct DA_stack stk_t;
	double avg_time = 0;
	#define CNT_RUNS ((size_t)3)
	for (size_t run_num = 0; run_num < CNT_RUNS; run_num++) {
		switch (test) {
			case TEST1:
				stk_t *restrict stk = nullptr;
				NEW(DA_stack, stk, sizeof(int), int_assign, int_free);
				#undef FINAL_CODE
				#define FINAL_CODE			\
				DELETE_UNCHECKED(DA_stack, stk);

				struct timespec	beg_tm = {},
						end_tm = {};
				if (clock_gettime(CLOCK_MONOTONIC, &beg_tm)) { LEAVE(errno); }

				for (size_t i = 0; i < 1'000'000; i++) {
					CHECK_PROC(DA_stack_push, stk, &(int){(int)i});
				}

				for (size_t it = 0; it < 9; it++) {
					size_t size = DA_stack_size(stk);

					for (size_t i = 0; i < size / 2; i++) {
						CHECK_PROC(DA_stack_pop, stk);
					}

					for (size_t i = 0; i < size / 4; i++) {
						CHECK_PROC(DA_stack_push, stk, &(int){(int)i});
					}
				}

				if (clock_gettime(CLOCK_MONOTONIC, &end_tm)) { LEAVE(errno); }

				avg_time +=	(double)end_tm.tv_sec + (double)end_tm.tv_nsec / 1'000'000'000 -
						(double)beg_tm.tv_sec - (double)beg_tm.tv_nsec / 1'000'000'000;

				#undef FINAL_CODE
				#define FINAL_CODE
				DELETE_CHECKED(DA_stack, stk);
				break;

			case TEST2:
			case TEST3:
			case TEST4:
				break;

			default:
				PRINT_LINE();
				abort();
		}
	}

	fprintf(output, "%g,", avg_time / 3);

	LEAVE(0);
	#undef FINAL_CODE
}

static int FL_Tester(enum Test_type const test, FILE *const restrict output) {
	#define FINAL_CODE

	assert(output);

	typedef struct FL_stack stk_t;
	double avg_time = 0;
	#define CNT_RUNS ((size_t)3)
	for (size_t run_num = 0; run_num < CNT_RUNS; run_num++) {
		switch (test) {
			case TEST1:
				stk_t *restrict stk = nullptr;
				NEW(FL_stack, stk, sizeof(int), int_assign, int_free);
				#undef FINAL_CODE
				#define FINAL_CODE			\
				DELETE_UNCHECKED(FL_stack, stk);

				struct timespec	beg_tm = {},
						end_tm = {};
				if (clock_gettime(CLOCK_MONOTONIC, &beg_tm)) { LEAVE(errno); }

				for (size_t i = 0; i < 1'000'000; i++) {
					CHECK_PROC(FL_stack_push, stk, &(int){(int)i});
				}

				for (size_t it = 0; it < 9; it++) {
					size_t size = FL_stack_size(stk);

					for (size_t i = 0; i < size / 2; i++) {
						CHECK_PROC(FL_stack_pop, stk);
					}

					for (size_t i = 0; i < size / 4; i++) {
						CHECK_PROC(FL_stack_push, stk, &(int){(int)i});
					}
				}

				if (clock_gettime(CLOCK_MONOTONIC, &end_tm)) { LEAVE(errno); }

				avg_time +=	(double)end_tm.tv_sec + (double)end_tm.tv_nsec / 1'000'000'000 -
						(double)beg_tm.tv_sec - (double)beg_tm.tv_nsec / 1'000'000'000;

				#undef FINAL_CODE
				#define FINAL_CODE
				DELETE_CHECKED(FL_stack, stk);
				break;

			case TEST2:
			case TEST3:
			case TEST4:
				break;

			default:
				PRINT_LINE();
				abort();
		}
	}

	fprintf(output, "%g,", avg_time / 3);

	LEAVE(0);
	#undef FINAL_CODE
}

#define FINAL_CODE

int Tester(enum Stack_type const stk, enum Test_type const test, FILE *const output) {
	switch (stk) {
		case DA:
			CHECK_PROC(DA_Tester, test, output);

			break;

		case FL:
			CHECK_PROC(FL_Tester, test, output);

			break;

		default:
			PRINT_LINE();
			abort();
	}

	LEAVE(0);
}

#undef FINAL_CODE
