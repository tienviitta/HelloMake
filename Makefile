## ###############################################################
# Project specific part
#
# A recursively expanded variable: ‘=’
# Simply expanded variables are defined by lines using ‘:=’ or ‘::=’
#
#  ###############################################################

# target name
TARGET := hellomake

# object file and build directory
OBJ_DIR := ./obj
BUILD_DIR := ./build

# include directories
#~ INC_DIRS := \
	#~ ./inc
INC_DIRS = \
	-Iinc

# ld
LFLAGS = \
	-lPocoFoundation \
	-lPocoUtil \
	-L /usr/local/lib/

# sources
SRCS := \
	./src/main.cpp \
	./src/myapp.cpp

## ###############################################################

## ###############################################################
# General part
#  ###############################################################

# The value of the make variable VPATH specifies a list of directories
# that make should search. Most often, the directories are expected to
# contain prerequisite files that are not in the current directory; 
# however, make uses VPATH as a search list for both prerequisites and
# targets of rules.
# [https://www.gnu.org/software/make/manual/html_node/General-Search.html]
VPATH := $(dir $(SRCS))

# object files
OBJS := \
	$(patsubst %.cpp, $(OBJ_DIR)/%$(ARCH)$(DEBUG).o, $(notdir $(SRCS)))

# create object files
#~ $(OBJ_DIR)/%$(ARCH)$(DEBUG).o : %.cpp Makefile
	#~ @echo creating $@ ...
	#~ $(CXX) -I$(INC_DIRS) $(CFLAGS) $(EXTRA_CFLAGS) -c -o $@ $<
$(OBJ_DIR)/%$(ARCH)$(DEBUG).o : %.cpp Makefile
	@echo creating $@ ...
	$(CXX) $(INC_DIRS) $(CFLAGS) $(EXTRA_CFLAGS) -c -o $@ $<

# build target
$(BUILD_DIR)/$(TARGET): $(OBJS)
	@echo building output ...
	$(CXX) -o $(BUILD_DIR)/$(TARGET) $(OBJS) $(LFLAGS)

# clean
.PHONY: clean
clean:
	rm --force $(OBJ_DIR)/*.o
	rm --force $(BUILD_DIR)/$(TARGET)

## ###############################################################
