//
//! Copyright © 2008-2009
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GENERATIVE_GEOMETRY_ADAPT_BOOST_ARRY_HPP
#define GENERATIVE_GEOMETRY_ADAPT_BOOST_ARRY_HPP
#pragma once

#include "../../indexed_access_traits.hpp"
#include <boost/fusion/include/array.hpp>
#include <boost/array.hpp>

namespace generative{ namespace numeric{ namespace geometry {                                   

template <typename T, unsigned int N>
struct indexed_access_policy< boost::array< T, N > >
: boost::integral_constant<indexed_sequence_access_type, prefer_compile_time_access_policy::value>
{}; 

template <typename T, unsigned int N>
struct is_indexed_sequence< boost::array< T, N > > : boost::true_type{};      

template <typename T, unsigned int N>
struct is_sequence< boost::array< T, N > > : boost::true_type{};            

template <typename T, unsigned int N>
struct sequence_traits< boost::array< T, N > >
{
    BOOST_STATIC_ASSERT( N > 0 );
    typedef boost::array< T, N > sequence_type;
    typedef T                    value_type;
    typedef dimension_traits<N>  dimension_type;
    typedef value_type&          reference;
    typedef const value_type&    const_reference;
};

}}} //namespace

#endif //GENERATIVE_GEOMETRY_ADAPT_BOOST_ARRY_HPP
