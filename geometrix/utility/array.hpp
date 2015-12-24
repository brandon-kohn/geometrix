//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_ADAPT_BOOST_ARRY_HPP
#define GEOMETRIX_ADAPT_BOOST_ARRY_HPP

#include <geometrix/tensor/tensor_traits.hpp>
#include <geometrix/tensor/index_operator_vector_access_policy.hpp>
#include <geometrix/space/dimension.hpp>

#include <boost/fusion/include/array.hpp>
#include <boost/array.hpp>

namespace geometrix {

template <typename T, std::size_t N>
struct tensor_traits< boost::array<T, N> >
{
    typedef index_operator_vector_access_policy< boost::array<T,N> > access_policy;
    typedef boost::mpl::int_<1>                                      tensor_order;
    typedef void                                                     rank_1;
    typedef void                                                     is_tensor;
};

template <typename T, std::size_t N>
struct geometric_traits< boost::array<T, N> >
{
    GEOMETRIX_STATIC_ASSERT( N > 0 );
    typedef boost::array<T, N>    sequence_type;
    typedef boost::mpl::vector<T> storage_types;
    typedef dimension<N>          dimension_type;
    typedef void                  is_sequence;
    typedef void                  is_homogeneous;
};

}//namespace geometrix;


#endif //GEOMETRIX_ADAPT_BOOST_ARRY_HPP
