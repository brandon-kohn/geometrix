//
//! Copyright © 2008-2009
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _GENERATIVE_GEOMETRY_INDEXED_SEQUENCE_OPERATORS_HPP
#define _GENERATIVE_GEOMETRY_INDEXED_SEQUENCE_OPERATORS_HPP
#pragma once

#include "indexed_access_traits.hpp"
//#include "detail/indexed_access_fusion_adaptor.hpp"
#include <boost/fusion/functional/adapter/fused_procedure.hpp>
#include <boost/utility.hpp>

namespace generative
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
    
    //! \brief A utility class to generate a result type for binary operations which return return an indexed sequence.
    template <typename IndexedSequence1, typename IndexedSequence2, typename Enable = void >
    struct indexed_sequence_result_chooser
    {
        //! Default is the first type (point + vector = point, vector + vector = vector).
        typedef IndexedSequence1 result_type;
    };

}//detail

//! Compile-time operations

// dot product operator.
template <typename IndexedSequence1, typename IndexedSequence2>
inline typename boost::lazy_enable_if_c
< 
    is_vector< IndexedSequence1 >::value                                               &&
    is_indexed_sequence< IndexedSequence1 >::value                                     &&
    should_use_compile_time_access2< IndexedSequence1, IndexedSequence2 >::type::value &&
    is_vector< IndexedSequence2 >::value                                               &&
    is_indexed_sequence< IndexedSequence2 >::value, 
    boost::mpl::identity< typename sequence_traits<IndexedSequence1>::value_type >
>::type operator*( const IndexedSequence1& v1, const IndexedSequence2& v2 )
{
    typedef boost::fusion::vector< IndexedSequence1&, IndexedSequence2& > sequences;
    typedef typename sequence_traits<IndexedSequence1>::value_type result_type;
    return boost::fusion::accumulate( 
                                      boost::fusion::zip_view<sequences>( 
                                         sequences( IndexedSequence1(v1),
                                                    IndexedSequence2(v2) ) ),
                                      result_type(0),
                                      detail::vector_summation< result_type, boost::fusion::fused< std::multiplies<result_type> > >() );
}

//! \brief vector addition.
template <typename IndexedSequence1, typename IndexedSequence2>
inline typename boost::enable_if_c
<
    ( is_point< IndexedSequence1 >::value || is_vector< IndexedSequence1 >::value )    &&
    is_indexed_sequence< IndexedSequence1 >::value                                     &&
    should_use_compile_time_access2< IndexedSequence1, IndexedSequence2 >::type::value &&
    is_vector< IndexedSequence2 >::value                                               &&
    !is_reference_frame_tag< IndexedSequence1 >::value &&
    !is_reference_frame_tag< IndexedSequence2 >::value &&
    is_indexed_sequence< IndexedSequence2 >::value,
    typename detail::indexed_sequence_result_chooser<IndexedSequence1, IndexedSequence2>::result_type
>::type operator+( const IndexedSequence1& v1, const IndexedSequence2& v2 )
{
 
    typedef typename detail::indexed_sequence_result_chooser<IndexedSequence1, IndexedSequence2>::result_type result_type;
    typedef typename indexed_access_traits< result_type >::value_type value_type;
    typedef typename indexed_access_traits< result_type >::dimension_type dimension_type;
    boost::array< value_type, dimension_type::value > temp = make_initialized_array( v1 );

    typedef boost::fusion::vector< boost::array< value_type, dimension_type::value >&, const IndexedSequence2& > sequences;
    boost::fusion::for_each( 
        boost::fusion::zip_view<sequences>( 
        sequences( temp,
        v2 ) ),
        make_fused_procedure( boost::lambda::_1 += boost::lambda::_2 ) );

    return construction_traits<result_type>::construct( temp );
}

