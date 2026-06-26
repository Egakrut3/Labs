SRC = My_std Strong_fenwick_tree Segment_tree Tester main

COMMON_MAKEFILE_PATH = ../Common_Makefile.mk
include $(COMMON_MAKEFILE_PATH)

.PNONY: all_tests

RESULTS_DIR = results/

prepare::
	@mkdir -p $(RESULTS_DIR)

$(RESULTS_DIR)Test1.csv: $(TARGET) | prepare
	@$(RUN_TARGET) 0 $@

$(RESULTS_DIR)Test2.csv: $(TARGET) | prepare
	@$(RUN_TARGET) 1 $@

all_tests:
	$(MAKE) $(RESULTS_DIR)Test1.csv
	$(MAKE) $(RESULTS_DIR)Test2.csv
