CXX = c++
CXXFLAGS = -Wall -std=c++20
AR = ar
ARFLAGS = rcs

all: plus libPlus.a

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
	
libPlus.a: add.o parse.o print.o
	$(AR) $(ARFLAGS) libPlus.a add.o parse.o print.o

plus: main.o libPlus.a
	$(CXX) $(CXXFLAGS) main.o -o plus libPlus.a
	
clean:
	rm -f *.o libPlus.a plus
	