//! \brief vector subtraction.
template <typename IndexedSequence1, typename IndexedSequence2>
inline typename boost::enable_if_c
<
    ( is_point< IndexedSequence1 >::value || is_vector< IndexedSequence1 >::value )    &&
    is_indexed_sequence< IndexedSequence1 >::value                                     &&
    should_use_compile_time_access2< IndexedSequence1, IndexedSequence2 >::type::value &&
    is_vector< IndexedSequence2 >::value                                               &&
    !is_reference_frame_tag< IndexedSequence1 >::value &&
    !is_reference_frame_tag< IndexedSequence2 >::value &&
    is_indexed_sequence< IndexedSequence2 >::value,
    typename detail::indexed_sequence_result_chooser<IndexedSequence1, IndexedSequence2>::result_type
>::type operator-( const IndexedSequence1& v1, const IndexedSequence2& v2 )
{
    typedef typename detail::indexed_sequence_result_chooser<IndexedSequence1, IndexedSequence2>::result_type result_type;
    typedef typename indexed_access_traits< result_type >::value_type value_type;
    typedef typename indexed_access_traits< result_type >::dimension_type dimension_type;
    boost::array< value_type, dimension_type::value > temp = make_initialized_array( v1 );

    typedef boost::fusion::vector< boost::array< value_type, dimension_type::value >&, const IndexedSequence2& > sequences;
    boost::fusion::for_each( 
        boost::fusion::zip_view<sequences>( 
        sequences( temp,
        v2 ) ),
        make_fused_procedure( boost::lambda::_1 -= boost::lambda::_2 ) );

    return construction_traits<result_type>::construct( temp );
}

//! \brief vector scalar multiplication.
template <typename IndexedSequence, typename NumericType>
inline typename boost::enable_if_c< is_vector< IndexedSequence >::value              &&
                                    is_indexed_sequence< IndexedSequence >::value    &&
                                    is_numeric< NumericType >::value                 &&
                                    !is_reference_frame_tag< IndexedSequence >::value &&
                                    should_use_compile_time_access1< IndexedSequence >::type::value,
IndexedSequence >::type operator*( const IndexedSequence& v, const NumericType& s )
{
    typedef typename indexed_access_traits< IndexedSequence >::value_type value_type;
    typedef typename indexed_access_traits< IndexedSequence >::dimension_type dimension_type;
    boost::array< value_type, dimension_type::value > temp = make_initialized_array( v );

    boost::fusion::for_each( temp, boost::lambda::_1 *= s );

    return construction_traits< IndexedSequence >::construct( temp );
}

template <typename IndexedSequence, typename NumericType>
inline typename boost::enable_if_c< is_vector< IndexedSequence >::value              &&
                                    is_indexed_sequence< IndexedSequence >::value    &&
                                    is_numeric< NumericType >::value                 &&
                                    !is_reference_frame_tag< IndexedSequence >::value &&
                                    should_use_compile_time_access1< IndexedSequence >::type::value,
IndexedSequence >::type operator*( const NumericType& s, const IndexedSequence& v )
{
    typedef typename indexed_access_traits< IndexedSequence >::value_type value_type;
    typedef typename indexed_access_traits< IndexedSequence >::dimension_type dimension_type;
    boost::array< value_type, dimension_type::value > temp = make_initialized_array( v );

    boost::fusion::for_each( temp, boost::lambda::_1 *= s );

    return construction_traits< IndexedSequence >::construct( temp );
}

//! \brief vector scalar division.
template <typename IndexedSequence, typename NumericType>
inline typename boost::enable_if_c< is_vector< IndexedSequence >::value            &&
                                    is_indexed_sequence< IndexedSequence >::value  &&
                                    is_numeric< NumericType >::value               &&
                                    !is_reference_frame_tag< IndexedSequence >::value &&
                                    should_use_compile_time_access1< IndexedSequence >::type::value,
                                    IndexedSequence
>::type operator/( const IndexedSequence& v, const NumericType& s )
{
    typedef typename indexed_access_traits< IndexedSequence >::value_type value_type;
    typedef typename indexed_access_traits< IndexedSequence >::dimension_type dimension_type;
    boost::array< value_type, dimension_type::value > temp = make_initialized_array( v );

    boost::fusion::for_each( temp, boost::lambda::_1 /= s );

    return construction_traits<IndexedSequence>::construct( temp );
}

//! Run-time operations

