#pragma once

#include <concepts>
#include <cstdint>
#include <type_traits>

#include <mv/utils.hpp>

namespace mv {

namespace be {

using bases = std::uintmax_t;

// list of bases
template <bases... t> struct list {
  using type = list;
};

namespace details {
template <typename> constexpr bool is_list  = false;
template <typename> constexpr bool is_sorted_list = false;
template <typename> constexpr bool is_strictly_sorted_list = false;

template <bases... t> constexpr bool is_list<list<t...>> = true;

template <bases... t>
constexpr bool is_strictly_sorted_list<list<t...>> = is_strictly_sorted<bases>({t...});

template <bases... t>
constexpr bool is_sorted_list<list<t...>> = is_sorted<bases>({t...});
} // namespace details

template <typename T> concept strictly_sorted_list = details::is_list<T> && details::is_strictly_sorted_list<T>;

template <typename T>
concept proper_list = details::is_list<T> && !strictly_sorted_list<T>;

template <typename T>
concept basis_blades = strictly_sorted_list<T> && !std::is_same_v<T, list<>>;

template <bases h, typename T>
struct push_front: push_front<h, typename T::type> {
};

template <bases h, typename l> 
using push_front_t = typename push_front<h,l>::type;

template <typename T> struct sort: sort<typename T::type> {
};

template <typename l> 
using sort_t = typename sort<l>::type;

template <typename A, typename B>
struct merge: merge<sort_t<A>, sort_t<B>> {
};

template <typename A, typename B>
using merge_t = typename merge<A,B>::type;

/// sorting is not costly, because  1 split of the 2 splits will already be sorted
/// thanks to concept check, which is just a `constexpr` function call
template <bases a, typename T>
struct insert: insert<a, typename T::type> {
};

template <bases a, typename T>
using insert_t = typename insert<a, T>::type;

template <bases a, bases... t>
struct insert<a, list<t...>> : sort<push_front<a,list<t...>>>  {
};

template <typename T> struct odd: odd<typename T::type> {
};

template <typename T> using odd_t = typename odd<T>::type;

template <typename T> struct even: even<typename T::type> {
};

template <typename T> using even_t = typename even<T>::type;


// --- specializations ---
template <bases h, bases... t>
struct push_front<h, list<t...>>: list<h, t...> {
};

template <> struct merge<list<>, list<>>: list<> {
};

template <typename T> struct merge<list<>, T>: sort<T> {
};

template <typename T> struct merge<T, list<>>: sort<T> {
};

template <bases ah, bases... at, bases... bt> 
  requires(strictly_sorted_list<list<ah,at...>> && strictly_sorted_list<list<ah,bt...>>)
struct merge<list<ah, at...>, list<ah, bt...>> :
merge<list<ah,at...>, list<bt...>>{};

template <bases ah, bases bh, bases... at, bases... bt> 
  requires(strictly_sorted_list<list<ah,at...>> && strictly_sorted_list<list<bh,bt...>>)
struct merge<list<ah, at...>, list<bh, bt...>>
    : push_front<
          (ah < bh ? ah : bh),
          std::conditional<(ah < bh),                       //
                            merge<list<at...>, list<bh, bt...>>, //
                            merge<list<ah, at...>, list<bt...>>>> {
};

template <> struct odd<list<>>: list<> {
};

template <> struct even<list<>>: list<> {
};

template <bases h> struct odd<list<h>>: list<> {
};

template <bases h> struct even<list<h>>: list<h> {
};

template <bases h, bases... t> struct odd<list<h, t...>>: even<list<t...>> {
};

template <bases h, bases... t>
struct even<list<h, t...>>: push_front<h, odd<list<t...>>> {
};

template <strictly_sorted_list s> struct sort<s>: s {
};

template <proper_list l> struct sort<l>: merge<sort_t<even_t<l>>, sort_t<odd_t<l>>> {
};


} // namespace be
} // namespace mv