.PHONY: all bench graph smoke clean

TREE		?= Naive_tree
DATA_TYPE	?= random
OPERATION	?= all
INSERT_CNT	?=
HYPERFINE_RUNS	?= 5

TREE_DIR	= $(TREE)
TREE_MAKE_ARGS	= DATA_TYPE=$(DATA_TYPE) OPERATION=$(OPERATION) HYPERFINE_RUNS=$(HYPERFINE_RUNS)
ifneq ($(INSERT_CNT),)
TREE_MAKE_ARGS	+= INSERT_CNT=$(INSERT_CNT)
endif

all:
	@echo Usage: make bench TREE=AVL_tree DATA_TYPE=random OPERATION=all
	@echo Trees: Naive_tree AVL_tree Treap Splay_tree RB_tree B_tree Skip_list
.DEFAULT_GOAL = all

bench:
	@$(MAKE) -C $(TREE_DIR) bench $(TREE_MAKE_ARGS)

graph:
	@$(MAKE) -C $(TREE_DIR) graph $(TREE_MAKE_ARGS)

smoke:
	@$(MAKE) -C $(TREE_DIR) smoke $(TREE_MAKE_ARGS)

clean:
	@$(MAKE) -C $(TREE_DIR) clean