//! \brief Run-time dot product operator.
template <typename IndexedSequence1, typename IndexedSequence2>
inline typename boost::lazy_enable_if_c
< 
    is_vector< IndexedSequence1 >::value                                             &&
    is_indexed_sequence< IndexedSequence1 >::value                                   &&
    should_use_run_time_access2< IndexedSequence1, IndexedSequence2 >::type::value   &&
    is_vector< IndexedSequence2 >::value                                             &&
    is_indexed_sequence< IndexedSequence2 >::value, 
    boost::mpl::identity< typename sequence_traits<IndexedSequence1>::value_type >
>::type operator*( const IndexedSequence1& v1, const IndexedSequence2& v2 )
{
    typedef typename sequence_traits<IndexedSequence1>::value_type result_type;
    result_type sum(0);
    for( std::size_t i = 0;i < sequence_traits< IndexedSequence1 >::dimension_type::value; ++i )
    {
        sum += indexed_access_traits<IndexedSequence1>::get( v1, i ) * indexed_access_traits<IndexedSequence2>::get( v2, i );
    }
    return sum;
}

//! \brief Run-time addition operator.
template <typename IndexedSequence1, typename IndexedSequence2>
inline typename boost::enable_if_c
<
    ( is_point< IndexedSequence1 >::value || is_vector< IndexedSequence1 >::value )  &&
    is_indexed_sequence< IndexedSequence1 >::value                                   &&
    should_use_run_time_access2< IndexedSequence1, IndexedSequence2 >::type::value   &&
    is_vector< IndexedSequence2 >::value                                             &&
    is_indexed_sequence< IndexedSequence2 >::value,
    typename detail::indexed_sequence_result_chooser<IndexedSequence1, IndexedSequence2>::result_type
>::type operator+( const IndexedSequence1& v1, const IndexedSequence2& v2 )
{
    typedef typename detail::indexed_sequence_result_chooser<IndexedSequence1, IndexedSequence2>::result_type result_type;
    typedef typename indexed_access_traits< result_type >::value_type value_type;
    typedef typename indexed_access_traits< result_type >::dimension_type dimension_type;
    boost::array< value_type, dimension_type::value > temp;
    for( std::size_t i = 0;i < sequence_traits< IndexedSequence1 >::dimension_type::value; ++i )
    {
         temp[i] = indexed_access_traits<result_type>::get( v1, i ) + indexed_access_traits<IndexedSequence2>::get( v2, i );
    }

    return construction_traits<result_type>::construct( temp );
}

//! \brief Run-time subtraction operator.
template <typename IndexedSequence1, typename IndexedSequence2>
inline typename boost::enable_if_c
<
    ( is_point< IndexedSequence1 >::value || is_vector< IndexedSequence1 >::value )  &&
    is_indexed_sequence< IndexedSequence1 >::value                                   &&
    should_use_run_time_access2< IndexedSequence1, IndexedSequence2 >::type::value   &&
    is_vector< IndexedSequence2 >::value                                             &&
    is_indexed_sequence< IndexedSequence2 >::value,
    typename detail::indexed_sequence_result_chooser<IndexedSequence1, IndexedSequence2>::result_type
>::type operator-( const IndexedSequence1& v1, const IndexedSequence2& v2 )
{
    typedef typename detail::indexed_sequence_result_chooser<IndexedSequence1, IndexedSequence2>::result_type result_type;
    typedef typename indexed_access_traits< result_type >::value_type value_type;
    typedef typename indexed_access_traits< result_type >::dimension_type dimension_type;
    boost::array< value_type, dimension_type::value > temp;
    for( std::size_t i = 0;i < sequence_traits< IndexedSequence1 >::dimension_type::value; ++i )
    {
        temp[i] = indexed_access_traits<result_type>::get( v1, i ) - indexed_access_traits<IndexedSequence2>::get( v2, i );
    }

    return construction_traits<result_type>::construct( temp );
}

//! \brief Run-time vector scalar multiplication.
template <typename IndexedSequence, typename NumericType>
inline typename boost::enable_if_c< is_vector< IndexedSequence >::value              &&
                                    is_indexed_sequence< IndexedSequence >::value    &&
                                    is_numeric< NumericType >::value                 &&
                                    should_use_run_time_access1<IndexedSequence>::type::value,
