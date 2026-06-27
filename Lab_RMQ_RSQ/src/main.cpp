#include "Tester.hpp"
#include "Sparce_table.hpp"

int main(int const argc, char const *const __restrict argv[]) {
	#define FINAL_CODE

	assert(argc == 3); assert(argv);
	assert(argv[1]);

	size_t	res_size		= 0;
	char	*res_buf 		= nullptr;
	FILE	*__restrict output	= open_memstream(&res_buf, &res_size);
	if (!output) { LEAVE(errno); }
	#undef FINAL_CODE
	#define FINAL_CODE		\
	fclose(output);			\
	FREE_ARR(res_buf, res_size);

	CHECK_PROC(Tester, (enum Test_type)(argv[1][0] - '0'), output);
	fclose(output);
	#undef FINAL_CODE
	#define FINAL_CODE		\
	FREE_ARR(res_buf, res_size);

	output = fopen(argv[2], "w");
	if (!output) { LEAVE(errno); }
	#undef FINAL_CODE
	#define FINAL_CODE	\
	fclose(output);		\
	free(res_buf);

	if (fwrite(res_buf, sizeof(char), res_size, output) < res_size) { LEAVE(errno); }

	LEAVE(0);

	#undef FINAL_CODE
}
