#pragma once
#if __cplusplus > 201703
#include <concepts>
#endif
#include <cstdint>
#include <type_traits>

#include "./meta.hpp"

namespace mv {

namespace nset {

using N = std::uintmax_t;

namespace details {
template <typename T> struct IsScons : std::false_type {};
} // namespace details

#if __cplusplus <= 201703
#define Set typename
#else
template <typename S>
concept Set = std::is_same_v<NIL, S> || details::IsScons<S>::value();

template <typename T> concept arithmetic = std::is_arithmetic_v<T>;
#endif

///\invariant `degenerate(m&n) == !get<n&m>()`
// template <typename S>
// concept Metric = requires (N n, N m)
//{
//  template <N n, N m>  static get() -> std::convertible_to<double>;
//};

#define Metric typename

template <N n, Set S> struct SCons {
  using type = SCons;
  using tail = S;
  static constexpr N value(){ return n; }
};
namespace details {
template <N n, Set S> struct IsScons<SCons<n, S>> : std::true_type {};
} // namespace details

///\invarinat `Set<Insert::type>`
template <N n, Set S> struct Insert : Insert<n, typename S::type> {};
template <N n> struct Insert<n, NIL> : SCons<n, NIL> {};
template <N n, Set S> struct Insert<n, SCons<n, S>> : SCons<n, S> {};
template <N n, N m, Set S>
struct Insert<n, SCons<m, S>>
    : SCons<(n < m ? n : m), typename Insert<(n < m ? m : n), S>::type> {};

namespace details {

///\invarinat `Set<MakeSet::type>`
template <N... t> struct MakeSet;
template <> struct MakeSet<> : NIL {};
template <N n, N... t>
struct MakeSet<n, t...> : Insert<n, typename MakeSet<t...>::type> {};

} // namespace details

template <N... t> using NSet = typename details::MakeSet<t...>::type::type;

template <typename A, typename B>
struct Union : Union<typename A::type, typename B::type> {};

template <N n, Set S> struct Union<SCons<n, S>, NIL> : SCons<n, S> {};
template <N n, Set S> struct Union<NIL, SCons<n, S>> : SCons<n, S> {};
template <> struct Union<NIL, NIL> : NIL {};
template <N n, Set S> struct Union<SCons<n, S>, SCons<n, S>> : SCons<n, S> {};

template <N n, Set S1, Set S2>
struct Union<SCons<n, S1>, SCons<n, S2>>
    : SCons<n, typename Union<S1, S2>::type> {};

template <N n, N m, Set S1, Set S2>
struct Union<SCons<n, S1>, SCons<m, S2>>
    : SCons<(n < m ? n : m),
            typename std::conditional_t<(n < m), Union<S1, SCons<m, S2>>,
                                        Union<SCons<n, S1>, S2>>::type> {};

template <typename A, typename B> using Union_t = typename Union<A, B>::type;

template <typename A, typename B>
struct Difference : Difference<typename A::type, typename B::type> {};
template <> struct Difference<NIL, NIL> : NIL {};
template <N n, Set S> struct Difference<NIL, SCons<n, S>> : NIL {};
template <N n, Set S> struct Difference<SCons<n, S>, NIL> : SCons<n, S> {};
template <N n, Set S> struct Difference<SCons<n, S>, SCons<n, S>> : NIL {};

template <N n, Set S1, Set S2>
struct Difference<SCons<n, S1>, SCons<n, S2>> : Difference<S1, S2> {};
template <N n, N m, Set S1, Set S2>
struct Difference<SCons<n, S1>, SCons<m, S2>>
    : std::conditional_t<(n < m), Insert<n, Difference<S1, SCons<m, S2>>>,
                         Difference<SCons<n, S1>, S2>> {};

template <typename A, typename B>
using Difference_t = typename Difference<A, B>::type;

template <typename A, typename B>
struct Intersection : Intersection<typename A::type, typename B::type> {};
template <> struct Intersection<NIL, NIL> : NIL {};
template <N n, Set S> struct Intersection<NIL, SCons<n, S>> : NIL {};
template <N n, Set S> struct Intersection<SCons<n, S>, NIL> : NIL {};
template <N n, Set S>
struct Intersection<SCons<n, S>, SCons<n, S>> : SCons<n, S> {};

template <N n, Set S1, Set S2>
struct Intersection<SCons<n, S1>, SCons<n, S2>>
    : SCons<n, typename Intersection<S1, S2>::type> {};
template <N n, N m, Set S1, Set S2>
struct Intersection<SCons<n, S1>, SCons<m, S2>>
    : std::conditional_t<(n < m), Intersection<S1, SCons<m, S2>>,
                         Intersection<SCons<n, S1>, S2>> {};

template <typename A, typename B>
using Intersection_t = typename Intersection<A, B>::type;

template <typename A, typename B>
struct SymmetricDifference
    : SymmetricDifference<typename A::type, typename B::type> {};
template <> struct SymmetricDifference<NIL, NIL> : NIL {};
template <N n, Set S>
struct SymmetricDifference<NIL, SCons<n, S>> : SCons<n, S> {};
template <N n, Set S>
struct SymmetricDifference<SCons<n, S>, NIL> : SCons<n, S> {};

template <N n, Set S1, Set S2>
struct SymmetricDifference<SCons<n, S1>, SCons<n, S2>>
    : SymmetricDifference<S1, S2> {};
template <N n, N m, Set S1, Set S2>
struct SymmetricDifference<SCons<n, S1>, SCons<m, S2>>
    : SCons<(n < m ? n : m), typename std::conditional_t<
                                 (n < m), SymmetricDifference<S1, SCons<m, S2>>,
                                 SymmetricDifference<SCons<n, S1>, S2>>::type> {
};

template <typename A, typename B>
using SymmetricDifference_t = typename SymmetricDifference<A, B>::type;

template <typename A, typename B>
struct ExteriorProduct : ExteriorProduct<typename A::type, typename B::type> {};
template <> struct ExteriorProduct<NIL, NIL> : NIL {};
template <N n, Set S> struct ExteriorProduct<NIL, SCons<n, S>> : NIL {};
template <N n, Set S> struct ExteriorProduct<SCons<n, S>, NIL> : NIL {};

template <N n, N m, Set S>
struct ExteriorProduct<SCons<n, NIL>, SCons<m, S>>
    : std::conditional_t<
          (0 != (n & m)), ExteriorProduct<SCons<n, NIL>, S>,
          SCons<(0 != (n | m)), typename ExteriorProduct<SCons<n, NIL>, S>::type>> {};

template <N n, N m, Set S1, Set S2>
struct ExteriorProduct<SCons<n, S1>, SCons<m, S2>>
    : Union<ExteriorProduct<SCons<n, NIL>, SCons<m, S2>>,
            ExteriorProduct<S1, SCons<m, S2>>> {};

template <typename A, typename B>
using ExteriorProduct_t = typename ExteriorProduct<A, B>::type;

template <Metric M, Set A, Set B>
struct InnerProduct : InnerProduct<M, typename A::type, typename B::type> {};
template <Metric M> struct InnerProduct<M, NIL, NIL> : NIL {};
template <Metric M, N n, Set S>
struct InnerProduct<M, NIL, SCons<n, S>> : NIL {};
template <Metric M, N n, Set S>
struct InnerProduct<M, SCons<n, S>, NIL> : NIL {};

template <Metric M, N n, N m, Set S>
struct InnerProduct<M, SCons<n, NIL>, SCons<m, S>>
    : std::conditional_t<0!=(n & m) && M::template get<n, m>(),
                         Insert<(n ^ m), InnerProduct<M, SCons<n, NIL>, S>>,
                         InnerProduct<M, SCons<n, NIL>, S>> {};
template <Metric M, N n, N m, Set S1, Set S2>
struct InnerProduct<M, SCons<n, S1>, SCons<m, S2>>
    : Union<ExteriorProduct<SCons<n, NIL>, SCons<m, S2>>,
            InnerProduct<M, S1, SCons<m, S2>>> {};

template <Metric M, Set A, Set B>
struct CliffordProd : Union<ExteriorProduct<A, B>, InnerProduct<M, A, B>> {};

} // namespace nset

} // namespace mv
