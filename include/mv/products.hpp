#pragma once

#include <type_traits>

namespace mv {
template <BitSet n, typename A1, typename A2>
std::common_type_t<typename A1::value_type, typename A2::value_type>
    outer_product(A1 const &, A2 const &);
template <BitSet n, typename A1, typename A2>
std::common_type_t<typename A1::value_type, typename A2::value_type>
    inner_product(A1 const &, A2 const &);

/// TEMPLATE IMPLEMENTATION ////////////////////////////////////////////////////////////////////////////////



} // namespace mv