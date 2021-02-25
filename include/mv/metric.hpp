#pragma once

namespace mv {

///\brief mixin class for Metric
///\tparam D derived class
///\concept
/// - mixin
/// - symmetric bilinear form
template <typename D, typename S>
class Metric_M {
public:
  using value_type = S;

  constexpr value_type operator()(value_type n, value_type m) const
  {
    return D::factorImp(n, m);
  }

private:

  constexpr Metric_M()                 = default;
  ~Metric_M()                          = default;
  constexpr Metric_M(Metric_M const &) = default;
  Metric_M &operator=(Metric_M const &) = delete;

  friend D;
};

template <int n, int m = 0, int p = 0>
class DiagonalMetric: public Metric_M<DiagonalMetric<n, m, p>, int> {
  using Inherited = Metric_M<DiagonalMetric, int>;
  using V = typename Inherited::value_type;
  static_assert(!(n < 0 || m < 0 || p < 0), "Signature must constist of nonnegative numbers only.");

public:
  constexpr DiagonalMetric() noexcept{};
  constexpr DiagonalMetric(DiagonalMetric const &)noexcept{};
  constexpr static unsigned positive() { return (unsigned)n; } ///<
  constexpr static unsigned negative() { return (unsigned)m; }
  constexpr static unsigned degenerate() { return (unsigned)p; }
private:  
  constexpr static V factorImp(V const &a, V const &b)
  {
    return a != b && a < n + m //
               ? 0             //
               : a < n         //
                     ? 1       //
                     : -1;
  }
  friend Inherited;
};

template <int n>
using EuclideanMetric = DiagonalMetric<n>;

template <int n>
using MinkowsiMetric = DiagonalMetric<n - 1, 1>;

} // namespace mv