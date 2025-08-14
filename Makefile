CXX := c++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2 -Wno-missing-field-initializers \
    -I/usr/local/include
LDFLAGS := -L/usr/local/lib -larmadillo -lcairo

SRC := main.cpp
OBJ := $(SRC:.cpp=.o)
TARGET := main

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJ) $(TARGET)

