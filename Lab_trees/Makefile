SRC = My_std Naive_tree AVL_tree Treap_tree Tester main

COMMON_MAKEFILE_PATH = ../Common_Makefile.mk
include $(COMMON_MAKEFILE_PATH)

.PNONY: all_tests plot total

RESULTS_DIR = results/

prepare::
	@mkdir -p $(RESULTS_DIR)

$(RESULTS_DIR)Naive.csv: $(TARGET) | prepare
	@$(RUN_TARGET) 0 $@

$(RESULTS_DIR)Naive_srtd.csv: $(TARGET) | prepare
	@$(RUN_TARGET) 1 $@

$(RESULTS_DIR)AVL.csv: $(TARGET) | prepare
	@$(RUN_TARGET) 2 $@

$(RESULTS_DIR)AVL_srtd.csv: $(TARGET) | prepare
	@$(RUN_TARGET) 3 $@

$(RESULTS_DIR)Treap.csv: $(TARGET) | prepare
	@$(RUN_TARGET) 4 $@

all_tests:
	$(MAKE) $(RESULTS_DIR)Naive.csv
	$(MAKE) $(RESULTS_DIR)Naive_srtd.csv

	$(MAKE) $(RESULTS_DIR)AVL.csv
	$(MAKE) $(RESULTS_DIR)AVL_srtd.csv

	$(MAKE) $(RESULTS_DIR)Treap.csv

plot: $(RESULTS_DIR)AVL.csv $(RESULTS_DIR)Treap.csv
	@python src/plot_drawer.py

clean::
	@rm -rf $(RESULTS_DIR)
