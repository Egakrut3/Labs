.PHONY: all bench graph smoke clean

all:
	@echo Please, run "make" from Lab_trees or one of the tree subdirectories
.DEFAULT_GOAL = all

bench graph smoke clean:
	@$(MAKE) -C Lab_trees $@
