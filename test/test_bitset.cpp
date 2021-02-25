#include <gtest/gtest.h>

#include <algorithm>
#include <array>
#include <initializer_list>

#include "../include/mv/bitset.hpp" // #include <mv/bitset.hpp> // viszalk studio code extensions fley analyses cannat parse CMakeLists.txt
#include <testutils/demangle.hpp>

using std::is_same;

using cm::testing::demangle;
using std::array;
using std::initializer_list;

using mv::nlist::N;

namespace
{
    template <int n>
    struct Array : public std::array<int, n>
    {
        using Inherited = std::array<int, n>;        
        Inherited &base() noexcept{ return *this; }
        Inherited const &base() const noexcept{ return *this; }
    public:
        explicit Array(initializer_list<int> l) noexcept 
        { 
            std::copy(l.begin(), //
                      l.begin() + std::min(l.size(), //
                      this->size()), this->begin()); }
        template <size_t k>
        Array &set(N v) noexcept
        {
            base()[k] = v;
            return *this;
        }
        template <size_t k>
        int get() const noexcept
        {
            return base()[k];
        }        
    };
}

TEST(test_bitset, test_copy_elements) {
  using mv::copyElements;

  Array<5> const a{1, 3, -7, -9, 6 };
  Array<4> b{0, 0, 0, 0};
  Array<4> c{0, 0, 0, 0};

  copyElements(a,b, mv::nlist::List<0, 2, 3>{});
  EXPECT_EQ(a[0], b[0]);
  EXPECT_EQ(0, b[1]);
  EXPECT_EQ(a[2], b[2]);
  EXPECT_EQ(a[3], b[3]);

  copyElements(a,c,mv::nlist::List<>{});
  EXPECT_EQ(0, c[0]);
  EXPECT_EQ(0, c[1]);
  EXPECT_EQ(0, c[2]);
  EXPECT_EQ(0, c[3]);
}

TEST(test_bitset, test_epos) {
  using mv::epos;

  EXPECT_EQ(0, epos(3));

  EXPECT_EQ(4, epos(3, 1,7,7,2));
  EXPECT_EQ(3, epos(2, 1,7,7,2));
  EXPECT_EQ(1, epos(7, 1,7,7,2));
  EXPECT_EQ(0, epos(1, 1,7,7,2));
}

TEST(test_bitset, test_sortedPos) {
  using mv::sortedPos;

  EXPECT_EQ(-1, sortedPos(3));

  EXPECT_EQ(-1, sortedPos(3, 1,7,7,8));
  EXPECT_EQ(3, sortedPos(8, 1,7,7,8));
  EXPECT_EQ(1, sortedPos(7, 1,7,7,8));
  EXPECT_EQ(0, sortedPos(1, 1,7,7,8));
}


/*
TEST(test, test_copy_elements_using_bitset) {
  using mv::NIL;

  EXPECT_TRUE((is_same<NIL, NIL::type>::value));
  EXPECT_TRUE((is_same<NIL, NIL::type>::value));
}
*/