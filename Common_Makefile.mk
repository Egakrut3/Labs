RELEASE		?= 0

ALLOW_CPP	?= 1

C_COMPILER	?= gcc
CPP_COMPILER	?= g++
ifeq ($(ALLOW_CPP),0)

COMPILER	?= $(C_COMPILER)

else

COMPILER	?= $(CPP_COMPILER)

endif



make_shell_path	?= $(1)
make_dir	?= mkdir -p $(1)
remove_dir	?= rm -rf $(1)

ifeq ($(OS),Windows_NT)

make_shell_path	= $(subst /,\,$(patsubst %/,%,$(1)))
make_dir	= if not exist "$(call make_shell_path,$(1))" mkdir "$(call make_shell_path,$(1))"
remove_dir	= if exist "$(call make_shell_path,$(1))" rmdir /S /Q "$(call make_shell_path,$(1))"
EXEC_SUF	?= .exe

else

EXEC_SUF	?= .elf

endif



ifndef OPTIONS

ifeq ($(ALLOW_CPP),0)

FIXED_OPTIONS	+=	-std=c23

else

FIXED_OPTIONS	+=	-std=c++23

endif

ifeq ($(RELEASE),0)

FIXED_OPTIONS	+=	-Og	\
			-ggdb3

WARNINGS	+=	-Wall -Wextra -Wshadow -Wconversion -Wsign-conversion -Wcast-align -Wcast-qual	\
			-Wformat=2 -Wswitch-enum -Wundef -Wunused -Werror=vla

FEATURES	+=	-fcheck-new -fstack-protector -fstrict-overflow -fno-omit-frame-pointer

ifneq ($(OS),Windows_NT)

FEATURES	+=	-fsanitize=address,undefined

endif

else

FIXED_OPTIONS	+=	-O3	\
			-DNDEBUG

endif

OPTIONS = $(FIXED_OPTIONS) $(WARNINGS) $(FEATURES)

endif



INC_DIRS	?= inc/
INCLUDE_OPTIONS	?= $(addprefix -I,$(INC_DIRS))

SRC_DIR		?= src/
SRC_SUF		?= .cpp
make_src_path	?= $(addprefix $(SRC_DIR),$(addsuffix $(SRC_SUF),$(1)))

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

EXEC_NAME	?= Test
make_exec_path	?= $(addprefix $(BIN_SUBDIR),$(addsuffix $(EXEC_SUF),$(1)))
TARGET		?= $(call make_exec_path,$(EXEC_NAME))
RUN_TARGET	?= $(call make_shell_path,$(TARGET))

make_obj_rule	?= $(call make_obj_path,$(1)): $(call make_src_path,$(1)) | prepare;	\
	@$(COMPILER) -c $(OPTIONS) -MMD -MP -MF $(call make_dep_path,$(1)) -MT $$@ -MT $(call make_dep_path,$(1)) $(INCLUDE_OPTIONS) -o $$@ $$<



ifeq ($(filter clean,$(MAKECMDGOALS)),)

-include $(call make_dep_path,$(SRC))

endif
$(foreach src,$(SRC),$(eval $(call make_obj_rule,$(src))))
$(TARGET): $(call make_obj_path,$(SRC))



.PHONY: prepare all test clean

prepare::
	@$(call make_dir,$(DEP_SUBDIR))
	@$(call make_dir,$(BIN_SUBDIR))

$(TARGET):
	@$(COMPILER) $(OPTIONS) -o $@ $^
	@echo Compilation end

all: $(TARGET)
	@echo Build end
.DEFAULT_GOAL = all

test: $(TARGET)
	@$(RUN_TARGET)

clean::
	@$(call remove_dir,$(DEP_DIR))
	@$(call remove_dir,$(BIN_DIR))
	@$(call remove_dir,.vscode)
