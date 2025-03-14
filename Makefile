# Compiler
CXX = g++

# Compiler flags (use C++17)
CXXFLAGS = -Wall -std=c++17

# Source files
SRCS = main.cpp game.cpp utils.cpp credits.cpp chapters/chapter1.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Detect the operating system
ifeq ($(OS),Windows_NT)
    # Windows
    RM = del /f
    EXE = game.exe
    OBJS_WIN = $(subst /,\,$(OBJS))
else
    # Linux/Unix
    RM = rm -f
    EXE = game
endif

# Default target
all: $(EXE)

# Target for Windows
game.exe: $(OBJS)
	$(CXX) $(CXXFLAGS) -o game.exe $(OBJS)
	@echo Windows build successful. Removing object files...
	@$(RM) $(OBJS_WIN)

# Target for Linux
game: $(OBJS)
	$(CXX) $(CXXFLAGS) -o game $(OBJS)
	@echo Linux build successful. Removing object files...
	@$(RM) $(OBJS)

# Compile each source file into an object file
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up the build files
clean:
	@$(RM) $(OBJS_WIN) $(OBJS) game.exe game

# Build both versions
both: game.exe game

# Phony targets
.PHONY: all clean both
