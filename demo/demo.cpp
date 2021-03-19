
#include <iostream>
#include <nnshared/shared.hpp>

int
main()
{
   nn::shared<int> p1(new int{ 9 });
   //nn::shared<int> p2(nullptr); // OK, not allowed!

   nn::shared<int> p3(p1);
   std::cout << p3 << std::endl;

   nn::shared<std::string> p_str(new std::string{ "hello world!" });
   std::cout << (std::string) p_str << std::endl;

   return 0;
}