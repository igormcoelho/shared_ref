#include <gsl/gsl> // not_null
#include <memory>  // shared_ptr

// Not Null Shared Pointer
// Authors: Igor M. Coelho & Fellipe Pessanha
// MIT License (2021)

namespace nn {
//
template<typename T>
class shared
{
public:
   gsl::not_null<std::shared_ptr<T>> data_;

   shared(const shared<T>& other)
     : data_{ other.data_ }
   {
   }

   shared(const shared<T>&& corpse)
     : data_{ corpse.data_ }
   {
   }

   shared(std::shared_ptr<T>& data)
     : data_{ data }
   {
   }

   shared(T* data)
     : data_{ std::shared_ptr<T>{ data } }
   {
   }

   // disallow explicit nullptr
   shared(std::nullptr_t data) = delete;

   T* operator->()
   {
      return data_.get().get();
   }

   T& operator*()
   {
      // return dereferenced shared object
      return *data_;
   }

   T& get()
   {
      // return dereferenced shared object
      return *data_;
   }

   operator T&()
   {
      // return dereferenced shared object
      return *data_;
   }

   // cast operator for derived class: Y& y = t, or Y* y = &t
   // example (B extends A): shared<B> b = ...; shared<A> a = b;
   template<class Y,
            typename = typename std::enable_if<
              std::is_convertible<T*, Y*>::value>::type>
   operator shared<Y>() // not explicit for now
   {
      std::shared_ptr<Y> py = data_.get(); // remove not null layer for cast
      return py;
   }
};

} // namespace nn
