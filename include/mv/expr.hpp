#pragma once

#include "concepts.hpp"

namespace mv {

template <typename T> class constant {
public:
  using bbl = be::list<0>;
  constant(T const& t) noexcept(T{std::declval<T>()}): t_(t) {}
  template <be::bases n, typename E>
  auto get() const noexcept(get<n>(std::declval<T>()))
  {
    return get<n>(t_);
  }

private:
  T t_;
};

template <typename Op> class nullary_op {
public:
  using bbl = typename Op::bbl;
  explicit nullary_op(Op const& op) noexcept(Op{std::declval<Op>()}): op_{op} {}
  template <be::bases n, typename E>
  auto get() const noexcept(std::declval<Op>().template compute<n>())
  {
    return op_.template compute<n>();
  }

private:
  Op op_;
};

template <typename Op, typename Arg> class unary_op {
public:
  using bbl = typename Op::template bbl<typename Arg::bbl>;
  explicit unary_op(Op const& op, Arg const& arg): op_{op}, arg_{arg} {}
  template <be::bases n, typename E> auto get() const
  {
    return op_.template compute<n>(arg_);
  }

private:
  Op  op_;
  Arg arg_;
};

template <typename Op, typename LHS, typename RHS> class binary_op {
public:
  using bbl =
      typename Op::template bbl<typename LHS::bbl, typename RHS::bbl>;
  explicit binary_op(Op const& op, LHS const& lhs, RHS const& rhs) requires(same_metric<LHS,RHS>)
      : op_{op}, lhs_{lhs}, rhs_{rhs}
  {
  }
  template <be::bases n> auto get() const
  {
    return op_.template compute<n>(lhs_, rhs_);
  }

private:
  Op  op_;
  LHS lhs_;
  RHS rhs_;
};

} // namespace mv