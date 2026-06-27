#include "Universal_tester.hpp"

static int CONCAT(TYPE, _tester)(FILE *const __restrict insert_output, FILE *const __restrict erase_output) {
	#define FINAL_CODE

	assert(insert_output); assert(erase_output);

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

			CHECK_PROC(CONCAT(TYPE, _test_instance), size, buffer, &insert_time, &erase_time);
		}
		fprintf(insert_output, "%g,", insert_time / NRUNS);
		fprintf(erase_output, "%g,", erase_time / NRUNS);

		#undef FINAL_CODE
		#define FINAL_CODE
		FREE_ARR(buffer, size);
	}

	LEAVE(0);

	#undef FINAL_CODE
}
