
#include <memory>
//
#include <nnshared/shared.hpp>

template<class Lhs, class Rhs>
bool
checkValues(Lhs lhs, Rhs rhs)
{
   if (lhs == nullptr || rhs == nullptr)
      return lhs == rhs;
   return *lhs == *rhs;
}

int
main()
{
   int* myptr = nullptr;
   nn::nnptr<int*> ptr = myptr; // breaks on runtime only
   std::shared_ptr<int> sptr{ new int{ 10 } };
   //
   // checkValues(ptr, sptr); // SHOULD break if uncommented (cannot even compare
   // with nullptr...)

   return 0;
}