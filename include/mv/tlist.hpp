#pragma once
#include <cstdint>
#include <type_traits>

#include "./meta.hpp"

namespace mv{

namespace tlist
{

///\section Type list 
///
/// number list is a lazy meta sum type.It is either of:
/// - `NIL`
/// - `Cons<T,L>`, where H is a type and `L` a metafunction returning a type list

///\tparam L a meta function returning an number list
  template<typename H, typename L=NIL>
  struct Cons : Const<Cons<H,L>>{
      using head = H;  
      using tail = L;
  };


   template <typename... T>
   struct List : Const<List<T...>>{};

   template <typename H,typename L>
   struct PushFront : PushFront<H, typename L::type>{};

   template <typename L>
   struct ListForm : ListForm<typename L::type>{};

   template <>
   struct ListForm<NIL> : Const<List<>>{};

   template <typename H,typename L>
   struct ListForm<Cons<H,L>> : PushFront<H, typename ListForm<L>::type>{};
  
   template <typename H, typename... T>
   struct PushFront<H, List<T...>> : Const<List<H,T...>>{};
}

  ///\brief specialization for type lists
  template <typename H, typename L>
  struct Strict<tlist::Cons<H,L>> : tlist::Cons<H,typename Strict<typename L::type>::type>{};

///\brief specialization for type lists
  template <typename H, typename L>
  struct Even<tlist::Cons<H,L>> : tlist::Cons<H,Odd<L>>{};

///\brief specialization for type lists
  template <typename H, typename L>
  struct Odd<tlist::Cons<H,L>> : Even<L>{};


}