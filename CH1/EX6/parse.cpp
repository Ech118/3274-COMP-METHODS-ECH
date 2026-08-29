#include "common.h"
#include <cstdlib>


bool parse(int argc, char* argv[], int& a, int& b)
{
  if(argc != 3) return false;

  
  a = std::atoi(argv[1]);
  b = std::atoi(argv[2]);
  return true;
}
