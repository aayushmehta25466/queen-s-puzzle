# Set include and library directories
INCLUDE_DIR = src/include
LIB_DIR = src/lib

# Compiler and flags
CXX = g++
CXXFLAGS = -I$(INCLUDE_DIR)
LDFLAGS = -L$(LIB_DIR) -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf

# Default target
all: help

# Rule to build a binary for any given C++ source file
%: %.c
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

# Help target to guide the user
help:
	@echo "Usage: make <filename (without .c)>"
	@echo "Example: make new"
