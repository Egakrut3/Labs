SRC = My_std Dynamic_array Forward_list Stack Tester main

ALLOW_CPP		= 0
COMMON_MAKEFILE_PATH	= ../Common_Makefile.mk
include $(COMMON_MAKEFILE_PATH)

.PNONY: all_tests plot total

RESULTS_DIR = results/

prepare::
	@mkdir -p $(RESULTS_DIR)

$(RESULTS_DIR)DA_1.csv: $(TARGET) | prepare
	@$(RUN_TARGET) 0 0 $@

$(RESULTS_DIR)FL_1.csv: $(TARGET) | prepare
	@$(RUN_TARGET) 1 0 $@

$(RESULTS_DIR)DA_2.csv: $(TARGET) | prepare
	@$(RUN_TARGET) 0 1 $@

$(RESULTS_DIR)FL_2.csv: $(TARGET) | prepare
	@$(RUN_TARGET) 1 1 $@

$(RESULTS_DIR)DA_3.csv: $(TARGET) | prepare
	@$(RUN_TARGET) 0 2 $@

$(RESULTS_DIR)FL_3.csv: $(TARGET) | prepare
	@$(RUN_TARGET) 1 2 $@

$(RESULTS_DIR)DA_4.csv: $(TARGET) | prepare
	@$(RUN_TARGET) 0 3 $@

$(RESULTS_DIR)FL_4.csv: $(TARGET) | prepare
	@$(RUN_TARGET) 1 3 $@

all_tests:
	$(MAKE) $(RESULTS_DIR)DA_1.csv
	$(MAKE) $(RESULTS_DIR)FL_1.csv

	$(MAKE) $(RESULTS_DIR)DA_2.csv
	$(MAKE) $(RESULTS_DIR)FL_2.csv

	$(MAKE) $(RESULTS_DIR)DA_3.csv
	$(MAKE) $(RESULTS_DIR)FL_3.csv

	$(MAKE) $(RESULTS_DIR)DA_4.csv
	$(MAKE) $(RESULTS_DIR)FL_4.csv

plot: results/DA_4.csv results/FL_4.csv
	@python src/plot_drawer.py

total: all_tests plot

clean::
	@rm -rf results/
