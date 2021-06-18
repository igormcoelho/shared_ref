//#include <gsl/gsl> // not_null (local copy)
#include <memory> // shared_ptr

// ====================================================================
// Not Null Shared Pointer (nn::shared)
// Authors: Igor M. Coelho & Fellipe Pessanha
// MIT License (2021)
// ====================================================================

// This library depends on...

// ====================================================================
// nnptr: an alternative to gsl::not_null, but with NDEBUG removing all
// 'not_null' during Release
// ====================================================================

// =============
// For nn::nnptr
// =============

#include <algorithm>    // for forward
#include <iosfwd>       // for ptrdiff_t, nullptr_t, ostream, size_t
#include <memory>       // for shared_ptr, unique_ptr
#include <system_error> // for hash
#include <type_traits>  // for enable_if_t, is_convertible, is_assignable

// ======================
// begin nn::nnptr part
// ======================

// =============================================
// Same assumptions are valid from gsl::not_null
// =============================================
//
// nn::nnptr restricts a pointer to only hold non-null values (enforced during
// Debug time, not Release)
//
// Has zero size overhead over T (during both Debug and Release)
//
// For nnptr<T>, if T is a pointer (i.e. T == U*) then
// - allow construction from U*
// - disallow construction from nullptr_t
// - disallow default construction
// - ensure construction from null U* fails
// - allow implicit conversion to U*
//

// ==============================================
// code for nnptr (compatible with gsl::not_null)
// ==============================================

namespace nn {

template <class T> class nnptr {
public:
  static_assert(std::is_assignable<T &, std::nullptr_t>::value,
                "T cannot be assigned nullptr.");

  template <typename U,
            typename = std::enable_if_t<std::is_convertible<U, T>::value>>
  constexpr nnptr(U &&u) : ptr_(std::forward<U>(u)) {
#ifndef NDEBUG
    if (ptr_ == nullptr)
      std::terminate();
#endif
  }

  template <
      typename = std::enable_if_t<!std::is_same<std::nullptr_t, T>::value>>
  constexpr nnptr(T u) : ptr_(u) {
#ifndef NDEBUG
    if (ptr_ == nullptr)
      std::terminate();
#endif
  }

  template <typename U,
            typename = std::enable_if_t<std::is_convertible<U, T>::value>>
  constexpr nnptr(const nnptr<U> &other) : nnptr(other.get()) {}

  nnptr(const nnptr &other) = default;
  nnptr &operator=(const nnptr &other) = default;

  constexpr T get() const {
#ifndef NDEBUG
    if (ptr_ == nullptr)
      std::terminate();
#endif
    return ptr_;
  }

  constexpr operator T() const { return get(); }
  constexpr T operator->() const { return get(); }
  constexpr decltype(auto) operator*() const { return *get(); }

  // prevents nullptr_t in constructor
  nnptr(std::nullptr_t) = delete;
  nnptr &operator=(std::nullptr_t) = delete;

  // disable harmful arithmetics and offset (just a holder pointer)
  nnptr &operator++() = delete;
  nnptr &operator--() = delete;
  nnptr operator++(int) = delete;
  nnptr operator--(int) = delete;
  nnptr &operator+=(std::ptrdiff_t) = delete;
  nnptr &operator-=(std::ptrdiff_t) = delete;
  void operator[](std::ptrdiff_t) const = delete;

private:
  T ptr_;
};

template <class T> auto make_nnptr(T &&t) {
  return nnptr<std::remove_cv_t<std::remove_reference_t<T>>>{
      std::forward<T>(t)};
}

template <class T>
std::ostream &operator<<(std::ostream &os, const nnptr<T> &val) {
  os << val.get();
  return os;
}

template <class T, class U>
auto operator==(const nnptr<T> &lhs, const nnptr<U> &rhs)
    -> decltype(lhs.get() == rhs.get()) {
  return lhs.get() == rhs.get();
}
// should prevent comparisons with nullptr_t
template <class T>
bool operator==(const nnptr<T> &lhs, std::nullptr_t nptr) = delete;

template <class T, class U>
auto operator!=(const nnptr<T> &lhs, const nnptr<U> &rhs)
    -> decltype(lhs.get() != rhs.get()) {
  return lhs.get() != rhs.get();
}
// should prevent comparisons with nullptr_t
template <class T>
bool operator!=(const nnptr<T> &lhs, std::nullptr_t nptr) = delete;

template <class T, class U>
auto operator<(const nnptr<T> &lhs, const nnptr<U> &rhs)
    -> decltype(lhs.get() < rhs.get()) {
  return lhs.get() < rhs.get();
}

template <class T, class U>
auto operator<=(const nnptr<T> &lhs, const nnptr<U> &rhs)
    -> decltype(lhs.get() <= rhs.get()) {
  return lhs.get() <= rhs.get();
}

template <class T, class U>
auto operator>(const nnptr<T> &lhs, const nnptr<U> &rhs)
    -> decltype(lhs.get() > rhs.get()) {
  return lhs.get() > rhs.get();
}

template <class T, class U>
auto operator>=(const nnptr<T> &lhs, const nnptr<U> &rhs)
    -> decltype(lhs.get() >= rhs.get()) {
  return lhs.get() >= rhs.get();
}

// more unwanted operators
template <class T, class U>
std::ptrdiff_t operator-(const nnptr<T> &, const nnptr<U> &) = delete;
template <class T>
nnptr<T> operator-(const nnptr<T> &, std::ptrdiff_t) = delete;
template <class T>
nnptr<T> operator+(const nnptr<T> &, std::ptrdiff_t) = delete;
template <class T>
nnptr<T> operator+(std::ptrdiff_t, const nnptr<T> &) = delete;

} // namespace nn