IndexedSequence >::type operator*( const IndexedSequence& v, const NumericType& s )
{
    typedef typename indexed_access_traits< IndexedSequence >::value_type value_type;
    typedef typename indexed_access_traits< IndexedSequence >::dimension_type dimension_type;
    boost::array< value_type, dimension_type::value > temp;
    
    for( std::size_t i = 0;i < sequence_traits< IndexedSequence >::dimension_type::value; ++i )
    {
        temp[i] = indexed_access_traits< IndexedSequence >::get( v, i ) * s;
    }

    return construction_traits< IndexedSequence >::construct( temp );
}

template <typename IndexedSequence, typename NumericType>
inline typename boost::enable_if_c< is_vector< IndexedSequence >::value              &&
                                    is_indexed_sequence< IndexedSequence >::value    &&
                                    is_numeric< NumericType >::value                 &&
                                    should_use_run_time_access1<IndexedSequence>::type::value,
IndexedSequence >::type operator*( const NumericType& s, const IndexedSequence& v )
{
    typedef typename indexed_access_traits< IndexedSequence >::value_type value_type;
    typedef typename indexed_access_traits< IndexedSequence >::dimension_type dimension_type;
    boost::array< value_type, dimension_type::value > temp;

    for( std::size_t i = 0;i < sequence_traits< IndexedSequence >::dimension_type::value; ++i )
    {
        temp[i] = indexed_access_traits< IndexedSequence >::get( v, i ) * s;
    }

    return construction_traits< IndexedSequence >::construct( temp );
}

//! \brief Run-time vector scalar division.
template <typename IndexedSequence, typename NumericType>
inline typename boost::enable_if_c< is_vector< IndexedSequence >::value              &&
                                    is_indexed_sequence< IndexedSequence >::value    &&
                                    is_numeric< NumericType >::value                 &&
                                    should_use_run_time_access1<IndexedSequence>::type::value,
IndexedSequence >::type operator/( const IndexedSequence& v, const NumericType& s )
{
    typedef typename indexed_access_traits< IndexedSequence >::value_type value_type;
    typedef typename indexed_access_traits< IndexedSequence >::dimension_type dimension_type;
    boost::array< value_type, dimension_type::value > temp;

    for( std::size_t i = 0;i < sequence_traits< IndexedSequence >::dimension_type::value; ++i )
    {
        temp[i] = indexed_access_traits< IndexedSequence >::get( v, i ) / s;
    }

    return construction_traits< IndexedSequence >::construct( temp );
}

//! \brief Return the magnitude of a vector squared.
template <typename IndexedSequence>
inline typename boost::lazy_enable_if_c< is_vector< IndexedSequence >::value &&
                                         is_indexed_sequence< IndexedSequence >::value,
                                         boost::mpl::identity< typename sequence_traits<IndexedSequence>::value_type >
>::type magnitude_squared( const IndexedSequence& v )
{
    return v * v;
}

//! \brief Return the magnitude of a vector.
template <typename IndexedSequence>
inline typename boost::lazy_enable_if_c< is_vector< IndexedSequence >::value &&
                                         is_indexed_sequence< IndexedSequence >::value,
                                         boost::mpl::identity< typename sequence_traits<IndexedSequence>::value_type >
>::type magnitude( const IndexedSequence& v )
{
    typedef typename sequence_traits<IndexedSequence>::value_type result_type;
    return math_functions<result_type>::sqrt( magnitude_squared( v ) );
}

//! \brief Normalize a vector (returns a new unit vector with the same orientation as the original).
template <typename IndexedSequence>
inline typename boost::lazy_enable_if_c< is_vector< IndexedSequence >::value &&
                                         is_indexed_sequence< IndexedSequence >::value,
                                         boost::mpl::identity< IndexedSequence >
>::type normalize( const IndexedSequence& v )
{
    return ( v / magnitude( v ) );
}

}}}//namespace generative::numeric::geometry

#endif //_GENERATIVE_GEOMETRY_INDEXED_SEQUENCE_OPERATORS_HPP
