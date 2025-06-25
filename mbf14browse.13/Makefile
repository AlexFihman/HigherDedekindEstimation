# Compiler and flags
CXX := g++
CC  := gcc
CXXFLAGS := -O3 -DSFMT_MEXP=19937
CFLAGS   := -O3 -DSFMT_MEXP=19937

# Sources and objects
CXX_SRCS := mbf14browse.cpp fastlist.cpp mbf16c.cpp time_ms.cpp rnd.cpp
C_SRCS   := SFMT.c

CXX_OBJS := $(CXX_SRCS:.cpp=.o)
C_OBJS   := $(C_SRCS:.c=.o)
OBJS     := $(CXX_OBJS) $(C_OBJS)

# Target
TARGET := mbf14browse

# Default target
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
