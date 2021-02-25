#pragma once

#include <ratio>
#include "meta.hpp"

namespace mv
{
    // a rational constant similar to `std::integral_constant`
    template <typename S, std::intmax_t n, std::intmax_t d>
    struct RationalConstant 
    {
        using Reduced = typename std::ratio<n,d>::type;
        using type = RationalConstant<S, Reduced::num, Reduced::den>;
        using value_type = S;

        static constexpr value_type value = value_type(Reduced::num) / value_type(Reduced::den);

        constexpr operator value_type() const  { return value; }
        constexpr value_type operator()() const  { return value; };         
    };

    template <typename S, std::intmax_t n, std::intmax_t d>
    constexpr auto operator+(RationalConstant<S,n,d> a)
    {
       return a;    
    } 

    ///\note does not check for overflow (`n == std::numeric_limits<std::intmax_t>::min()`)
    template <typename S, std::intmax_t n, std::intmax_t d>
    constexpr auto operator-(RationalConstant<S,n,d>)
    {
       return typename RationalConstant<S,-n,d>::type{};    
    } 

    ///\note does not check for overflow
    template <typename S, std::intmax_t n1, std::intmax_t d1, std::intmax_t n2, std::intmax_t d2>
    constexpr auto operator+(RationalConstant<S,n1,d1>, RationalConstant<S,n2,d2>)
    {
       return typename RationalConstant<S,n1*d2+d1*n2, d1*d2>::type{};    
    } 

    ///\note does not check for overflow
    template <typename S, std::intmax_t n1, std::intmax_t d1, std::intmax_t n2, std::intmax_t d2>
    constexpr auto operator-(RationalConstant<S,n1,d1>, RationalConstant<S,n2,d2>)
    {       
       return typename RationalConstant<S,n1*d2-d1*n2, d1*d2>::type{};    
    } 

    ///\note does not check for overflow
    template <typename S, std::intmax_t n1, std::intmax_t d1, std::intmax_t n2, std::intmax_t d2>
    constexpr auto operator*(RationalConstant<S,n1,d1> a, RationalConstant<S,n2,d2> b)
    {
       return typename RationalConstant<S,(+a).num*(+b).den,(+a).den*(+b).num>::type{};    
    } 

    ///\pre `d != 0`
    ///\note does not check for overflow
    template <typename S, std::intmax_t n1, std::intmax_t d1, std::intmax_t n2, std::intmax_t d2>
    constexpr auto operator/(RationalConstant<S,n1,d1> a, RationalConstant<S,n2,d2>)
    {
       return +a * typename RationalConstant<S,d2,n2>::type{};    
    } 

    ///\note does not check for overflow
    template <typename S, std::intmax_t n1, std::intmax_t d1, std::intmax_t n2, std::intmax_t d2>
    constexpr auto operator==(RationalConstant<S,n1,d1> a, RationalConstant<S,n2,d2> b)
    {
       return +a == +b;    
    }

    ///\pre `d != 0`
    ///\note does not check for overflow
    template <typename S, std::intmax_t n, std::intmax_t d>
    constexpr auto operator==(RationalConstant<S,n,d>, RationalConstant<S,n,d>)
    {
       return true;    
    }    

    ///\note does not check for overflow
    template <typename S, std::intmax_t n1, std::intmax_t d1, std::intmax_t n2, std::intmax_t d2>
    constexpr auto operator!=(RationalConstant<S,n1,d1> a, RationalConstant<S,n2,d2> b)
    {
       return !(a==b); 
    }

    ///\note does not check for overflow
    template <typename S, std::intmax_t n1, std::intmax_t d1, std::intmax_t n2, std::intmax_t d2>
    constexpr auto operator<(RationalConstant<S,n1,d1> a, RationalConstant<S,n2,d2> b)
    {
       return (+a).num * (+b).den < (+b).num * (+a).den;
    }

    ///\note does not check for overflow
    template <typename S, std::intmax_t n1, std::intmax_t d1, std::intmax_t n2, std::intmax_t d2>
    constexpr auto operator>(RationalConstant<S,n1,d1> a, RationalConstant<S,n2,d2> b)
    {
       return +b < +a;
    }

    ///\note does not check for overflow
    template <typename S, std::intmax_t n1, std::intmax_t d1, std::intmax_t n2, std::intmax_t d2>
    constexpr auto operator<=(RationalConstant<S,n1,d1> a, RationalConstant<S,n2,d2> b)
    {
       return !(+a > +b);
    }

    ///\note does not check for overflow
    template <typename S, std::intmax_t n1, std::intmax_t d1, std::intmax_t n2, std::intmax_t d2>
    constexpr auto operator>=(RationalConstant<S,n1,d1> a, RationalConstant<S,n2,d2> b)
    {
       return !(+a < +b);
    }

    ///\pre `d != 0`
    template <typename S, std::intmax_t n, std::intmax_t d>
    constexpr auto operator!(RationalConstant<S,n,d>)
    {
       return n != 0;
    } 
}