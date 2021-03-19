
#include <iostream>
#include <nnshared/shared.hpp>

int
main()
{
   // ==========================================
   // easy to create by passing pointer
   //
   nn::shared<int> p1(new int{ 9 });

   // ==========================================
   // fails to create explicit nullptr (good thing!)
   //
   //nn::shared<int> p2(nullptr); // FAIL: not allowed!

   // ==========================================
   // copy shared ownership of object
   //
   nn::shared<int> p3(p1);

   // automatic conversion to internal type int
   std::cout << p3 << std::endl;

   // automatic conversion to internal type int
   auto p4 = p1 + p3;
   std::cout << typeid(p4).name() << ": " << p4 << std::endl;
   
   // ==========================================
   // cannot pass ownership of local reference (only pointers)
   int x5 = 10;
   //nn::shared<int> p5 = x5; // FAIL: cannot build from 'int'
   nn::shared<int> p5 (new int{p1 + p4}); // OK: can pack pointer


   // ==========================================
   // works with complex types (such as strings)
   //
   nn::shared<std::string> p_str(new std::string{ "hello world!" });
   // note: for now, explicit cast is needed...
   std::cout << (std::string)p_str << std::endl;
   

   return 0;
}