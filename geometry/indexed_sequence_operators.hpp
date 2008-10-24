//
//!  Copyright (c) 2008
//!  Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_GEOMETRY_INDEXED_SEQUENCE_OPERATORS_HPP
#define _BOOST_GEOMETRY_INDEXED_SEQUENCE_OPERATORS_HPP
#pragma once

#include "indexed_access_traits.hpp"
#include "detail/indexed_access_fusion_adaptor.hpp"
#include <boost/fusion/functional/adapter/fused_procedure.hpp>
#include <boost/utility.hpp>

namespace boost
{
namespace numeric
{
namespace geometry
{

namespace detail
{
    template <typename NumericType, typename Function>
    struct vector_summation
    {
        typedef NumericType result_type;

        vector_summation( const Function& f = Function() )
            : m_function( f )
        {}

        template <typename T>
        NumericType operator()( const T& t, const NumericType& sum ) const
        {
            return sum + m_function( t );
        }

        const Function& m_function;

    };
}//detail

//! \brief A utility class to generate a result type for binary operations which return return an indexed sequence.
template <typename IndexedSequence1, typename IndexedSequence2, typename Enable = void >
struct indexed_sequence_result_chooser
{
    //! Default is the first type (point + vector = point, vector + vector = vector).
    typedef IndexedSequence1 result_type;
};

// dot product operator.
template <typename IndexedSequence1, typename IndexedSequence2>
inline typename boost::lazy_enable_if_c
< 
    is_vector< IndexedSequence1 >::value                  &&
    is_indexed_sequence< IndexedSequence1 >::value        &&
    ( has_compile_time_access< IndexedSequence1 >::value  ||
      has_compile_time_access< IndexedSequence2 >::value )&&
    is_vector< IndexedSequence2 >::value                  &&
    is_indexed_sequence< IndexedSequence2 >::value, 
    boost::mpl::identity< typename sequence_traits<IndexedSequence1>::value_type >
>::type operator*( const IndexedSequence1& v1, const IndexedSequence2& v2 )
{
    typedef boost::fusion::vector< const_indexed_access_fusion_adaptor<IndexedSequence1>&, const_indexed_access_fusion_adaptor<IndexedSequence2>& > sequences;
    typedef typename sequence_traits<IndexedSequence1>::value_type result_type;
    return boost::fusion::accumulate( 
                                      boost::fusion::zip_view<sequences>( 
                                         sequences( const_indexed_access_fusion_adaptor<IndexedSequence1>(v1),
                                                    const_indexed_access_fusion_adaptor<IndexedSequence2>(v2) ) ),
                                      result_type(0),
                                      detail::vector_summation< result_type, boost::fusion::fused< std::multiplies<result_type> > >() );
}

//! \brief vector addition.
template <typename IndexedSequence1, typename IndexedSequence2>
inline typename boost::lazy_enable_if_c
<
    ( is_point< IndexedSequence1 >::value || is_vector< IndexedSequence1 >::value )   &&
      is_indexed_sequence< IndexedSequence1 >::value                                  &&
    ( has_compile_time_access< IndexedSequence1 >::value  ||
      has_compile_time_access< IndexedSequence2 >::value )&&
    is_vector< IndexedSequence2 >::value                                              &&
    is_indexed_sequence< IndexedSequence2 >::value,
    boost::mpl::identity< typename indexed_sequence_result_chooser<IndexedSequence1, IndexedSequence2>::result_type >
>::type operator+( const IndexedSequence1& v1, const IndexedSequence2& v2 )
{
    typedef typename indexed_sequence_result_chooser<IndexedSequence1, IndexedSequence2>::result_type result_type;
    result_type temp( construction_traits<result_type>::construct(v1) );
    typedef boost::fusion::vector< indexed_access_fusion_adaptor<result_type>&, const_indexed_access_fusion_adaptor<IndexedSequence2>& > sequences;
    boost::fusion::for_each( 
        boost::fusion::zip_view<sequences>( 
            sequences( indexed_access_fusion_adaptor<result_type>(temp),
                       const_indexed_access_fusion_adaptor<IndexedSequence2>(v2) ) ),
                       make_fused_procedure( boost::lambda::_1 += boost::lambda::_2 ) );
    return temp;
}

//! \brief vector subtraction.
template <typename IndexedSequence1, typename IndexedSequence2>
inline typename boost::lazy_enable_if_c
<
    ( is_point< IndexedSequence1 >::value || is_vector< IndexedSequence1 >::value )&&
      is_indexed_sequence< IndexedSequence1 >::value                               &&
    ( has_compile_time_access< IndexedSequence1 >::value ||
      has_compile_time_access< IndexedSequence2 >::value )                         &&
    is_vector< IndexedSequence2 >::value                                           &&
    is_indexed_sequence< IndexedSequence2 >::value,
    boost::mpl::identity< typename indexed_sequence_result_chooser<IndexedSequence1, IndexedSequence2>::result_type >
>::type operator-( const IndexedSequence1& v1, const IndexedSequence2& v2 )
{
    typedef typename indexed_sequence_result_chooser<IndexedSequence1, IndexedSequence2>::result_type result_type;
    result_type temp( construction_traits<result_type>::construct(v1) );
    typedef boost::fusion::vector< indexed_access_fusion_adaptor<result_type>&, const_indexed_access_fusion_adaptor<IndexedSequence2>& > sequences;
    boost::fusion::for_each( 
        boost::fusion::zip_view<sequences>( 
            sequences( indexed_access_fusion_adaptor<result_type>(temp),
                       const_indexed_access_fusion_adaptor<IndexedSequence2>(v2) ) ),
                       make_fused_procedure( boost::lambda::_1 -= boost::lambda::_2 ) );
    return temp;
}

//! \brief vector scalar multiplication.
template <typename IndexedSequence, typename NumericType>
inline typename boost::lazy_enable_if_c< is_vector< IndexedSequence >::value              &&
                                         is_indexed_sequence< IndexedSequence >::value    &&
                                         is_numeric< NumericType >::value                 &&
                                         has_compile_time_access< IndexedSequence >::value,
                                         boost::mpl::identity< IndexedSequence > 
>::type operator*( const IndexedSequence& v, const NumericType& s )
{
    IndexedSequence temp(v);    
    boost::fusion::for_each( indexed_access_fusion_adaptor<IndexedSequence>(temp), boost::lambda::_1 *= s );
    return temp;
}

//! \brief vector scalar division.
template <typename IndexedSequence, typename NumericType>
inline typename boost::lazy_enable_if_c< is_vector< IndexedSequence >::value            &&
                                         is_indexed_sequence< IndexedSequence >::value  &&
                                         is_numeric< NumericType >::value               &&
                                         has_compile_time_access<IndexedSequence>::value,
                                         boost::mpl::identity< IndexedSequence >
>::type operator/( const IndexedSequence& v, const NumericType& s )
{
    IndexedSequence temp(v);    
    boost::fusion::for_each( indexed_access_fusion_adaptor<IndexedSequence>(temp), boost::lambda::_1 /= s );
    return temp;
}

//! \brief Return the magnitude of the vector squared.
template <typename IndexedSequence>
inline typename boost::lazy_enable_if_c< is_vector< IndexedSequence >::value &&
                                         is_indexed_sequence< IndexedSequence >::value,
                                         boost::mpl::identity< typename sequence_traits<IndexedSequence>::value_type >
>::type magnitude_squared( const IndexedSequence& v )
{
    return v * v;
}

//! \brief Return the magnitude of the vector.
//! \brief Return the magnitude of the vector squared.
template <typename IndexedSequence>
inline typename boost::lazy_enable_if_c< is_vector< IndexedSequence >::value &&
                                         is_indexed_sequence< IndexedSequence >::value,
                                         boost::mpl::identity< typename sequence_traits<IndexedSequence>::value_type >
>::type magnitude( const IndexedSequence& v )
{
    typedef typename sequence_traits<IndexedSequence>::value_type result_type;
    return math_functions<result_type>::sqrt( magnitude_squared( v ) );
}

//! \brief Return the magnitude of the vector.
template <typename IndexedSequence>
inline typename boost::lazy_enable_if_c< is_vector< IndexedSequence >::value &&
                                         is_indexed_sequence< IndexedSequence >::value,
                                         boost::mpl::identity< IndexedSequence >
>::type normalize( const IndexedSequence& v )
{
    return ( v / magnitude( v ) );
}

}}}//namespace boost::numeric::geometry

#endif //_BOOST_GEOMETRY_INDEXED_SEQUENCE_OPERATORS_HPP
