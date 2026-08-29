#include <iostream>
#include <string>

// Expects two integers and prints sum

int add(int, int);

int main(int argc, char *argv[])
{
  if(argc != 3) return 1;

  
  int a = std::stoi(argv[1]);
  int b = std::stoi(argv[2]);
  
  std::cout << a << "+" << b << "=" << add(a, b) << std::endl;
}

int add(int a, int b)
{
  return a + b;
}
