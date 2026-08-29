CXX = c++
CXXFLAGS = -Wall -std=c++20
AR = ar
ARFLAGS = rcs

all: plus libPlus.so


%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
	
libPlus.so: add.o parse.o print.o
	$(CXX) $(CXXFLAGS) -shared -fPIC add.o parse.o print.o -o libPlus.so

plus: main.o libPlus.so
	$(CXX) $(CXXFLAGS) main.o -o plus libPlus.so
	
clean:
	rm -f *.o libPlus.so plus
	
