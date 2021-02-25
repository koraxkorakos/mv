#pragma once

/// We want to avoid excessive CRTP

#include "meta.hpp"
#include "nlist.hpp"
#include "nset.hpp"
#include "rational_constant.hpp"

#include <cstdint>
#include <functional>
#include <limits>
#include <type_traits>

namespace mv {

namespace expr {

using BitSet = std::uintmax_t;
using zero_t = typename RationalConstant<int, 0, 1>::type;

/// emeulation for pre C++20
#if __cplusplus <= 201703L
#  define MV_expr typename

namespace details {
/// SFINAE check that checked for nested `typedef` Bases
template <typename T> struct isMVExpr {
  template <typename U> static int check(U*, typename U::Bases* = nullptr);
  static char                      check(...);

  constexpr static bool value = sizeof(std::declval<T*>()) == sizeof(int);
};
} // namespace details

///\brief
///\par concept
/// - boolean trait
template <typename S>
constexpr bool const is_MV_expr = details::isMVExpr<S>::value;

///\brief get for multivector types that have coefficient zero at `n`
///\overload
template <BitSet n, typename S>
constexpr auto
    get(S const& v) noexcept
    -> std::enable_if_t<!is_MV_expr<S> && n == 0, S const&>
{
  return v;
}

///\brief get for multivector types that have coefficient zero at `n`
///\overload
template <BitSet n, typename S>
constexpr auto
    get(S const& v) noexcept
    -> std::enable_if_t<!is_MV_expr<S> && n != 0, zero_t>
{
  return zero_t{};
}

///\brief get for multivector types that have coefficient zero at `n`
///\overload
template <BitSet n, MV_expr S>
auto
    get(S const& v) noexcept(
        noexcept(std::declval<S const&>().template get<n>()))
        -> std::enable_if_t<
            is_MV_expr<S> && contains(typename S::Bases{}, n),
            decltype(std::declval<S const&>().template get<n>())>
{
  return v.template get<n>();
}

///\brief get for multivector types that have coefficient zero at `n`
///\overload
template <BitSet n, MV_expr S>
auto
    get(S const& v) noexcept
    -> std::enable_if_t<is_MV_expr<S> && !contains(typename S::Bases{}, n),
                        zero_t>
{
  return zero_t{};
}

///\brief set multivector coefficient
/// \overload for coefficients known to be zero.
template <BitSet n, typename B, typename A, typename V>
inline constexpr auto
    set(A&& a, V&& v) noexcept(noexcept(std::declval<A>().template set<n>(v)))
        -> A&&
{
  a.template set<n>(std::forward<V>(v));
  return a;
}

#else
template <typename T> concept Arithmetic = std::is_arithmetic<T>::value;

template <typename E>
concept MV_expr = Arithmetic<typename E::value_type>&& requires(E e, BitSet n)
{
  {
    e.get<n>(e)
  }
  ->std::convertible_to<typename E::value_type>;
};

template <typename E>
concept MV_sink = Arithmetic<typename E::value_type>&& requires(
    E                             e,
    BitSet                        n,
    typename E::value_type const& v)
{
  {
    e.set<n>(v)
  }
  ->std::same_as<E&>;
};

template <typename T> static constexpr bool const is_MV_expr = MV_expr<T>;

///\brief get for multivector types that have coefficient zero at `n`
///\overload
template <BitSet n, typename S>
constexpr S const&
    get(S const& v, std::enable_if_t<n == 0, void*>) noexcept
{
  return v;
}

///\brief get for multivector types that have coefficient zero at `n`
///\overload
template <BitSet n, typename S>
constexpr auto
    get(S const& v, std::enable_if_t<n != 0, void*>) noexcept
{
  return zero_t();
}

///\brief get for multivector types that have coefficient zero at `n`
///\overload
template <BitSet n, MV_expr S>
auto get(S const &v) noexcept(noexcept(std::declval<S const &>().template get<n>())) -> 
-> std::enable_if_t<contains(typename S::Bases{},n), decltype(std::declval<S const &>().template get<n>()))>
{
  return v.template get<n>();
}

///\brief get for multivector types that have coefficient zero at `n`
///\overload
template <BitSet n, MV_expr S>
auto
    get(S const& v) noexcept
    -> std::enable_if_t<!contains(typename S::Bases{}, n), zero_t>
{
  return zero_t;
}

#endif

template <typename T> class Constant {
public:
  using Bases = nlist::List<0>;
  Constant(T const& t) noexcept(T{std::declval<T>()}): t_(t) {}
  template <BitSet n, typename E>
  auto get() const noexcept(get<n>(std::declval<T>()))
  {
    return get<n>(t_);
  }

private:
  T t_;
};

template <typename Op> class NullaryOp {
public:
  using Bases = typename Op::Bases;
  explicit NullaryOp(Op const& op) noexcept(Op{std::declval<Op>()}): op_{op} {}
  template <BitSet n, typename E>
  auto get() const noexcept(std::declval<Op>().template compute<n>())
  {
    return op_.template compute<n>();
  }

private:
  Op op_;
};

template <typename Op, typename Arg> class UnaryOp {
public:
  using Bases = typename Op::template Bases<typename Arg::Bases>;
  explicit UnaryOp(Op const& op, Arg const& arg): op_{op}, arg_{arg} {}
  template <BitSet n, typename E> auto get() const
  {
    return op_.template compute<n>(arg_);
  }

private:
  Op  op_;
  Arg arg_;
};

template <typename Op, typename LHS, typename RHS> class BinaryOp {
public:
  using Bases =
      typename Op::template Bases<typename LHS::Bases, typename RHS::Bases>;
  explicit BinaryOp(Op const& op, LHS const& lhs, RHS const& rhs)
      : op_{op}, lhs_{lhs}, rhs_{}
  {
  }
  template <BitSet n> auto get() const
  {
    return op_.template compute<n>(lhs_, rhs_);
  }

private:
  Op  op_;
  LHS lhs_;
  RHS rhs_;
};

struct UPlus {
  template <typename Arg> using Bases = typename Arg::Bases;
  template <BitSet n, typename Arg>
  auto compute(Arg const& arg) const
      noexcept(noexcept(get<n>(std::declval<Arg>())))
  {
    return get<n>(arg);
  }
};

struct UMinus {
  template <typename Arg> using Bases = typename Arg::Bases;
  template <BitSet n, typename Arg>
  auto compute(Arg const& arg) const
      noexcept(noexcept(-get<n>(std::declval<Arg>())))
  {
    return get<n>(arg);
  }
};

struct Sum {
  template <typename LHS, typename RHS>
  using type =
      typename nset::Union<typename LHS::Bases, typename RHS::Bases>::type;
  template <BitSet n, typename LHS, typename RHS>
  auto compute(LHS const& e1, RHS const& e2) const
      noexcept(noexcept(get<n>(std::declval<LHS>())
                        + get<n>(std::declval<RHS>())))
  {
    return get<n>(e1) + get<n>(e2);
  }
};

struct Difference {
  template <typename LHS, typename RHS>
  using type =
      typename nset::Union<typename LHS::Bases, typename RHS::Bases>::type;
  template <BitSet n, typename LHS, typename RHS>
  auto compute(LHS const& e1, RHS const& e2) const
      noexcept(noexcept(get<n>(std::declval<LHS>())
                        - get<n>(std::declval<RHS>())))
  {
    return get<n>(e1) - get<n>(e2);
  }
};

struct OuterProduct {
  template <typename LHS, typename RHS>
  using type = nset::ExteriorProduct<typename LHS::Bases, typename RHS::Bases>;
  template <BitSet n, typename LHS, typename RHS>
  auto compute(LHS const& e1, RHS const& e2) const
      noexcept(noexcept(outer_product<n, B>(e1, e2))) ///\todo
  {
    using details::outer_product;
    return outer_product<n, B>(e1, e2);
  }
};

struct InnerProduct {
  template <typename LHS, typename RHS, Metric M>
  using type = nset::InnerProduct<M, typename LHS::Bases, typename RHS::Bases>;
  template <BitSet n, typename LHS, typename RHS>
  auto compute(LHS const& e1, RHS const& e2) const
      noexcept(noexcept(inner_product<M, n, B>(e1, e2)))
  {
      using details::inner_product;
      return inner_product<M, n, B>(e1, e2);
  }
};

struct GeometricProduct {
  template <typename LHS, typename RHS, Metric M>
  using type = nset::CliffordProd<M, typename LHS::Bases, typename RHS::Bases>;
  template <BitSet n, typename LHS, typename RHS>
  auto compute(LHS const& e1, RHS const& e2) const
      noexcept(noexcept(product<M, n, B>(e1, e2)))  
  {
      using details::product;
      return product<M, n, B>(e1, e2);
  }
};
#if 0
/////////////////////////////////////////////////////////
/*
///\brief set multivector coefficient
/// \overload for coefficients known to be zero.
template <BitSet n, typename B, typename A, typename V>
inline constexpr auto
    set(A&& a, V&& v) noexcept(noexcept(std::declval<A>().template set<n>(v)))
        -> A&&
{
  a.template set<n>(v);
  return a;
}
*/
template <typename M, typename B> class Identity {
  template <BitSet n, class A>
  constexpr auto get()(A&& a) const noexcept(noexcept(+a.template get<n, B>()))
      -> decltype(+a.template get<n>())
  {
    return +a. template get<n, B>();
  }
};

template <typename M, typename B> class Negate {
  template <BitSet n, class A>
  constexpr auto get()(A&& a) const noexcept(noexcept(-a.template get<n, B>()))
      -> decltype(-a.template get<n>())
  {
    return -a. template get<n, B>();
  }
};

template <typename M, typename B> class Sum {
  template <BitSet n, class A>
  constexpr auto get()(A&& a, B&& b) const
      noexcept(noexcept(get<n, B>(a) + get<n, B>(b)))
          -> decltype(get<n, B>(a) + get<n, B>(b))
  {
    return get<n, B>(a) + get<n, B>(b);
  }
};

template <typename M, typename B> class Difference {
  template <BitSet n, class A>
  constexpr auto get()(A&& a, B&& b) const
      noexcept(noexcept(get<n, B>(a) + get<n, B>(b)))
          -> decltype(get<n, B>(a) + get<n, B>(b))
  {
    return get<n, B>(a) - get<n, B>(b);
  }
};

template <typename M, typename B> class Product {
  template <BitSet n, class A>
  constexpr auto get()(A&& a, B&& b) const
      noexcept(noexcept(a.template get<n>() * a.template get<n>()))
          -> decltype(a.template get<n>() * a.template get<n>())
  {
    return product<>(a, b);
  }

  template <typename M, typename B> class InnerProduct {
    template <BitSet n, class A>
    constexpr auto get()(A&& a, B&& b) const
        noexcept(noexcept(a.template get<n>() * a.template get<n>()))
            -> decltype(a.template get<n>() * a.template get<n>())
    {
      return inner_product<M, n, B>(a, b);
    }
  };

  template <typename M, typename B> class OuterProduct {
    template <BitSet n, class A>
    constexpr auto get()(A&& a, B&& b) const
        noexcept(noexcept(a.template get<n>() * a.template get<n>()))
            -> decltype(a.template get<n>() * a.template get<n>())
    {
      return outer_product<n, B>(a, b);
    }
  };

  template <typename M, typename B> class Product {
    template <BitSet n, class A>
    constexpr auto get()(A&& a, B&& b) const
        noexcept(noexcept(a.template get<n>() * a.template get<n>()))
            -> decltype(a.template get<n>() * a.template get<n>())
    {
      return outer_product<n, B>(a, b) + inner_product<M, n, B>(a, b);
    }

    /// evaluate an expression
    ///
    /// This one is applied to things that aren't constants or expressions
    template <typename T> inline T const& evaluate(T const& t) noexcept
    {
      return t;
    }

    /// \overload
    template <BitSet n, class Value>
    inline auto evaluate(Constant<Value> const& value)
        std::enable_idf_t<n == 0, decltype(value.value)>
    {
      return evaluate(value.value);
    }

    /// \overload
    template <BitSet n, class Value>
    inline auto evaluate(Constant<Value> const& value)
        std::enable_idf_t<n != 0, decltype(value.value)>
    {
      return decltype(value.value){};
    }

    /// \overload
    template <BitSet n, class Op, class Input>
    inline auto evaluate(UnaryOp<Op, Input> const& expr)
    {
      return expr.op template get<n>(expr);
    }

    /// \overload
    template <BitSet n, class Op, class LHS, class RHS>
    inline auto evaluate(BinaryOp<Op, LHS, RHS> const& expr)
    {
      return expr.op template get<n>(expr.lhs, expr.rhs);
    }

    template <typename Arg> auto operator-(Arg const& a)
    {
      return UnaryOp<Negate, A>{{}, a};
    }

    template <typename A, typename B> auto operator+(A const& a, B const& b)
    {
      return BinaryOp<Plus, A, B>{{}, a, b};
    }

    template <typename A, typename B> auto operator~(A const& a, B const& b)
    {
      return BinaryOp<Reverse, A, B>{{}, a, b};
    }

    template <typename A, typename B> auto operator-(A const& a, B const& b)
    {
      return BinaryOp<Minus, A, B>{{}, a, b};
    }

    template <typename A, typename B> auto operator*(A const& a, B const& b)
    {
      return BinaryOp<GeometricProduct, A, B>{{}, a, b};
    }

    template <typename A, typename B> auto operator^(A const& a, B const& b)
    {
      return BinaryOp<OuterProduct, A, B>{{}, a, b};
    }

    template <typename A, typename B> auto operator|(A const& a, B const& b)
    {
      return BinaryOp<InnerProduct, A, B>{{}, a, b};
    }

    template <typename Value> struct Constant {
      Value value;
    };

    template <typename Op, typename Input> struct UnaryOp {
      Op    op;
      Input arg;
    };
    template <typename Op, typename Left, typename Right> struct BinaryOp {
      Op    op;
      Left  lhs;
      Right rhs;
    };
#endif
  } // namespace expr
} // namespace mv
