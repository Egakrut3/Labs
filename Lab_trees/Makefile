.PHONY: all bench graph smoke clean bench_all graph_all smoke_all clean_all bench_required graph_required bench_all_random graph_all_random

TREE_LIST	?= Naive_tree AVL_tree Treap Splay_tree RB_tree B_tree Skip_list
EXISTING_TREES	= $(foreach tree,$(TREE_LIST),$(if $(wildcard $(tree)/Makefile),$(tree),))

REQUIRED_SORTED_TREES =
ifneq ($(wildcard Naive_tree/Makefile),)
REQUIRED_SORTED_TREES += Naive_tree
endif
ifneq ($(wildcard AVL_tree/Makefile),)
REQUIRED_SORTED_TREES += AVL_tree
endif

TREE		?= Naive_tree
DATA_TYPE	?= random
OPERATION	?= all
INSERT_CNT	?=
HYPERFINE_RUNS	?= 5

TREE_DIR	= $(TREE)
COMMON_TREE_MAKE_ARGS	= HYPERFINE_RUNS=$(HYPERFINE_RUNS)
ifneq ($(INSERT_CNT),)
COMMON_TREE_MAKE_ARGS	+= INSERT_CNT=$(INSERT_CNT)
endif
TREE_MAKE_ARGS	= DATA_TYPE=$(DATA_TYPE) OPERATION=$(OPERATION) $(COMMON_TREE_MAKE_ARGS)

all:
	@echo Usage: make bench TREE=AVL_tree DATA_TYPE=random OPERATION=all
	@echo Usage: make bench_all DATA_TYPE=random OPERATION=all
	@echo Usage: make bench_required
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

bench_all: $(addprefix bench_,$(EXISTING_TREES))

graph_all: $(addprefix graph_,$(EXISTING_TREES))

smoke_all: $(addprefix smoke_,$(EXISTING_TREES))

clean_all: $(addprefix clean_,$(EXISTING_TREES))

bench_required: bench_all_random $(addprefix bench_sorted_,$(REQUIRED_SORTED_TREES))

graph_required: graph_all_random $(addprefix graph_sorted_,$(REQUIRED_SORTED_TREES))

bench_all_random:
	@$(MAKE) bench_all DATA_TYPE=random OPERATION=all $(COMMON_TREE_MAKE_ARGS)

graph_all_random:
	@$(MAKE) graph_all DATA_TYPE=random OPERATION=all $(COMMON_TREE_MAKE_ARGS)

bench_sorted_%:
	@$(MAKE) -C $* bench DATA_TYPE=sorted OPERATION=all $(COMMON_TREE_MAKE_ARGS)

graph_sorted_%:
	@$(MAKE) -C $* graph DATA_TYPE=sorted OPERATION=all $(COMMON_TREE_MAKE_ARGS)

bench_%:
	@$(MAKE) -C $* bench $(TREE_MAKE_ARGS)

graph_%:
	@$(MAKE) -C $* graph $(TREE_MAKE_ARGS)

smoke_%:
	@$(MAKE) -C $* smoke $(TREE_MAKE_ARGS)

clean_%:
	@$(MAKE) -C $* clean
