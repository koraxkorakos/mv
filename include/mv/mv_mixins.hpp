#pragma once

#include "./bitset.hpp"

#include <algorithm>
#include <array>
#include <type_traits>

namespace mv {

namespace expr {

using BitSet = std::uintmax_t;
using zero_t = std::integral_constant<int, 0>;

///\brief multi vector expression mixin
template <typename D, typename M, typename BS, typename S> class MVE_M;

///\brief specialization for scalar only multi vector expressions
// template <typename D, typename M, typename S>
// class MVE_M<D, M, nlist::List<>, S>;

///\brief multi vector sink
template <typename D, typename M, typename BS, typename S> class MVSink_M;

////////////////////////////////////////////////////////////////////////////////////
// DECLARATION & DEFINITION
///////////////////////////////////////////////////////////////////////////////////

///\concepts
/// - mixin
///
///\tparam D derived class (only class that can derive from this mixin)
///\tparam M fullfills the concept of a metric
///\tparam BS template instance of `nlist::List` ``, a sorted list `nlist::List`
/// of `BiSet`
/// Best pass the a sorted list without repetition to avoid a lot of template
/// instantions, but we sort and make unique the list anyhow.
///\tparam S the scalar type of the coefficients, e.g. `double`
template <typename D, typename M, typename BS, typename S> class MVE_M {
  using Derived = D;

public:
  using Metric = M;
  using Bases =
      typename nlist::ListForm<nlist::Sort<nlist::ConsForm<BS>>>::type;
  using value_type = S;

  template <BitSet n>
  std::enable_if_t<(sortedPos(n, Bases{}) >= 0), S> get() const noexcept
  {
    return derived().template getImpl<n>();
  } ///< \overload \return value computed by `D`

  template <BitSet n>
  std::enable_if_t<(sortedPos(n, Bases{}) < 0), S> get() const
  {
    return value_type();
  } ///< \overload \return zero (`value_type{}`)

  MVE_M &operator=(MVE_M const &) = delete;

  /// number of coefficients not necessarily zero
  constexpr static std::size_t size() { return nlist::size(Bases{}); }

private:
  MVE_M()              = default;
  MVE_M(MVE_M const &) = default;
  ~MVE_M()             = default;
  constexpr D const &derived() const { return static_cast<D const &>(*this); }
  friend D;
};

///\brief specialization for the scalar only multivector expression adding an
/// implicit converstion to the `value_type`
///
///\tparam D derived class (only class that can derive from this mixin)
///\tparam M fullfills the concept of a metric
///\tparam S the scalar type of the coefficients, e.g. `double`
template <typename D, typename M, typename S>
class MVE_M<D, M, nlist::List<0>, S> {
  using Derived = D;

public:
  using Metric     = M;
  using Bases      = nlist::List<0>;
  using value_type = S;

  MVE_M &operator=(MVE_M const &) = delete;

  template <BitSet n> std::enable_if_t<n == 0, S> get() const noexcept
  {
    return derived().template getImpl<n>();
  }

  template <BitSet n> std::enable_if_t<n != 0, S> get() const noexcept
  {
    return value_type{};
  }

  operator value_type() const
  {
    return derived().template get<0>();
  } ///< implicitely convert to `value_type`

  /// number of coefficients not necessarily zero
  constexpr static std::size_t size() { return nlist::size(Bases{}); }

private:
  MVE_M()              = default;
  MVE_M(MVE_M const &) = default;
  ~MVE_M()             = default;

  constexpr D const &derived() const { return static_cast<D const &>(*this); }
  friend D;
};

///\brief multi vector expression sink
///
/// Can be used to implement variables, streaming, etc.
///\concepts
/// - mixin
template <typename D, typename M, typename BS, typename S> class MVSink_M {
  using Derived = D;

public:
  using Metric = M;
  using Bases =
      typename nlist::ListForm<nlist::Sort<nlist::ConsForm<BS>>>::type;
  using value_type = S;

  template <BitSet n>
  std::enable_if_t<(sortedPos(n, Bases{}) >= 0), D &> set(S const &v) noexcept
  {
    derived().template setImpl<n>(v);
    return derived();
  }

  /// number of coefficients not necessarily zero
  constexpr static std::size_t size() { return nlist::size(Bases{}); }

protected:
  MVSink_M &operator=(MVSink_M const &) = default;

private:
  MVSink_M()                 = default;
  MVSink_M(MVSink_M const &) = default;
  ~MVSink_M()                = default;

  constexpr D &derived() { return static_cast<D &>(*this); }
  friend D;
};
} // namespace expr

} // namespace mv