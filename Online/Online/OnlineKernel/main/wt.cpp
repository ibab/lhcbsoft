#include "RTL/DllAccess.h"
#include <iostream>

int main (int argc, char** argv)  {
  void* handle = LOAD_LIB("OnlineKernel");
  if ( 0 != handle )  {
    Function fun(GETPROC(handle, argv[1] ));
    if ( fun.function ) {
      return (*fun.function)(argc-1, &argv[1]);
    }
    std::cout << "Failed to access test procedure!" << std::endl;
  }
  std::cout << "Failed to load test library!" << std::endl;
  std::cout << "Error: " << DLERROR << std::endl;
  return 0;
}
