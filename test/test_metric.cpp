#include "../include/mv/metric.hpp" // #include <mv/mv.hpp> // vs code cannt parse our CMakeLists.txt correctly

#include <gtest/gtest.h>
#include <testutils/demangle.hpp>

using cm::testing::demangle;
using std::is_same;

TEST(test_bitset, test_euclidean_metric)
{
  using mv::EuclideanMetric;

  EXPECT_EQ(0U, EuclideanMetric<0>::positive());
  EXPECT_EQ(0U, EuclideanMetric<0>::negative());
  EXPECT_EQ(0U, EuclideanMetric<0>::degenerate());

  EXPECT_EQ(1U, EuclideanMetric<1>::positive());
  EXPECT_EQ(0U, EuclideanMetric<1>::negative());
  EXPECT_EQ(0U, EuclideanMetric<1>::degenerate());

  EXPECT_EQ(2U, EuclideanMetric<2>::positive());
  EXPECT_EQ(0U, EuclideanMetric<2>::negative());
  EXPECT_EQ(0U, EuclideanMetric<2>::degenerate());

  EXPECT_EQ(3U, EuclideanMetric<3>::positive());
  EXPECT_EQ(0U, EuclideanMetric<3>::negative());
  EXPECT_EQ(0U, EuclideanMetric<3>::degenerate());

  constexpr const EuclideanMetric<4> m;

  for (int k = 0; k != 4; ++k)
    for (int j = 0; j != 4; ++j) EXPECT_EQ(k == j, m(k, j));
}

TEST(test_bitset, test_minkowski_metric)
{
  using mv::MinkowsiMetric;

  EXPECT_EQ(0U, MinkowsiMetric<1>::positive());
  EXPECT_EQ(1U, MinkowsiMetric<3>::negative());
  EXPECT_EQ(0U, MinkowsiMetric<3>::degenerate());

  EXPECT_EQ(1U, MinkowsiMetric<2>::positive());
  EXPECT_EQ(1U, MinkowsiMetric<2>::negative());
  EXPECT_EQ(0U, MinkowsiMetric<2>::degenerate());

  EXPECT_EQ(2U, MinkowsiMetric<3>::positive());
  EXPECT_EQ(1U, MinkowsiMetric<3>::negative());
  EXPECT_EQ(0U, MinkowsiMetric<3>::degenerate());

  EXPECT_EQ(3U, MinkowsiMetric<4>::positive());
  EXPECT_EQ(1U, MinkowsiMetric<4>::negative());
  EXPECT_EQ(0U, MinkowsiMetric<4>::degenerate());

  constexpr const MinkowsiMetric<4> m;

  for (int k = 0; k != 4; ++k)
    for (int j = 0; j != 4; ++j)
      EXPECT_EQ(k != j ? 0 : k == 3 ? -1 : 1, m(k, j));
}
