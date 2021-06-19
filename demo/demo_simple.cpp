
#include <iostream>
#include <nnptr/nnshared.hpp>

template<class R>
using sref = nnptr::NNShared<R>;

sref<double>
foo(sref<int> si)
{
   double d = *si;
   return new double{ d };
}

int
main()
{
   auto sd = foo(10);
   std::cout << sd << std::endl;
   sd++;
   std::cout << sd << std::endl;
   return 0;
}