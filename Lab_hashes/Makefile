SRC =

COMMON_MAKEFILE_PATH = ../Common_Makefile.mk
include $(COMMON_MAKEFILE_PATH)

RESULTS_DIR = results/

.PHONY: all_tests plot

prepare::
	@mkdir -p $(RESULTS_DIR)

GENERATOR = $(call make_exec_path,gen)
$(GENERATOR): $(call make_src_path,generator) | prepare
	@gcc -o $@ -x c -std=gnu23 -Ofast $<

$(RESULTS_DIR)hash_info_0.csv: $(GENERATOR) | prepare
	@./$(GENERATOR) 0 $@

$(RESULTS_DIR)hash_info_1.csv: $(GENERATOR) | prepare
	@./$(GENERATOR) 1 $@

$(RESULTS_DIR)hash_info_2.csv: $(GENERATOR) | prepare
	@./$(GENERATOR) 2 $@

$(RESULTS_DIR)hash_info_3.csv: $(GENERATOR) | prepare
	@./$(GENERATOR) 3 $@

$(RESULTS_DIR)hash_info_4.csv: $(GENERATOR) | prepare
	@./$(GENERATOR) 4 $@

$(RESULTS_DIR)hash_info_5.csv: $(GENERATOR) | prepare
	@./$(GENERATOR) 5 $@

$(RESULTS_DIR)hash_info_6.csv: $(GENERATOR) | prepare
	@./$(GENERATOR) 6 $@

$(RESULTS_DIR)hash_info_7.csv: $(GENERATOR) | prepare
	@./$(GENERATOR) 7 $@

$(RESULTS_DIR)hash_info_8.csv: $(GENERATOR) | prepare
	@./$(GENERATOR) 8 $@

$(RESULTS_DIR)hash_info_9.csv: $(GENERATOR) | prepare
	@./$(GENERATOR) 9 $@

$(RESULTS_DIR)hash_info_10.csv: $(GENERATOR) | prepare
	@./$(GENERATOR) 10 $@

$(RESULTS_DIR)hash_info_11.csv: $(GENERATOR) | prepare
	@./$(GENERATOR) 11 $@

all_tests:
	$(MAKE) $(RESULTS_DIR)hash_info_0.csv
	$(MAKE) $(RESULTS_DIR)hash_info_1.csv
	$(MAKE) $(RESULTS_DIR)hash_info_2.csv

	$(MAKE) $(RESULTS_DIR)hash_info_3.csv
	$(MAKE) $(RESULTS_DIR)hash_info_4.csv
	$(MAKE) $(RESULTS_DIR)hash_info_5.csv
	$(MAKE) $(RESULTS_DIR)hash_info_6.csv
	$(MAKE) $(RESULTS_DIR)hash_info_7.csv

	$(MAKE) $(RESULTS_DIR)hash_info_8.csv
	$(MAKE) $(RESULTS_DIR)hash_info_9.csv
	$(MAKE) $(RESULTS_DIR)hash_info_10.csv
	$(MAKE) $(RESULTS_DIR)hash_info_11.csv

plot: all_tests
	@python src/histogram_drawer.py

clean::
	@rm -rf $(RESULTS_DIR)
