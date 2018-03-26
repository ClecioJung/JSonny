# ----------------------------------------
# Project definitions
# ----------------------------------------

# Name of the project
EXEC = JSonny

# Folders
ODIR = .obj
DDIR = .deps
SDIR = src

# .c files
SRC = $(wildcard $(SDIR)/*.c)

# .h files
INC = $(wildcard $(SDIR)/*.h)

# .d files
DEPS = $(subst .c,.d,$(subst $(SDIR),$(DDIR),$(SRC)))

# Object files
OBJ = $(subst .c,.o,$(subst $(SDIR),$(ODIR),$(SRC)))

# ----------------------------------------
# Compiler and linker definitions
# ----------------------------------------

 # Compiler and linker
CC = gcc

# Flags for compiler
CFLAGS = -c -W -Wall -pedantic -std=c99

# ----------------------------------------
# Fomating macros
# ----------------------------------------

BOLD = \033[1m
NORMAL = \033[0m
RED = \033[0;31m
GREEN = \033[0;32m

# ----------------------------------------
# Compilation and linking rules
# ----------------------------------------

all: directories $(EXEC)

$(EXEC): $(OBJ)
	@ echo "${GREEN}Building binary: ${BOLD}$@${GREEN} using dependencies: ${BOLD}$^ ${NORMAL}"
	@ $(CC) $(filter %.c %.s %.o,$^) -o $@
	@ echo "${GREEN}Finished building binary: ${BOLD}$@ ${NORMAL}"

$(ODIR)/%.o: $(SDIR)/%.c
	@ echo "${GREEN}Building target: ${BOLD}$@${GREEN}, using dependencies: ${BOLD}$^ ${NORMAL}"
	@ $(CC) $(CFLAGS) $(filter %.c %.s %.o,$^) -o $@

-include $(DEPS)

$(DDIR)/%.d: $(SDIR)/%.c
	@ $(CC) $(CFLAGS) $< -MM -MT '$(ODIR)/$*.o $(DDIR)/$*.d' -MF $@

# ----------------------------------------
# Script rules
# ----------------------------------------

status:
	@ echo "${GREEN}Git project status:${NORMAL}"
	@ git status -s

log:
	@ echo "${GREEN}Git project log:${NORMAL}"
	@ git log --oneline --decorate --all --graph

commit-%:
	@ echo "${GREEN}Inserting new commit to git project:${NORMAL}"
	git add --all
	git commit -m "$(@:commit-%=%)"
	git push origin master

directories:
	@ mkdir -p $(DDIR)
	@ mkdir -p $(ODIR)

run:
	@ echo "${GREEN}Testing the aplication:${NORMAL}"
	./$(EXEC) test/test01.js

memcheck:
	valgrind --tool=memcheck ./$(EXEC) test/test01.js

clean:
	@ rm -fr $(ODIR)/ $(DDIR)/ $(EXEC) *~ env.mk

remade:
	@ $(MAKE) clean
	@ $(MAKE) all

.PHONY: all status log commit-% directories run memcheck clean remade

# ----------------------------------------
