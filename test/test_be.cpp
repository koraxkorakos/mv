#include <gtest/gtest.h>

#include <algorithm>
#include <array>
#include <initializer_list>

#include <mv/be.hpp>
#include <testutils/demangle.hpp>

namespace be = mv::be;

using cm::testing::demangle;
using cm::testing::demangle_t;

TEST(test_be, test_concepts) {
   using be::strictly_sorted_list;
   using be::proper_list;
   using be::basis_blades;
   using be::list;

   EXPECT_TRUE(strictly_sorted_list<list<>>);
   EXPECT_FALSE(proper_list<list<>>);
   EXPECT_FALSE(basis_blades<list<>>);

   EXPECT_TRUE(strictly_sorted_list<list<0>>);
   EXPECT_FALSE(proper_list<list<0>>);
   EXPECT_TRUE(basis_blades<list<0>>);

   EXPECT_TRUE((strictly_sorted_list<list<0,1>>));
   EXPECT_FALSE((proper_list<list<0,1>>));
   EXPECT_TRUE((basis_blades<list<0,1>>));

   EXPECT_FALSE((strictly_sorted_list<list<1,0>>));
   EXPECT_TRUE((proper_list<list<1,0>>));
   EXPECT_FALSE((basis_blades<list<1,0>>));

   EXPECT_TRUE((strictly_sorted_list<list<1,3,9>>));
   EXPECT_FALSE((proper_list<list<1,3,9>>));
   EXPECT_TRUE((basis_blades<list<1,3,9>>));

   EXPECT_FALSE((strictly_sorted_list<list<0,0>>));
   EXPECT_TRUE((proper_list<list<0,0>>));
   EXPECT_FALSE((basis_blades<list<0,0>>));

   EXPECT_FALSE((strictly_sorted_list<list<0,1,1>>));
   EXPECT_TRUE((proper_list<list<0,1,1>>));
   EXPECT_FALSE((basis_blades<list<0,1,1>>));

   EXPECT_FALSE((strictly_sorted_list<list<0,1,1,4>>));
   EXPECT_TRUE((proper_list<list<0,1,1,4>>));
   EXPECT_FALSE((basis_blades<list<0,1,1,4>>));
}

TEST(test_be, test_basis_blades) {

   using std::is_same_v;
   using be::list;
   using be::sort_t;

   EXPECT_TRUE((is_same_v<list<>, sort_t<list<>>>)) << "got: " << demangle_t<sort_t<list<>>>();
   EXPECT_TRUE((is_same_v<list<0>, sort_t<list<0>>>)) << "got: " << demangle_t<sort_t<list<0>>>();
   EXPECT_TRUE((is_same_v<list<0>, sort_t<list<0,0>>>))<< "got: " << demangle_t<sort_t<list<0,0>>>();
   EXPECT_TRUE((is_same_v<list<0,1>, sort_t<list<0,1>>>))<< "got: " << demangle_t<sort_t<list<0,1>>>();
   EXPECT_TRUE((is_same_v<list<0,1>, sort_t<list<1,0>>>))<< "got: " << demangle_t<sort_t<list<1,0>>>();
   EXPECT_TRUE((is_same_v<list<0,1>, sort_t<list<0,1,1>>>))<< "got: " << demangle_t<sort_t<list<0,1,1>>>();
   EXPECT_TRUE((is_same_v<list<0,1>, sort_t<list<1,1,0>>>))<< "got: " << demangle_t<sort_t<list<1,1,0>>>();
   EXPECT_TRUE((is_same_v<list<1,2,3>, sort_t<list<2,3,1>>>))<< "got: " << demangle_t<sort_t<list<2,3,1>>>();
   EXPECT_TRUE((is_same_v<list<1,2,3>, sort_t<list<2,1,1,3>>>))<< "got: " << demangle_t<sort_t<list<2,1,1,3>>>();
}