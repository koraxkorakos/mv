#pragma once

#include <algorithm>
#include <initializer_list>

namespace mv {
template <typename T>
constexpr bool
    is_sorted(std::initializer_list<T> l)
{
    using std::begin;
    using std::end;
    return std::is_sorted(begin(l), end(l));
}

template <typename T>
constexpr bool
    is_strictly_sorted(std::initializer_list<T> l)
{
    using std::begin;
    using std::end;
    if (begin(l) == end(l)) return true;
    auto p = begin(l);
    for(auto b=p; ++b != end(l);p=b)
    {
        if (!(*p < *b)) return false;
    } 
    return true;
}
} // namespace mv