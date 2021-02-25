#pragma once
#include <cstdint>
#include <type_traits>

///\file shared basic meta programming facilities 

namespace mv{
    
///\brief metafunction return type `T` after n+1 th call
template <typename T, unsigned n = 0U> 
struct Quote{ 
    using type = Quote<T,n-1U>; 
};

template <typename T> struct Quote<T,0U>{ 
    using type = T; 
};

///\brief use via CRTP to create meta function fixed point
template<typename T> 
using Const = Quote<T>;

struct NIL : Const<NIL>{}; ///< empty list, shared by type lists and value lists

///\brief metafunction expand lazy list
template <typename L>
struct Strict : Strict<typename L::type>{};

template <>
struct Strict<NIL> : NIL{};

  ///\brief metafunction filtering out odd elements in a lazy list
  ///\note We start at index 0
  template <typename L>
  struct Even : Even<typename L::type>{};

  ///\brief metafunction filtering out even elements in a lazy list
  ///\note We start at index 0
  template <typename L>
  struct Odd : Odd<typename L::type>{};

  template <>
  struct Even<NIL> : NIL{};

  template <>
  struct Odd<NIL> : NIL{};


}