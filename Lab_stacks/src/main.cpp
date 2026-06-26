#include "Tester.hpp"

int main(int const argc, char const *const restrict argv[]) {
	#define FINAL_CODE

	assert(argc == 4);
	assert(argv[1]); assert(argv[2]);

	size_t	res_size		= 0;
	char	*res_buf	= nullptr;
	FILE	*restrict output	= open_memstream(&res_buf, &res_size);
	if (!output) { LEAVE(errno); }
	#undef FINAL_CODE
	#define FINAL_CODE	\
	fclose(output);		\
	free(res_buf);

	CHECK_PROC(Tester, argv[1][0] - '0', argv[2][0] - '0', output);
	fclose(output);
	#undef FINAL_CODE
	#define FINAL_CODE	\
	free(res_buf);

	output = fopen(argv[3], "w");
	if (!output) { LEAVE(errno); }
	#undef FINAL_CODE
	#define FINAL_CODE	\
	fclose(output);		\
	free(res_buf);

	if (fwrite(res_buf, sizeof(char), res_size, output) < res_size) { LEAVE(errno); }

	LEAVE(0);
}

#undef FINAL_CODE
