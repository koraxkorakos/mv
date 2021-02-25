#pragma once

#include "./nlist.hpp"

#include <type_traits>

namespace mv {

using B      = uint8_t;
using Zero_t = std::integral_constant<int, 0>;

template <nlist::N... t>
using BitSets = typename nlist::ListForm<
    nlist::Sort<nlist::ConsForm<nlist::List<t...>>>>::type;

template <typename S, typename D>
constexpr void
    copyElements(S const &, D &, nlist::List<>)
{
}

template <typename S, typename D, nlist::N h, nlist::N... t>
constexpr void
    copyElements(S const &s, D &d, nlist::List<h, t...>)
{
  d.template set<h>(s.template get<h>());
  copyElements(s, d, nlist::List<t...>{});
}

namespace details {
constexpr int
    incrementIfNonNegative(int a)
{
  return a < 0 ? a : a + 1;
}
} // namespace details

///\overload terminating recursion
constexpr std::size_t epos(nlist::N) { return 0; }

///\brief find position of first occurrence of element `n` in argument list
///`h,t...`
///\return position of first occurrence
///\retval `sizeof...(h,t...)` if not found
template <typename... T>
constexpr std::size_t
    epos(nlist::N n, nlist::N h, T... t)
{
  return n == h ? 0 : 1 + epos(n, t...);
}

///\overload
constexpr int sortedPos(nlist::N) { return -1; }

///\brief find position of first occurrence of `n` in ascending sorted argument
/// list `h,t...`
///\retval -1 if not found
///\pre `t...` sorted ascending
template <typename... T>
constexpr int
    sortedPos(nlist::N n, nlist::N h, T... t)
{
  return h > n
             ? -1
             : h == n ? 0 : details::incrementIfNonNegative(sortedPos(n, t...));
}

///\brief find number in list
///\note could stop early since sorted
template <nlist::N... t>
constexpr int
    sortedPos(nlist::N n, nlist::List<t...>)
{
  return sortedPos(n, t...);
}

} // namespace mv