
#include <iostream>
#include <nnshared/shared.hpp>
#include <vector>

template<class T>
using nn_shared_ptr = nn::nnptr<std::shared_ptr<T>>;

class Person
{
public:
};

class Company
{
public:
   Company(Person& _owner, std::vector<Person*> _employees)
     : owner{ _owner }
     , employees{ _employees }
   {}

private:
   Person& owner;                  // shared ownership?
   std::vector<Person*> employees; // shared ownership?
};

class Company2
{
public:
   Company2(nn::shared<Person>& _manager,
            std::vector<nn::shared<Person>>& _employees)
     : manager{ _manager }
     , employees{ _employees }
   {}

private:
   nn::shared<Person> manager;                // shared ownership!
   std::vector<nn::shared<Person>> employees; // shared ownership!
};

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
   // nn::shared<int> p2(nullptr); // FAIL: not allowed!

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
   // nn::shared<int> p5 = x5; // FAIL: cannot build from 'int'
   nn::shared<int> p5(new int{ p1 + p4 }); // OK: can pack pointer

   // ==========================================
   // works with complex types (such as strings)
   //
   nn::shared<std::string> p_str(new std::string{ "hello world!" });
   // note: for now, explicit cast is needed...
   std::cout << (std::string)p_str << std::endl;

   // =========================================
   // even better for complex scenarios
   nn::nnptr<std::shared_ptr<std::vector<int>>> nnsptr_1{
      std::shared_ptr<std::vector<int>>(new std::vector<int>(10, 1))
   };
   std::cout << "v[0] = " << nnsptr_1.get().get()->at(0) << std::endl;

   nn_shared_ptr<std::vector<int>> nnsptr_2{
      std::shared_ptr<std::vector<int>>(new std::vector<int>(10, 1))
   };
   std::cout << "v[0] = " << nnsptr_2.get().get()->at(0) << std::endl;

   nn::shared<std::vector<int>> nnsptr_3{ new std::vector<int>(10, 1) };
   std::cout << "v[0] = " << nnsptr_3->at(0) << std::endl;

   // =========================================
   // nn::shared is allowed inside vectors
   //
   std::vector<nn::shared<int>> vshared;
   vshared.push_back(nn::shared<int>{ new int{ 1 } });
   vshared.push_back(nn::shared<int>{ new int{ 2 } });
   vshared.push_back(nn::shared<int>{ new int{ 3 } });
   // should print '123'
   std::cout << vshared[0] << vshared[1] << vshared[2] << std::endl;

   // ==========================================
   // derived classes have automatic cast
   //
   class A
   {
   public:
   };

   class B : public A
   {
   public:
   };
   nn::shared<B> b{ new B };
   nn::shared<B> b2 = b;
   nn::shared<A> a = b;

   return 0;
}