#pragma once

#include "bitset.hpp"
#include "mv_mixins.hpp"

#include <algorithm>
#include <array>
#include <type_traits>

namespace mv {

using BitSet = nist::N;

struct ZeroInit {
};

/// Container Adapter for memory not owned
template <typename M,
          typename B,
          typename S,
          typename Stride = std::integral_constant<std::ptrdiff_t, 1>>
class View;

/// \specialization for read only memory
template <typename M, typename B, typename S, typename Stride>
class View<M, B, S const, Stride>;

template <typename M, typename B, typename S>
class MV;

namespace details {

template <BitSet n, typename A1, typename A2>
std::common_type_t<typename A1::value_type, typename A2::value_type>
    outer_product(A1 const &, A2 const &);
template <BitSet n, typename A1, typename A2>
std::common_type_t<typename A1::value_type, typename A2::value_type>
    inner_product(A1 const &, A2 const &);

template <typename A, typename BS>
class Cast;
template <typename A,
          typename M,
          typename BS = typename A::Bases,
          typename S  = typename A::value_type>
class Reinterpret;

template <typename A, typename M, typename B, typename C>
class Scaled;

template <typename M, BitSet n, typename C>
class ScaledBasisBlade;

template <typename A>
class Negation;

template <typename A1, typename A2>
class Sum;

template <typename A1, typename A2>
class Difference;

template <typename A1, typename A2>
class GeometricProduct;
template <typename A1, typename A2>
class OuterProduct;
template <typename A1, typename A2>
class InnerProduct;

template <typename A1, typename A2>
class LeftContraction;

template <typename A1, typename A2>
class RightContraction;

} // namespace details

//////////////////////////////////////////////////////////////////////////////
// DECLARATIONS & DEFINITIONS
//////////////////////////////////////////////////////////////////////////////

///\brief metric, bases, scalar infering assignment to sink
template <template <typename, typename, typename> class Sink, typename E>
inline auto eval(E const &e)
{
  using M = typename E::Metric;
  using B = typename E::Bases;
  using S = typename E::Scalar;
  return Sink<M,B,S>(e);
}

template <typename M, typename B, typename S>
class MV : 
    : public MVE_M<MV<M, B, S>, M, B, S>
    , public View<MV<M,B,S>,M, B, S> {
  using Expression = MVE_M<MV, M, B, S>;
  using Sink       = View<MV<M,B,S>,M, B, S>;

public:
  using typename Expression::Bases;
  using typename Expression::Metric;
  using typename Expression::value_type;

  using Expression::size;

  MV() = default; ///\note no initialization!
  explicit constexpr MV(ZeroInit): data_(){};
  ~MV()               = default;
  MV(MV const &other) = default;

  template <typename S2>
  MV(MV<Metric, Bases, S2> const &other)
  noexcept
  {
    std::copy_n(other.data_, Expression::size(), this->data_);
  }

  /// _implicit_ conversion constructor (copy from sibling)
  template <typename D2, typename S2>
  MV(MVE_M<D2, M, B, S2> const &other)
  noexcept // cppcheck-suppress "noExplicitConstructor"
  {
    assign(other);
  }

  MV &operator=(MV const &other)
  {
    data_ = other.data_;
    return *this;
  }

  /// _implicit_ conversion copy (assign from sibling)
  template <typename D2, typename M2, typename BS2, typename S2>
  MV &operator=(MVE_M<D2, M, B, S2> const &other) noexcept
  {
    assign(other);
    return *this;
  }

private:
  constexpr Sink const &sink() const { return *this; }
  constexpr Sink &      sink() { return *this; }

  template <B
itSet n>
  value_type const &getImpl() const noexcept
  {
    return data_[sortedPos(n, Bases{})];
  }

  template <B
itSet n>
  void setImpl(value_type const &v) noexcept
  {
    data_[sortedPos(n, Bases{})] = v;
  }

  template <typename D2, typename M2, typename BS2, typename S2>
  void assign(MVE_M<D2, M2, BS2, S2> const &other)
  {
    using Other = MVE_M<D2, M2, BS2, S2>;

    static_assert(std::is_same<Metric, typename Other::Metric>::value,
                  "Mixup of metrices");
    static_assert(std::is_same<Bases, typename Other::Bases>::value,
                  "Incompatible coefficients");

    copy(other, *this, Bases{});
  }

  std::array<S, Expression::size()> data_;

  template <typename, typename, typename>
  friend class MV;
  friend Expression;
  friend Sink;
};

namespace details {

template <typename A>
class Negation
    : public MVE_M<Negation<A>,
                   typename A::Metric,
                   typename A::Bases,
                   typename A::value_type> {
public:
  constexpr explicit Negation(A const &arg): arg_(arg) {}

private:
  template <BitSet n>
  decltype(auto) getImpl() const
  {
    return -arg_.template get<n>();
  }

  A &arg_;
  friend class Inherited;
};
template <typename A1, typename A2>
class Sum
    : public MVE_M<Sum<A1, A2>,
                   typename A1::Metric,
                   typename A1::Bases,
                   std::common_type_t<typename A1::value_type,
                                      typename A2::value_type>> {
  using Inherited = MVE_M<
      Sum,
      typename A1::Metric,
      typename A1::Bases,
      std::common_type_t<typename A1::value_type, typename A2::value_type>>;

public:
  static_assert(std::is_same<typename A1::Metric, typename A2::Metric>::value,
                "metric mismatch");
  constexpr explicit Sum(A1 const &lhs, A2 const &rhs): lhs_(lhs), rhs_(rhs) {}

private:
  template <B
itSet n>
  decltype(auto) getImpl() const
  {
    return lhs_.template get<n>() + rhs_.template get<n>();
  }

  A1 &lhs_;
  A2 &rhs_;
  friend Inherited;
};

template <typename A1, typename A2>
class Difference
    : public MVE_M<Difference<A1, A2>,
                   typename A1::Metric,
                   typename A1::Bases,
                   typename std::common_type<typename A1::value_type,
                                             typename A2::value_type>::type> {
  using Inherited =
      MVE_M<Difference,
            typename A1::Metric,
            typename A1::Bases,
            typename std::common_type<typename A1::value_type,
                                      typename A2::value_type>::typ>;

public:
  static_assert(std::is_same<typename A1::Metric, typename A2::Metric>::value,
                "metric misnmatch");
  constexpr explicit Difference(A1 const &lhs, A2 const &rhs)
      : lhs_(lhs), rhs_(rhs)
  {
  }

private:
  template <BitSet n>
  decltype(auto) getImpl() const
  {
    return lhs_.template get<n>() - rhs_.template get<n>();
  }
  A1 &lhs_;
  A2 &rhs_;
  friend Inherited;
};

template <typename A1, typename A2>
class OuterProduct
    : public MVE_M<OuterProduct<A1, A2>,
                   typename A1::Metric,
                   typename A1::Bases,
                   std::common_type_t<typename A1::value_type,
                                      typename A2::value_type>> {
  using Inherited = MVE_M<
      OuterProduct,
      typename A1::Metric,
      typename A1::Bases,
      std::common_type_t<typename A1::value_type, typename A2::value_type>>;

public:
  static_assert(std::is_same<typename A1::Metric, typename A2::Metric>::value,
                "metric mismatch");

  constexpr explicit OuterProduct(A1 const &lhs, A2 const &rhs)
      : lhs_(lhs), rhs_(rhs)
  {
  }

private:
  template <B
itSet n>
  decltype(auto) getImpl() const
  {
    return outer_product<n>(lhs_, rhs_);
  }

  A1 &lhs_;
  A2 &rhs_;
  friend Inherited;
};

template <typename A1, typename A2>
class InnerProduct
    : public MVE_M<InnerProduct<A1, A2>,
                   typename A1::Metric,
                   typename A1::Bases,
                   std::common_type_t<typename A1::value_type,
                                      typename A2::value_type>> {
  using Inherited = MVE_M<
      InnerProduct,
      typename A1::Metric,
      typename A1::Bases,
      std::common_type_t<typename A1::value_type, typename A2::value_type>>;

public:
  static_assert(std::is_same<typename A1::Metric, typename A2::Metric>::value,
                "metric mismatch");

  constexpr explicit InnerProduct(A1 const &lhs, A2 const &rhs)
      : lhs_(lhs), rhs_(rhs)
  {
  }

private:
  template <BitSet n>
  decltype(auto) getImpl() const
  {
    return inner_product<n>(lhs_, rhs_);
  }

  A1 &lhs_;
  A2 &rhs_;
  friend Inherited;
};

template <typename A1, typename A2>
class GeometricProduct
    : public MVE_M<
          GeometricProduct<A1, A2>,
          typename A1::Metric,
          typename A1::Bases,
          std::common_type_t<typename A1::value_type, typename A2::value_type>>
    , private InnerProduct<A1, A2>
    , OuterProduct<A1, A2> {
  using Inherited = MVE_M<
      GeometricProduct,
      typename A1::Metric,
      typename A1::Bases,
      std::common_type_t<typename A1::value_type, typename A2::value_type>>;
  using Inner = InnerProduct<A1, A2>;
  using Outer = OuterProduct<A1, A2>;

public:
  static_assert(std::is_same<typename A1::Metric, typename A2::Metric>::value,
                "metric mismatch");

  constexpr explicit GeometricProduct(A1 const &lhs, A2 const &rhs)
      : Inner(lhs, rhs), Outer(lhs, rhs)
  {
  }

private:
  constexpr Inner const &inner() const
  {
    return static_cast<Inner const &>(*this);
  }
  constexpr Outer const &outer() const
  {
    return static_cast<Outer const &>(*this);
  }
  template <BitSet n>
  decltype(auto) getImpl() const
  {
    return inner().template get<n>() + outer().template get<n>();
  }
  friend Inherited;
};

template <typename A, typename BS>
class Cast
    : public MVE_M<Cast<A, BS>,
                   typename A::Metric,
                   BS,
                   typename A::value_type> {
  using Inherited = MVE_M<Cast, typename A::Metric, BS, typename A::value_type>;

public:
  constexpr explicit Cast(A const &arg): arg_(arg) {}

private:
  template <B
itSet n>
  decltype(auto) getImpl() const
  {
    return arg_.template get<n>();
  }
  A &arg_;
  friend Inherited;
};

template <typename A, typename M, typename BS, typename S>
class Reinterpret: public MVE_M<Reinterpret<A, M, BS, S>, M, BS, S> {
  using Inherited = MVE_M<Reinterpret, M, BS, S>;

public:
  constexpr explicit Reinterpret(A const &arg): arg_(arg) {}

private:
  template <B
itSet n>
  decltype(auto) getImpl() const
  {
    return arg_.template get<n>();
  }
  A &arg_;
  friend Inherited;
};

///\tparam C must have an nullary function returning a value
template <typename M, BitSet n, typename C>
class ScaledBasisBlade
    : public MVE_M<ScaledBasisBlade<M, n, C>, M, nlist::List<n>, C>
    , private C {
  using Inherited = MVE_M<ScaledBasisBlade<M, n, C>, M, nlist::List<n>, C>;

public:
  constexpr explicit ScaledBasisBlade(C const &c = C{}): C{c} {}

private:
  template <BitSet m>
  decltype(auto) getImpl() const
  {
    return static_cast<C const &>(*this);
  }
  friend Inherited;
};

///\tparam C must have an nullary function returning a value
template <typename A, typename M, typename B, typename C>
class Scaled
    : public MVE_M<Scaled<A, M, B, C>,
                   M,
                   B,
                   typename std::common_type<C, typename A::value_type>::type>
    , private C {
  using Inherited =
      MVE_M<Scaled,
            M,
            B,
            typename std::common_type<C, typename A::value_type>::type>;

public:
  constexpr explicit Scaled(A const &a, C const &c = C{}): C{c}, arg_{a} {}

private:
  template <B
itSet m>
  decltype(auto) getImpl() const
  {
    return static_cast<C const &>(*this) * arg_->template get<m>();
  }
  A *arg_;
  friend Inherited;
};

/// has all its set `BitSet` coefficiens set to `static_cast<typename C::value_type>(C{})`
template <typename M, typename B, typename C = std::integral_constant<int,1>>
class Uniform
    : public MVE_M<Uniform<M, B, C>,
                   M,
                   B,
                   typename C::value_type>
    , private C {
  using Inherited =
      MVE_M<Uniform,
            M,
            B,
            typename std::common_type<C, typename C::value_type>::type>;

public:
  constexpr explicit Uniform(C const &c = C{}): C{c}, arg_{a} {}

private:
  template <B
itSet m>
  decltype(auto) getImpl() const
  {
    return static_cast<C const &>(*this);
  }
  friend Inherited;
}

template <typename M, typename B, typename S, typename Stride>
class View<M, B, S const, Stride>
    : public MVE_M<View<M, B, S const, Stride>, M, B, S> {
  using Inherited = MVE_M<View, M, B, S>;

public:
  constexpr explicit View(S data[], Stride stride = Stride{})
      : data_{data}, stride_{stride}
  {
  }

private:
  template <B
itSet m>
  S const &getImpl() const
  {
    return data_[stride_ * sortedPos(m, typename Inherited::Bases{})];
  }
  S *    data_;
  Stride stride_;
  friend Inherited;
};

template <typename M, typename B, typename S, typename Stride>
class View: public MVE_M<View<M, B, S, Stride>, M, B, S> {
  using Inherited = MVE_M<View, M, B, S>;

public:
  constexpr explicit View(S data[], Stride stride = Stride{})
      : data_{data}, stride_(stride)
  {
  }

private:
  template <BitSet m>
  S const &getImpl() const
  {
    return data_[stride_ * sortedPos(m, typename Inherited::Bases{})];
  }
  template <BitSet m>
  View &setImpl(S const &v) const
  {
    data_[stride_ * sortedPos(m, typename Inherited::Bases{})] = v;
    return *this;
  }
  S *    data_;
  Stride stride_;
  friend Inherited;
};

/// operators ///////////////////////////


template <typename M, typename D, typename OM, typename OBS, typename OS>
constexpr auto
    reinterpretMetric(MVE_M<D, OM, OBS, OS> const &other)
{
  return Reinterpret<MVE_M<D, OM, OBS, OS>, M, OBS, OS>{other};
}

template <typename BS, typename D, typename OM, typename OBS, typename OS>
constexpr auto
    reinterpretCoeff(MVE_M<D, OM, OBS, OS> const &other)
{
  return Reinterpret<MVE_M<D, OM, OBS, OS>, OM, OBS, OS>{other};
}

template <typename S, typename D, typename OM, typename OBS, typename OS>
constexpr auto
    reinterpretScalar(MVE_M<D, OM, OBS, S> const &other)
{
  return Reinterpret<MVE_M<D, OM, OBS, OS>, OM, OBS, S>{other};
}

template <typename M,
          typename BS,
          typename S,
          typename D,
          typename OM,
          typename OBS,
          typename OS>
constexpr auto
    reinterpret(MVE_M<D, OM, OBS, BS> const &other)
{
  return Reinterpret<MVE_M<D, OM, OBS, OS>, M, BS, S>{other};
}

template <typename BS, typename D, typename M, typename OBS, typename S>
constexpr auto
    cast(MVE_M<D, M, OBS, S> const &other)
{
  return Cast<M, MVE_M<D, M, OBS, S>>{other};
}

template <typename BS, typename D, typename M, typename S>
constexpr decltype(auto)
    operator+(MVE_M<D, M, BS, S> const &arg)
{
  return arg;
}

template <typename BS, typename D, typename M, typename S>
constexpr auto
    operator-(MVE_M<D, M, BS, S> const &arg)
{
  return Negation<MVE_M<D, M, BS, S>>(arg);
}

#if 0
template <typename BS, typename D, typename M, typename S>
constexpr auto operator~(MVE_M<D,M,BS,S> const &arg) 
{ 
    return Reverse<MVE_M<D,M,BS,S>>(arg); 
}

template <typename BS, typename D, typename M, typename S>
constexpr auto operator*(MVE_M<D,M,BS,S> const &arg) 
{ 
    return Involute<MVE_M<D,M,BS,S>>(arg); 
}
#endif

template <typename BS1,
          typename BS2,
          typename M,
          typename D1,
          typename D2,
          typename S1,
          typename S2>
constexpr auto
    operator+(MVE_M<D1, M, BS1, S1> const &lhs,
              MVE_M<D2, M, BS2, S2> const &rhs)
{
  using LHS = MVE_M<D1, M, BS1, S1>;
  using RHS = MVE_M<D2, M, BS2, S2>;

  return Sum<LHS, RHS>{lhs, rhs};
}

template <typename BS1,
          typename BS2,
          typename M,
          typename D1,
          typename D2,
          typename S1,
          typename S2>
constexpr auto
    operator-(MVE_M<D1, M, BS1, S1> const &lhs,
              MVE_M<D2, M, BS2, S2> const &rhs)
{
  using LHS = MVE_M<D1, M, BS1, S1>;
  using RHS = MVE_M<D2, M, BS2, S2>;

  return Difference<LHS, RHS>(lhs, rhs);
}

template <typename BS1,
          typename BS2,
          typename M,
          typename D1,
          typename D2,
          typename S1,
          typename S2>
constexpr auto
    operator^(MVE_M<D1, M, BS1, S1> const &lhs,
              MVE_M<D2, M, BS2, S2> const &rhs)
{
  using LHS = MVE_M<D1, M, BS1, S1>;
  using RHS = MVE_M<D2, M, BS2, S2>;

  return OuterProduct<LHS, RHS>{lhs, rhs};
}

template <typename BS,
          typename M,
          typename D1,
          typename D2,
          typename S1,
          typename S2>
constexpr auto
    operator^(MVE_M<D1, M, BS, S1> const &lhs, MVE_M<D2, M, BS, S2> const &rhs)
{
  using LHS = MVE_M<D1, M, BS, S1>;
  using RHS = MVE_M<D2, M, BS, S2>;

  return OuterProduct<LHS, RHS>{lhs, rhs};
}

template <typename BS,
          typename M,
          typename D1,
          typename D2,
          typename S1,
          typename S2>
constexpr auto
    operator|(MVE_M<D1, M, BS, S1> const &lhs, MVE_M<D2, M, BS, S2> const &rhs)
{
  using LHS = MVE_M<D1, M, BS, S1>;
  using RHS = MVE_M<D2, M, BS, S2>;

  return InnerProduct<LHS, RHS>{lhs, rhs};
}

template <typename BS,
          typename M,
          typename D1,
          typename D2,
          typename S1,
          typename S2>
constexpr auto operator*(MVE_M<D1, M, BS, S1> const &lhs,
                         MVE_M<D2, M, BS, S2> const &rhs)
{
  using LHS = MVE_M<D1, M, BS, S1>;
  using RHS = MVE_M<D2, M, BS, S2>;

  return GeometricProduct<LHS, RHS>{lhs, rhs}; ///\bug references temporary
}

template <BitSet... n, typename M, typename D, typename S1, typename S2>
constexpr auto operator*(MVE_M<D, M, nlist::List<n...>, S1> const &lhs,
                         S2 const &                                rhs)
{
  return lhs * MV<M, nlist::List<0>, S1>{rhs};
}

template <typename B, typename M, typename D, typename S1, typename S2>
constexpr auto operator*(S1 const &lhs, MVE_M<D, M, B, S1> rhs)
{
  return MV<M, nlist::List<0>, S1>{lhs} * rhs;
}

#if 0
template <typename BS, typename M, typename D1, typename D2, typename S1, typename S2>
constexpr auto operator<(MVE_M<D1,M,BS,S1> const &lhs, MVE_M<D2,M,BS,S2> const &rhs) 
{ 
    return LeftConstract<MVE_M<D,M,BS,S>>(lhs,rhs); 
}

template <typename BS, typename M, typename D1, typename D2, typename S1, typename S2>
constexpr auto operator<(MVE_M<D1,M,BS,S1> const &lhs, MVE_M<D2,M,BS,S2> const &rhs) 
{ 
    return RightContraction
  <MVE_M<D,M,BS,S>>(lhs,rhs); 
}
#endif

///\pre `p != nullptr`
template <typename M, typename BS, typename S, typename Stride>
constexpr auto
    view(S *p, Stride stride)
{
  assert(p);
  return View<M, BS, S, Stride>{p, stride};
}

///\pre `p != nullptr`
template <typename M, typename BS, typename S>
constexpr auto
    view(S *p)
{
  return view(p, std::integral_constant<std::ptrdiff_t, 1>{});
}

} // namespace details

} // namespace mv
