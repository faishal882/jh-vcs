# Makefile

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++17 -Wall -I./init -I./commit

# Linker flags
LDFLAGS = -lz -lssl -lcrypto

# Source files
SRCS = main.cpp \
       ./init/init.cpp \
       ./commit/tree.cpp \
       ./commit/reset.cpp \
       ./commit/ls-tree.cpp \
       ./commit/hash-blob.cpp \
       ./commit/commit.cpp \
       ./commit/cat-file.cpp \
       ./commit/commit-logs.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Output executable
TARGET = main.elf

# Default target
all: $(TARGET)

# Rule to link the executable
$(TARGET): $(OBJS)
	$(CXX) -o $@ $(OBJS) $(LDFLAGS)

# Rule to compile each source file into an object file
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -f $(OBJS) 

