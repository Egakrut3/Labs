RESULTS_DIR		?= results/
RAW_RESULTS_DIR		?= $(RESULTS_DIR)raw/
SUMMARY_RESULTS_DIR	?= $(RESULTS_DIR)summary/
GRAPH_RESULTS_DIR	?= $(RESULTS_DIR)graphs/

DATA_TYPE		?= random
OPERATION		?= all
INSERT_CNT		?= $(DEFAULT_INSERT_CNT)
HYPERFINE_WARMUPS	?= 1
HYPERFINE_RUNS		?= 5
PYTHON			?= python
GRAPH_ENGINE		?= ../../Graphix/graph_engine.py

ifeq ($(DATA_TYPE),sorted)

ACTIVE_COUNT_ARR	= $(SORTED_COUNT_ARR)

else

ACTIVE_COUNT_ARR	= $(COUNT_ARR)

endif

EXEC_NAME		?= $(TREE_NAME)
SRC			?= $(TREE_NAME) $(TREE_NAME)_test main
INC_DIRS		?= inc/ ../Common/inc/

COMMON_MAKE		= ../../Common_Makefile.mk
include $(COMMON_MAKE)

make_raw_path		= $(RAW_RESULTS_DIR)$(TREE_NAME)_$(DATA_TYPE)_$(1).json
make_summary_path	= $(SUMMARY_RESULTS_DIR)$(TREE_NAME)_$(DATA_TYPE)_$(1).csv
make_graph_path		= $(GRAPH_RESULTS_DIR)$(TREE_NAME)_$(DATA_TYPE)_$(1).svg

.PHONY: check_hyperfine bench bench_all bench_insert bench_erase graph graph_all graph_insert graph_erase smoke

prepare::
	@$(call make_dir,$(RAW_RESULTS_DIR))
	@$(call make_dir,$(SUMMARY_RESULTS_DIR))
	@$(call make_dir,$(GRAPH_RESULTS_DIR))

ifeq ($(OS),Windows_NT)

check_hyperfine:
	@where hyperfine >NUL 2>NUL || (echo hyperfine was not found in PATH. Please install hyperfine and try again. && exit 1)

else

check_hyperfine:
	@command -v hyperfine >/dev/null 2>&1 || (echo hyperfine was not found in PATH. Please install hyperfine and try again. && exit 1)

endif

bench: bench_$(OPERATION)

bench_all: bench_insert bench_erase

bench_insert: $(TARGET) check_hyperfine | prepare
	@hyperfine --warmup $(HYPERFINE_WARMUPS) --runs $(HYPERFINE_RUNS) --export-json $(call make_raw_path,insert) $(foreach cnt,$(ACTIVE_COUNT_ARR),"$(RUN_TARGET) $(cnt) $(DATA_TYPE) insert")

bench_erase: $(TARGET) check_hyperfine | prepare
	@hyperfine --warmup $(HYPERFINE_WARMUPS) --runs $(HYPERFINE_RUNS) --export-json $(call make_raw_path,erase) $(foreach cnt,$(ACTIVE_COUNT_ARR),"$(RUN_TARGET) $(cnt) $(DATA_TYPE) erase")

graph: graph_$(OPERATION)

graph_all: graph_insert graph_erase

graph_insert: | prepare
	@$(PYTHON) $(GRAPH_ENGINE) $(TREE_NAME)_insert=$(call make_raw_path,insert) -o $(call make_graph_path,insert) --summary-output $(call make_summary_path,insert) --title "$(TREE_NAME) $(DATA_TYPE) insert" --x-label "elements" --y-label "time, s"

graph_erase: | prepare
	@$(PYTHON) $(GRAPH_ENGINE) $(TREE_NAME)_erase=$(call make_raw_path,erase) -o $(call make_graph_path,erase) --summary-output $(call make_summary_path,erase) --title "$(TREE_NAME) $(DATA_TYPE) erase" --x-label "elements" --y-label "time, s"

smoke: $(TARGET)
	@$(RUN_TARGET) 100 random insert
	@$(RUN_TARGET) 100 random erase
	@$(RUN_TARGET) 1000 random insert
	@$(RUN_TARGET) 1000 random erase

clean::
	@$(call remove_dir,$(RESULTS_DIR))
