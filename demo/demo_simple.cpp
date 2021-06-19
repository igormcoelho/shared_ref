
#include <iostream>
#include <nnptr/nnshared.hpp>
#include <vector>

template<class R>
using sref = nnptr::NNShared<R>;

sref<double>
foo(sref<int> si)
{
   double d = si;
   return new double{ d };
}

int
main()
{
   auto sd = foo(10);
   std::cout << sd << std::endl;
   sd++;
   std::cout << *sd << std::endl;

   sref<std::vector<int>> nnsptr_3{ std::vector<int>(5, 1) };
   // getting the first element in vector
   std::cout << "v[0] = " << nnsptr_3->at(0) << std::endl;

   return 0;
}