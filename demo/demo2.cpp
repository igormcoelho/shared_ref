
#include <iostream>
#include <memory>
//
#include <nnptr/sref.hpp>

template<class Lhs, class Rhs>
bool
checkValues(Lhs lhs, Rhs rhs)
{
   if (lhs == nullptr || rhs == nullptr)
      return lhs == rhs;
   return *lhs == *rhs;
}

// see: https://github.com/Microsoft/GSL/pull/675
class TestClass
{};

int
main()
{
   // ========= FIRST TEST ==========
   int* myptr = nullptr;
   // next line breaks on Debug only (without -DNDEBUG)
   nnptr::NotNull<int*> ptr = myptr;
   std::shared_ptr<int> sptr{ new int{ 10 } };
   //
   //checkValues(ptr, sptr); // SHOULD compile-break if uncommented (cannot even compare with nullptr...)

   // ========= SECOND TEST ==========
   nnptr::NotNull<std::unique_ptr<TestClass>> some_unique{ std::make_unique<TestClass>() };

   //TestClass some_class = std::move(*some_unique);
   TestClass some_class = *some_unique;

   // pointer could be zero...
   std::cout << some_unique.get().get() << std::endl;

   std::unique_ptr<TestClass>& u_ptr = (std::unique_ptr<TestClass>&)some_unique.get();
   u_ptr.release(); // will leak memory
   // pointer will be zero (next line will break on Debug)
   std::cout << some_unique.get().get() << std::endl;

   return 0;
}