namespace std {
template <class T> struct hash<nn::nnptr<T>> {
  std::size_t operator()(const nn::nnptr<T> &value) const {
    return hash<T>{}(value);
  }
};

} // namespace std

// ======================
// begin nn::shared part
// ======================

namespace nn {
//
template <typename T> class shared {
  using shared_type = T;

public:
  nnptr<std::shared_ptr<T>> data_;

  // this constructor can be used to "move" into new shared_ptr versions
  // this requires a copy constructor over the type X (must be concrete)
  template <
      class X,
      typename =
          typename std::enable_if<std::is_convertible<X *, T *>::value>::type,
      typename =
          typename std::enable_if<std::is_copy_constructible<X>::value>::type>
  shared(X &&other) : data_{std::shared_ptr<T>{new X(other)}} {}

  // this is for existing references (must have copy constructor)
  template <
      class X,
      typename =
          typename std::enable_if<std::is_convertible<X *, T *>::value>::type,
      typename =
          typename std::enable_if<std::is_copy_constructible<X>::value>::type>
  shared(const X &other) : data_{std::shared_ptr<T>{new X(other)}} {}

  shared(const shared<T> &other) : data_{other.data_} {}

  shared(const shared<T> &&corpse) : data_{corpse.data_} {}

  shared(std::shared_ptr<T> &data) : data_{data} {}

  shared(T *data) : data_{std::shared_ptr<T>{data}} {}

  // disallow explicit nullptr
  shared(std::nullptr_t data) = delete;

  T *operator->() { return data_.get().get(); }

  T &operator*() {
    // return dereferenced shared object
    return *data_;
  }

  T &get() {
    // return dereferenced shared object
    return *data_;
  }

  operator T &() {
    // return dereferenced shared object
    return *data_;
  }

  template <class Y, typename = typename std::enable_if<
                         std::is_convertible<T *, Y *>::value>::type>
  operator shared<Y>() // explicit? not necessary... (until now)
  {
    std::shared_ptr<Y> py = data_.get(); // remove encapsulation from 'nnptr'
    return py;
  }
};

} // namespace nn
