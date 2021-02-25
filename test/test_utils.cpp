#include <gtest/gtest.h>
#include "../include/mv/utils.hpp" // #include <mv/nlist.hpp> // vs code cannt parse our CMakeLists.txt correctly
#include "../testutils/include/testutils/demangle.hpp"
#include <type_traits>
using cm::testing::demangle;

TEST(test_tlist, test_is_sorted) {
  using mv::is_sorted;
  using std::integral_constant;

  EXPECT_TRUE(is_sorted<int>({}));
  EXPECT_TRUE(is_sorted({1}));
  EXPECT_TRUE(is_sorted({1,2}));
  EXPECT_TRUE(is_sorted({1,2,3}));
  EXPECT_TRUE(is_sorted({1,2,2,3}));
  EXPECT_TRUE(is_sorted({1,1,2,3}));
  EXPECT_TRUE(is_sorted({1,2,3,3}));
  EXPECT_FALSE(is_sorted({2,1}));
  EXPECT_FALSE(is_sorted({2,3,2}));

  EXPECT_TRUE((integral_constant<bool, is_sorted({1,2,3})>{}));
}


TEST(test_tlist, test_is_strictly_sorted) {
  using mv::is_strictly_sorted;
  using std::integral_constant;

  EXPECT_TRUE(is_strictly_sorted<int>({}));
  EXPECT_TRUE(is_strictly_sorted({1}));
  EXPECT_TRUE(is_strictly_sorted({1,2}));
  EXPECT_TRUE(is_strictly_sorted({1,2,3}));
  EXPECT_FALSE(is_strictly_sorted({1,2,2,3}));
  EXPECT_FALSE(is_strictly_sorted({1,1,2,3}));
  EXPECT_FALSE(is_strictly_sorted({1,2,3,3}));
  EXPECT_FALSE(is_strictly_sorted({2,1}));
  EXPECT_FALSE(is_strictly_sorted({2,3,2}));

  EXPECT_TRUE((integral_constant<bool, is_strictly_sorted({1,2,3})>{}));
}
