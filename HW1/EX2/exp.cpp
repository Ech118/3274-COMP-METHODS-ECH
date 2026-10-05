#include <cmath>
#include <iostream>
#include <sstream>

int main(int argc, char *argv[])
{
  if(argc != 2)
  {
    return 1;
  }
  
  std::stringstream ss(argv[1]);
  double  x;

  if(ss >> x)
  {
    std::cout << std::exp(x) << std::endl;
  }
  else return 1;

  
  return 0;
}
