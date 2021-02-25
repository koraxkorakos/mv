#include <gtest/gtest.h>
#include "../include/mv/nlist.hpp" // #include <mv/nlist.hpp> // vs code cannt parse our CMakeLists.txt correctly
#include "../testutils/include/testutils/demangle.hpp"

using std::is_same;

using cm::testing::demangle;

TEST(test_tlist, test_nil) {
  using mv::NIL;

  EXPECT_TRUE((is_same<NIL, NIL::type>::value));
  EXPECT_TRUE((is_same<NIL, NIL::type>::value));
}

TEST(test_tlist, test_tlist_cons) {
  using mv::NIL;
  using mv::nlist::Cons;

  EXPECT_EQ(5, (Cons<5>::head()));
  EXPECT_TRUE((is_same<NIL, Cons<5>::tail>::value));
  EXPECT_TRUE((is_same<Cons<5>, Cons<5>::type>::value));

  EXPECT_EQ(5, (Cons<5, Cons<2>>::head()));
  EXPECT_EQ(2, (Cons<2, Cons<2>>::tail::head()));
  EXPECT_TRUE((is_same<NIL, Cons<5, Cons<2>>::tail::tail>::value));
  EXPECT_TRUE((is_same<Cons<2>, Cons<5, Cons<2>>::tail>::value));
}

TEST(test_tlist, test_odd_even_strict) {
  using mv::NIL;
  using mv::nlist::Cons;
  using mv::Even;
  using mv::Odd;
  using mv::Strict;

  EXPECT_TRUE((is_same<NIL, Odd<NIL>::type>::value));
  EXPECT_TRUE((is_same<NIL, Even<NIL>::type>::value));

  EXPECT_TRUE((is_same<Cons<7>, Strict<Even<Cons<7>>>::type>::value))
      << demangle(typeid(Even<Cons<7>>::type).name());
  EXPECT_TRUE((is_same<NIL, Odd<Cons<7>>::type>::value))
      << demangle(typeid(Odd<Cons<7>>::type).name());

  EXPECT_TRUE((is_same<Cons<7, Cons<1>>,
                       Strict<Even<Cons<7, Cons<5, Cons<1>>>>>::type>::value))
      << demangle(typeid(Strict<Even<Cons<7, Cons<5, Cons<1>>>>>::type).name());
  EXPECT_TRUE(
      (is_same<Cons<5>, Strict<Odd<Cons<7, Cons<5, Cons<1>>>>>::type>::value))
      << demangle(typeid(Strict<Odd<Cons<7, Cons<5, Cons<1>>>>>::type).name());

  // lazyness
  EXPECT_TRUE((is_same<Cons<7, Odd<Cons<5, Cons<1>>>>,
                       Even<Cons<7, Cons<5, Cons<1>>>>::type>::value))
      << demangle(typeid(Even<Cons<7, Cons<5, Cons<1>>>>::type).name());
  ;
  EXPECT_TRUE((is_same<Cons<5, Odd<Cons<1>>>,
                       Odd<Cons<7, Cons<5, Cons<1>>>>::type>::value))
      << demangle(typeid(Cons<5, Odd<Cons<1>>>::type).name());
  ;
}

TEST(test_tlist, test_merge) {
  using mv::NIL;
  using mv::nlist::Cons;
  using mv::Even;
  using mv::Odd;
  using mv::Strict;
  using mv::nlist::Merge;

  using L = Cons<1, Cons<3, Cons<7, Cons<9, Cons<11>>>>>;
  EXPECT_TRUE((is_same<Merge<NIL, NIL>::type, NIL>::value));

  EXPECT_TRUE((is_same<Merge<L, NIL>::type, L>::value));
  EXPECT_TRUE((is_same<Merge<NIL, L>::type, L>::value));

  EXPECT_EQ(1, (Merge<Odd<L>, Even<L>>::type::head()));
  EXPECT_EQ(1, (Merge<Even<L>, Odd<L>>::type::head()));

  EXPECT_TRUE((is_same<L, Strict<Merge<Odd<L>, Even<L>>>::type>::value));
  EXPECT_TRUE((is_same<L, Strict<Merge<Even<L>, Odd<L>>>::type>::value));

  // lazyness
  EXPECT_FALSE((is_same<Merge<Odd<L>, Even<L>>::type,
                        Strict<Merge<Odd<L>, Even<L>>>::type>::value));
}

TEST(test_tlist, test_sort) {
  using mv::NIL;
  using mv::nlist::Cons;
  using mv::Even;
  using mv::Odd;
  using mv::Strict;
  using mv::nlist::Merge;
  using mv::nlist::Sort;

  // using SL = Cons<1, Cons<3, Cons<7, Cons<9, Cons<11>>>>>;
  // using L = Cons<3, Cons<3, Cons<1, Cons<7, Cons<11, Cons<9>>>>>>;

  using SL = Cons<1, Cons<3>>;
  using L = Cons<3, Cons<3, Cons<1>>>;

  EXPECT_TRUE((is_same<NIL, Sort<NIL>::type>::value));
  EXPECT_TRUE((is_same<Cons<6>, Sort<Cons<6>>::type>::value));

  EXPECT_TRUE((is_same<SL, Strict<Sort<L>>::type>::value))
      << demangle(typeid(Sort<L>::type).name());

  EXPECT_EQ(1, (Sort<L>::type::head()));

  // lazyness
  EXPECT_FALSE((is_same<Strict<Sort<L>>::type, Sort<L>::type>::value));
}

TEST(test_tlist, test_list) {
  using mv::NIL;
  using mv::nlist::Cons;
  using mv::Even;
  using mv::Odd;
  using mv::Strict;
  using mv::nlist::Merge;
  using mv::nlist::Sort;
  using mv::nlist::List;
  using mv::nlist::ListForm;
  using mv::nlist::PushFront;

  EXPECT_TRUE((is_same<List<>, ListForm<NIL>::type>::value));
  EXPECT_TRUE((is_same<List<3>, ListForm<Cons<3>>::type>::value));
  EXPECT_TRUE((is_same<List<3, 1>, ListForm<Cons<3, Cons<1>>>::type>::value));
  EXPECT_TRUE(
      (is_same<List<1, 3>, ListForm<Sort<Cons<3, Cons<1>>>>::type>::value));

  EXPECT_TRUE((is_same<List<5, 3, 7>, PushFront<5, List<3, 7>>::type>::value));
}
