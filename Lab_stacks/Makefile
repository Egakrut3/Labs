SRC = My_std Dynamic_array Forward_list Stack Tester main

ALLOW_CPP		= 0
COMMON_MAKEFILE_PATH	= ../Common_Makefile.mk
include $(COMMON_MAKEFILE_PATH)

prepare::
	@mkdir -p results/

all_tests: $(TARGET)
	@$(RUN_TARGET) 0 0 results/DA_1.csv
	@$(RUN_TARGET) 1 0 results/FL_1.csv

clean::
	@rm -rf results/
