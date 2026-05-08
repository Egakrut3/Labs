.PHONY: all clean

all:
	@echo Please, run "make" from one of the subdirectories
.DEFAULT_GOAL = all

MERGED_BRANCH_DIR = 
clean:
	$(foreach dir,$(MERGED_BRANCH_DIR),$(eval @$(MAKE) -C $(dir) clean))
