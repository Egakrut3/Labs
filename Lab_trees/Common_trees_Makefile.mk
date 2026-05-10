RESULTS_DIR		?= results/
make_results_path	?= $(addprefix $(RESULTS_DIR),$(1))



DATA_TYPE	?= random
ifeq (DATA_TYPE,random)

__DATA_TYPE	= 0

else

__DATA_TYPE	= 1

endif



QUERIES_TYPE	?= insert
ifeq (QUERIES_TYPE,insert)

__QUERIES_TYPE	= 0

else

__QUERIES_TYPE	= 1

endif
QUERIES_CNT	?= 100000



SRC		?= $(addprefix $(TREE_PREF),_tree _tree_test) main



ISOL_CPU_NUM	?= 15
RUN_TARGET	?= taskset -c $(ISOL_CPU_NUM) ./$(TARGET) $(__DATA_TYPE) $(__QUERIES_TYPE) $(QUERIES_CNT)



include $(COMMON_MAKEFILE)



.PHONY: hyperfine_report

prepare::
	@mkdir -p $(RESULTS_DIR)

HYPERFINE_WARMUPS	?= 1
HYPERFINE_RUNS		?= 5
hyperfine_report: $(TARGET) | prepare
	@hyperfine --warmup $(HYPERFINE_WARMUPS) --runs $(HYPERFINE_RUNS) --export-json $(call make_results_path,$(DATA_TYPE)_$(QUERIES_TYPE)_$(QUERIES_CNT)_results.json) "$(RUN_TARGET)"

clean::
	@rm -rf $(RESULTS_DIR)
