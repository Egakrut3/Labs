#include "Tester.hpp"

int main(int const argc, char const *const __restrict argv[]) {
	#define FINAL_CODE

	assert(argc == 3); assert(argv);
	assert(argv[1]);

	size_t	ins_res_size		= 0;
	char	*ins_res_buf 		= nullptr;
	FILE	*const __restrict ins_output	= open_memstream(&ins_res_buf, &ins_res_size);
	if (!ins_output) { LEAVE(errno); }
	#undef FINAL_CODE
	#define FINAL_CODE			\
	fclose(ins_output);			\
	FREE_ARR(ins_res_buf, ins_res_size);

	size_t	ers_res_size		= 0;
	char	*ers_res_buf 		= nullptr;
	FILE	*const __restrict ers_output	= open_memstream(&ers_res_buf, &ers_res_size);
	if (!ers_output) { LEAVE(errno); }
	#undef FINAL_CODE
	#define FINAL_CODE			\
	fclose(ers_output);			\
	FREE_ARR(ers_res_buf, ers_res_size);	\
	fclose(ins_output);			\
	FREE_ARR(ins_res_buf, ins_res_size);

	CHECK_PROC(Tester, (enum Test_type)(argv[1][0] - '0'), ins_output, ers_output);
	#undef FINAL_CODE
	#define FINAL_CODE			\
	FREE_ARR(ers_res_buf, ers_res_size);	\
	fclose(ins_output);			\
	FREE_ARR(ins_res_buf, ins_res_size);
	if (fclose(ers_output)) { LEAVE(errno); }

	#undef FINAL_CODE
	#define FINAL_CODE			\
	FREE_ARR(ers_res_buf, ers_res_size);	\
	FREE_ARR(ins_res_buf, ins_res_size);
	if (fclose(ins_output)) { LEAVE(errno); }

	FILE *const __restrict output = fopen(argv[2], "w");
	if (!output) { LEAVE(errno); }
	#undef FINAL_CODE
	#define FINAL_CODE			\
	fclose(output);				\
	FREE_ARR(ers_res_buf, ers_res_size);	\
	FREE_ARR(ins_res_buf, ins_res_size);

	ins_res_buf[ins_res_size] = '\n';
	ins_res_size += 1;
	if (fwrite(ins_res_buf, sizeof(char), ins_res_size, output) < ins_res_size) { LEAVE(errno); }
	if (fwrite(ers_res_buf, sizeof(char), ers_res_size, output) < ers_res_size) { LEAVE(errno); }

	#undef FINAL_CODE
	#define FINAL_CODE			\
	FREE_ARR(ers_res_buf, ers_res_size);	\
	FREE_ARR(ins_res_buf, ins_res_size);
	if (fclose(output)) { LEAVE(errno); }

	LEAVE(0);

	#undef FINAL_CODE
}
