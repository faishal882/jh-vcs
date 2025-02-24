# Makefile

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++17 -Wall -I./init -I./commit

# Linker flags
LDFLAGS = -lz -lssl -lcrypto

# Build directory
BUILD_DIR = build

# Source files
SRCS = main.cpp \
       ./init/init.cpp \
       ./commit/tree.cpp \
       ./commit/reset.cpp \
       ./commit/ls-tree.cpp \
       ./commit/hash-blob.cpp \
       ./commit/commit.cpp \
       ./commit/cat-file.cpp \
       ./commit/commit-logs.cpp \
       ./utils/remoteutils.cpp \

# Object files (with build directory prefix)
OBJS = $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SRCS))

# Output executable
TARGET = jh-vcs

# Default target
all: $(BUILD_DIR)/$(TARGET)

# Rule to compile each source file into an object file
$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)  # Create the directory structure for the object file
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to link the executable
$(BUILD_DIR)/$(TARGET): $(OBJS)
	$(CXX) -o $@ $(OBJS) $(LDFLAGS)

# Clean rule
clean:
	rm -rf $(BUILD_DIR)