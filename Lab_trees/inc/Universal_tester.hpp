static int CONCAT(TYPE, _test_instance)(size_t const size, int *const __restrict buffer, double *const __restrict insert_time, double *const __restrict erase_time) {
	#define FINAL_CODE

	assert(buffer);

	struct TYPE *__restrict obj = nullptr;
	NEW(TYPE, obj);
	#undef FINAL_CODE
	#define FINAL_CODE		\
	DELETE_UNCHECKED(TYPE, obj);



	struct timespec	beg_tm = {},
			end_tm = {};
	
	if (clock_gettime(CLOCK_MONOTONIC, &beg_tm)) { LEAVE(errno); }

	for (size_t i = 0; i < size; i++) {
		CHECK_PROC(CONCAT(TYPE, _insert), obj, buffer[i]);
	}

	if (clock_gettime(CLOCK_MONOTONIC, &end_tm)) { LEAVE(errno); }
	*insert_time +=	(double)end_tm.tv_sec + (double)end_tm.tv_nsec / 1'000'000'000 -
			(double)beg_tm.tv_sec - (double)beg_tm.tv_nsec / 1'000'000'000;

	for (size_t i = 0; i < size / 2; i++) {
		size_t const	ind = i + (size_t)random() % (size - i);
		int const	tmp = buffer[i];
		buffer[i]	= buffer[ind];
		buffer[ind]	= tmp;
	}

	if (clock_gettime(CLOCK_MONOTONIC, &beg_tm)) { LEAVE(errno); }

	for (size_t i = 0; i < size / 2; i++) {
		CHECK_PROC(CONCAT(TYPE, _erase), obj, buffer[i]);
	}

	if (clock_gettime(CLOCK_MONOTONIC, &end_tm)) { LEAVE(errno); }
	*erase_time +=	(double)end_tm.tv_sec + (double)end_tm.tv_nsec / 1'000'000'000 -
			(double)beg_tm.tv_sec - (double)beg_tm.tv_nsec / 1'000'000'000;

	LEAVE(0);

	#undef FINAL_CODE
}
