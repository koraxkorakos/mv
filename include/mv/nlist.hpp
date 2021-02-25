#pragma once
#include <cstdint>
#include <type_traits>

#include "./meta.hpp"

namespace mv{

namespace nlist
{
  using N = std::uintmax_t;

///\section Number list 
///
/// number list is a lazy meta sum type.It is either of:
/// - `NIL`
/// - `Cons<h,L>`, where h a number and `L` a metafunction returning a number list

///\tparam L a meta function returning an number list
  template<N h, typename L=NIL>
  struct Cons : Const<Cons<h,L>>{
      static constexpr N head(){ return h; };  
      using tail = L;
  };

  template <typename L1, typename L2>
  struct Merge : Merge<typename L1::type, typename L2::type>{};

  template <>
  struct Merge<NIL,NIL> : NIL{};

  template <N h, typename L>
  struct Merge<NIL,Cons<h,L>> : Cons<h,L>{};

  template <N h, typename L>
  struct Merge<Cons<h,L>,NIL> : Cons<h,L>{};

  ///\brief specialization removing duplictates
  template <N h, typename L1, typename L2>
  struct Merge<Cons<h,L1>,Cons<h,L2>> : Merge<Cons<h,L1>,L2>{};

  template <N h1, N h2, typename L1, typename L2>
  struct Merge<Cons<h1,L1>,Cons<h2,L2>> : 
     Cons<(h1 < h2 ? h1 : h2),
           typename std::conditional<(h1<h2),
                             Merge<L1,Cons<h2,L2>>,
                             Merge<Cons<h1,L1>,L2>>>::type{};


  template <typename L>
  struct Sort : Sort<typename L::type>{};

  template <>
  struct Sort<NIL>: NIL{};

  template <N h>
  struct Sort<Cons<h>> : Cons<h>{};

  template <N h, typename L>
  struct Sort<Cons<h,L>> : Merge<Sort<Even<L>>,Sort<Cons<h,typename Odd<L>::type>>>{};

  /// specialization is just optimization
  template <N h1, N h2>
  struct Sort<Cons<h1,Cons<h2>>> : Cons<(h1<h2?h1:h2),Cons<(h1<h2?h2:h1)>>{};

  /// specialization is just optimization
  template <N h>
  struct Sort<Cons<h,Cons<h>>> : Cons<h>{};

   template <N... n>
   struct List : Const<List<n...>>{};
   template <>
   struct List<> : Quote<List<>>{};


   template <N...t>
   constexpr std::size_t size(List<t...>)
   {
     return sizeof...(t);
   }

   template <N h,typename L>
   struct PushFront : PushFront<h, typename L::type>{};

   template <typename L>
   struct ListForm : ListForm<typename L::type>{};

      template <N h,N... n>
   struct PushFront<h, List<n...>> : Const<List<h,n...>>{};

   template <N h,typename L>
   struct ListForm<Cons<h,L>> : PushFront<h, typename ListForm<L>::type>{};
 
   template <>
   struct ListForm<NIL> : Const<List<>>{};

   template <typename T>
   struct ConsForm : Const<typename T::type>{};

   template <>
   struct ConsForm<List<>> : NIL{};

   template <N h, N... t>
   struct ConsForm<List<h,t...>> : Cons<h, ConsForm<List<t...>>>{};
}

  ///\brief specialization for number lists
  ///\note `L::type` is just an optimization
  template <nlist::N n, typename L>
  struct Strict<nlist::Cons<n,L>> : nlist::Cons<n,typename Strict<typename L::type>::type>{};

  ///\brief specialization for number lists
  template <nlist::N n, typename L>
  struct Even<nlist::Cons<n,L>> : nlist::Cons<n,Odd<L>>{};

  ///\brief specialization for number lists
  template <nlist::N n, typename L>
  struct Odd<nlist::Cons<n,L>> : Even<L>{};

  template <typename... T>
  constexpr unsigned numArgs(T const & ...)
  {
     return sizeof...(T);
  }

}