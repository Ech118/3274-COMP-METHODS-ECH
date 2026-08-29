CXX = clang++
CXXFLAGS = -std=c++23
TARGET = foo

all: $(TARGET)

$(TARGET): foo.cpp
	$(CXX) $(CXXFLAGS) foo.cpp -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean
