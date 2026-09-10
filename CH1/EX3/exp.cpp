#include <iostream>
#include <sstream>

double exp(double);

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
    std::cout << exp(x) << std::endl;
  }
  else return 1;

  
  return 0;
}

double exp(double x)
{
  double term = 1.0;
  double sum = 1.0;
  int n = 1;

  while (n < 100)
  {
    term *= x/n;
    sum += term;
    n++;
  }
  
  return sum;
}
