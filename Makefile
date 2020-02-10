# Based on http://make.mad-scientist.net/papers/advanced-auto-dependency-generation/
# https://gist.github.com/maxtruxa/4b3929e118914ccef057f8a05c614b0f
# https://spin.atomicobject.com/2016/08/26/makefile-c-projects/
# Git version: https://stackoverflow.com/questions/1704907/how-can-i-get-my-c-code-to-automatically-print-out-its-git-version-hash

# ----------------------------------------
# Project definitions
# ----------------------------------------

# Name of the project
EXEC = JSonny

# Folders
ODIR = .obj
DDIR = .deps
SDIR = src
IDIR = src

# .c files
SRCS = $(wildcard $(SDIR)/*.c $(SDIR)/*.cpp)

# .h files
INCS = $(wildcard $(IDIR)/*.h $(IDIR)/*.hpp)

# Dependency files (auto generated)
DEPS = $(patsubst %,%.d,$(basename $(subst $(SDIR),$(DDIR),$(SRCS))))

# Object files
OBJS = $(patsubst %,%.o,$(basename $(subst $(SDIR),$(ODIR),$(SRCS))))

# ----------------------------------------
# Compiler and linker definitions
# ----------------------------------------

 # Compiler and linker
CC = gcc
CXX = g++

# Libraries
INCLUDES =

# GIT version
GIT_VERSION := "$(shell git describe --abbrev=4 --dirty --always --tags)"

# Flags for compiler
CFLAGS = -W -Wall -Wextra -pedantic -std=c99 -DVERSION=\"$(GIT_VERSION)\"
CXXFLAGS = -W -Wall -Wextra -pedantic -DVERSION=\"$(GIT_VERSION)\"
DEPFLAGS = -MT $@ -MMD -MP -MF $(DDIR)/$*.Td

# Compiler macros
COMPILE.CC = $(CC) $(CFLAGS)
COMPILE.CXX = $(CXX) $(CXXFLAGS)
POSTCOMPILE = mv -f $(DDIR)/$*.Td $(DDIR)/$*.d && touch $@

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

all: $(EXEC)

$(EXEC): $(OBJS)
	@ echo "${GREEN}Building binary: ${BOLD}$@${GREEN} using dependencies: ${BOLD}$^${NORMAL}"
	$(COMPILE.CC) $(filter %.s %.o,$^) -o $@ $(INCLUDES)
	@ touch $@

$(ODIR)/%.o : $(SDIR)/%.c
$(ODIR)/%.o : $(SDIR)/%.c $(DDIR)/%.d | $(DDIR) $(ODIR)
	@ echo "${GREEN}Building target: ${BOLD}$@${GREEN}, using dependencies: ${BOLD}$^${NORMAL}"
	$(COMPILE.CC) $(DEPFLAGS) -c $(filter %.c %.s %.o,$^) -o $@
	@ $(POSTCOMPILE)

$(ODIR)/%.o : $(SDIR)/%.cpp
$(ODIR)/%.o : $(SDIR)/%.cpp $(DDIR)/%.d | $(DDIR) $(ODIR)
		@ echo "${GREEN}Building target: ${BOLD}$@${GREEN}, using dependencies: ${BOLD}$^${NORMAL}"
		$(COMPILE.CXX) $(DEPFLAGS) -c $(filter %.cpp %.s %.o,$^) -o $@
		@ $(POSTCOMPILE)

$(DDIR)/%.d: ;
.PRECIOUS: $(DDIR)/%.d

-include $(DEPS)

# ----------------------------------------
# Script rules
# ----------------------------------------

status:
	@ echo "${GREEN}Git project status:${NORMAL}"
	git status -s

log:
	@ echo "${GREEN}Git project log:${NORMAL}"
	git log --oneline --decorate --all --graph

commit-%:
	@ echo "${GREEN}Inserting new commit to git project:${NORMAL}"
	git add --all
	git commit -m "$(@:commit-%=%)"
	git push origin master

$(DDIR):
	mkdir -p $@

$(ODIR):
	mkdir -p $@

run:
	@ echo "${GREEN}Testing the aplication:${NORMAL}"
	./$(EXEC) test/test01.js

memcheck:
	valgrind --tool=memcheck ./$(EXEC) test/test01.js

clean:
	rm -fr $(ODIR)/ $(DDIR)/ $(EXEC) *~ env.mk

remade: clean all

.PHONY: all status log commit-% run memcheck clean remade

# ----------------------------------------
