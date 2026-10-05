CXX = c++
CXXFLAGS = -std=c++20 -Wall

TARGET = complex_trig
SOURCES = complex_trig.cpp
OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all clean
