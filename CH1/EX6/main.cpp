#include <iostream>
#include "common.h"

// Expects two integers and prints sum

int main(int argc, char *argv[])
{
  int a, b;
  if(!parse(argc, argv, a, b))
  {
    return 1;
  }

  print(add(a, b), a, b);
  return 0;
}

