/*
#include <gtest/gtest.h>

#include <testutils/demangle.hpp>
#include "../include/mv/metric.hpp" // #include <mv/metric.hpp> // vs code cannt parse our CMakeLists.txt correctly
#include "../include/mv/mv.hpp" // #include <mv/mv.hpp> // vs code cannt parse our CMakeLists.txt correctly

using std::is_same;

using cm::testing::demangle;

using E2 = mv::EuclideanMetric<2>;
using mv::BSet;
using mv::MV;
using mv::ZeroInit;
using mv::details::ScaledBasisBlade;

using OneInt = std::integral_constant<int, 1>;

using One = ScaledBasisBlade<E2,0,OneInt>;
using X = ScaledBasisBlade<E2,1,OneInt>;
using Y = ScaledBasisBlade<E2,2,OneInt>;
using I = ScaledBasisBlade<E2,3,OneInt>;

template <BSet... b>
using NList = mv::nlist::List<b...>;

TEST(test_mv, sink){
   mv::MV<E2, NList<>, double> v;

   EXPECT_EQ(0.0, v.get<0>());
   EXPECT_EQ(0.0, v.get<1>());
   EXPECT_EQ(0.0, v.get<2>());
   EXPECT_EQ(0.0, v.get<3>());
   EXPECT_EQ(0.0, v.get<4>());
   EXPECT_EQ(0.0, v.get<5>());
}

template <BSet... b>
using NList = mv::nlist::List<b...>;

TEST(test_mv_sink, zero){
   mv::MV<E2, NList<>, double> v;

   EXPECT_EQ(0.0, v.get<0>());
   EXPECT_EQ(0.0, v.get<1>());
   EXPECT_EQ(0.0, v.get<2>());
   EXPECT_EQ(0.0, v.get<3>());
   EXPECT_EQ(0.0, v.get<4>());
   EXPECT_EQ(0.0, v.get<5>());
}

TEST(test_mv_sink, n){
   mv::MV<E2, NList<1,3,7>, double> v{ZeroInit{}};

   EXPECT_EQ(0.0, v.get<0>());
   EXPECT_EQ(0.0, v.get<1>());
   EXPECT_EQ(0.0, v.get<2>());
   EXPECT_EQ(0.0, v.get<3>());
   EXPECT_EQ(0.0, v.get<4>());
   EXPECT_EQ(0.0, v.get<5>());

   v.set<3>(-8.0);

   EXPECT_EQ(0.0, v.get<0>());
   EXPECT_EQ(0.0, v.get<1>());
   EXPECT_EQ(0.0, v.get<2>());
   EXPECT_EQ(-8.0, v.get<3>());
   EXPECT_EQ(0.0, v.get<4>());
   EXPECT_EQ(0.0, v.get<5>());

   mv::MV<E2, NList<1,3,7>, double> v2{v};
   EXPECT_EQ(0.0, v2.get<0>());
   EXPECT_EQ(0.0, v2.get<1>());
   EXPECT_EQ(0.0, v2.get<2>());
   EXPECT_EQ(-8.0, v2.get<3>());
   EXPECT_EQ(0.0, v2.get<4>());
   EXPECT_EQ(0.0, v2.get<5>());

   mv::MV<E2, NList<1,3,7>, double> w(v);

   EXPECT_EQ(0.0, v.get<0>());
   EXPECT_EQ(0.0, v.get<1>());
   EXPECT_EQ(0.0, v.get<2>());
   EXPECT_EQ(-8.0, v.get<3>());
   EXPECT_EQ(0.0, v.get<4>());
   EXPECT_EQ(0.0, v.get<5>());
   EXPECT_EQ(v.get<0>(), w.get<0>());
   EXPECT_EQ(v.get<1>(), w.get<1>());
   EXPECT_EQ(v.get<2>(), w.get<2>());
   EXPECT_EQ(v.get<3>(), w.get<3>());
   EXPECT_EQ(v.get<4>(), w.get<4>());
   EXPECT_EQ(v.get<5>(), w.get<5>());


   w.set<1>(6.0f);
   EXPECT_EQ(6.0f, w.get<1>());

   v = w;

   EXPECT_EQ(v.get<0>(), w.get<0>());
   EXPECT_EQ(v.get<1>(), w.get<1>());
   EXPECT_EQ(v.get<2>(), w.get<2>());
   EXPECT_EQ(v.get<3>(), w.get<3>());
   EXPECT_EQ(v.get<4>(), w.get<4>());
   EXPECT_EQ(v.get<5>(), w.get<5>());

}



TEST(test_mv_sink, plus){
  using M = mv::MV<E2, NList<0,3>, double>;

  static constexpr const I i;
  static constexpr const One one;

  M v = 7 * one - 4 * i;
}
*/