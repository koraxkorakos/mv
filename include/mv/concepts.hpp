#pragma once

#include <concepts>
#include <cstdint>
#include <type_traits>

#include <mv/be.hpp>

namespace mv {

///\pre symmetric in arguments `m(a, b) == m(b,a)`
template <typename M> concept metric = requires(M const m, be::bases a, be::bases b)
{
  {
    m(a, b)
  }
  ->std::convertible_to<typename M::scalar>;
  {
    M::instance()
  }
  ->std::convertible_to<M>;
};

template <typename M> concept metrical = metric<typename M::metric>;

template <typename A, typename B>
concept same_metric = metrical<A> &&metrical<B>
    &&std::same_as<typename A::mt, typename B::mt>;

///\brief shared rquirements on expressions and sinks
template <typename A> concept mv_shared = metrical<A> && be::basis_blades<typename A::bbl>;

// multivector expression
template <typename E>
concept mv_expr = mv_shared<E> &&
    std::convertible_to<E, bool> &&requires(E const e)
{
  {
    e.template get<0ULL>()
  }
  ->std::convertible_to<typename E::scalar>;
};

// multivector sink
template <typename E>
concept mv_sink = mv_shared<E> &&requires(E e, typename E::scalar value)
{
  {
    e.template set<0ULL>(value)
  }
  ->std::same_as<E &>;
};

template <typename E> concept mv_storage = mv_sink<E> &&mv_expr<E>;



} // namespace mv