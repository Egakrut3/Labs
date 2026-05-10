RELEASE		?= 0

ALLOW_CPP	?= 1


INC_DIR		?= inc/



SRC_DIR		?= src/
SRC_SUF		?= .cpp
make_src_path	?= $(addprefix $(SRC_DIR),$(addsuffix $(SRC_SUF),$(1)))



COMPILER	?= gcc
ifndef (OPTIONS)

FIXED_OPTIONS	+=	-pie -fPIE

ifeq ($(ALLOW_CPP),0)

FIXED_OPTIONS	+=	-std=c23

else

FIXED_OPTIONS	+=	-std=c++23

endif

ifeq ($(RELEASE),0)

FIXED_OPTIONS	+=	-Og -ggdb3

WARNINGS	+=	-Wall -Wextra -Waggressive-loop-optimizations -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconversion -Wempty-body -Wfloat-equal	\
			-Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wopenmp-simd -Wpacked -Wpointer-arith -Winit-self		\
			-Wredundant-decls -Wshadow -Wsign-conversion -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types		\
			-Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-missing-field-initializers		\
			-Wno-narrowing -Wno-varargs -Wstack-protector -Wlarger-than=8192 -Wstack-usage=8192 -Werror=vla

FEATURES	+=	-fcheck-new -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer								\
			-fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size	\
			-fsanitize=return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr

ifeq ($(ALLOW_CPP),1)

WARNINGS	+=	-Weffc++ -Wc++14-compat -Woverloaded-virtual -Wconditionally-supported -Wctor-dtor-privacy -Wnon-virtual-dtor -Wsign-promo -Wstrict-null-sentinel	\
			-Wsuggest-override -Wno-literal-suffix -Wno-old-style-cast

FEATURES	+=	-fsized-deallocation

endif

else

FIXED_OPTIONS	+=	-Ofast -DNDEBUG

endif

OPTIONS = $(FIXED_OPTIONS) $(WARNINGS) $(FEATURES)

endif



DEP_DIR		?= dep/
ifeq ($(RELEASE),0)

DEP_SUBDIR	?= $(addprefix $(DEP_DIR),debug/)

else

DEP_SUBDIR	?= $(addprefix $(DEP_DIR),release/)

endif
DEP_SUF		?= .mk
make_dep_path	?= $(addprefix $(DEP_SUBDIR),$(addsuffix $(DEP_SUF),$(1)))



BIN_DIR		?= bin/
ifeq ($(RELEASE),0)

BIN_SUBDIR	?= $(addprefix $(BIN_DIR),debug/)

else

BIN_SUBDIR	?= $(addprefix $(BIN_DIR),release/)

endif

OBJ_SUF		?= .o
make_obj_path	?= $(addprefix $(BIN_SUBDIR),$(addsuffix $(OBJ_SUF),$(1)))

EXEC_SUF	?= .elf
make_exec_path	?= $(addprefix $(BIN_SUBDIR),$(addsuffix $(EXEC_SUF),$(1)))

TARGET		?= $(call make_exec_path,Test)
RUN_TARGET	?= ./$(TARGET)



make_obj_and_dep_recipe ?=													\
$(call make_obj_path,$(1)) $(call make_dep_path,$(1)): $(call make_src_path,$(1)) | prepare;					\
	@$(COMPILER)	-c -o $(call make_obj_path,$(1))									\
			-MMD -MT $(call make_obj_path,$(1)) -MT $(call make_dep_path,$(1)) -MF $(call make_dep_path,$(1))	\
			$(OPTIONS) -I$(INC_DIR) $$<



ifeq ($(filter clean,$(MAKECMDGOALS)),)

include $(call make_dep_path,$(SRC))

endif
$(foreach src,$(SRC),$(eval $(call make_obj_and_dep_recipe, $(src))))
$(TARGET): $(call make_obj_path,$(SRC))



.PHONY: prepare all test clean

prepare::
	@mkdir -p $(DEP_SUBDIR) $(BIN_SUBDIR)

$(TARGET):
	@gcc $(OPTIONS) -o $@ $^
	@echo Compilation end

all: $(TARGET)
	@echo Build end
.DEFAULT_GOAL = all

test: $(TARGET)
	@$(RUN_TARGET)

clean::
	@rm -fr	$(DEP_DIR) $(BIN_DIR) .vscode
