#pragma once

// This files shows the expression concepts in terms of C++20 concepts

#if __cpluplus <= 201703

#include "nlist.hpp"

#include <type_traits>
#include <concept>

namespace mv
{
    namespace details
    {
        using BitSet = nlist::N;

        template <typename T> struct IsSortedNLIst : std::false_t{};

        template <>
        struct IsSortedNLIst<nlist::List<>> : std::true_t{};

        template <BitSet n>
        struct IsSortedNLIst<nlist::List<n>> : std::true_t{};

        template <BitSet n, BitSet m, BitSet... t>
        struct IsSortedNLIst<nlist::List<n,m,t...>> : std::conditional<n<m, Quote<true_type>, IsSortedNLits<m, t...>>::type{};

        template <typenam M>
        concept Metric = requires(M m, BitSet a, BitSet b)
        {
            typename value_type;
            is_convertible_<>
            constexpr value_type operator()(BitSet a, BitSet b) const; 
            std::declval<M>()(a,b) ==  std::declval<M>()(b,a);
            { m(a,b) } -> std::is_convertible_to<double>
        } 

    }

}

#endif