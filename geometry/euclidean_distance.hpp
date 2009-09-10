//
//! Copyright © 2008-2009
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GENERATIVE_GEOMETRY_EUCLIDEAN_DISTANCE_HPP
#define GENERATIVE_GEOMETRY_EUCLIDEAN_DISTANCE_HPP
#pragma once

#include "vector_traits.hpp"

namespace generative
{
namespace numeric
{
namespace geometry
{

//! \brief Calculate the distance between two points in any dimensional space for a cartesian reference frame.
template <typename CoordinateSequence>
typename coordinate_sequence_traits<CoordinateSequence>::coordinate_type
euclidean_distance_squared( const CoordinateSequence& A, 
                            const CoordinateSequence& B,
                            typename boost::enable_if<
                                boost::is_same< 
                                    typename coordinate_sequence_traits<CoordinateSequence>::dimension_type,
                                    dimension_traits<2> >
                            >::type* = 0 )
{
    typedef cartesian_access_traits< CoordinateSequence > access_traits;

    typename coordinate_sequence_traits<CoordinateSequence>::coordinate_type dx =
        access_traits::get<0>( B ) - access_traits::get<0>( A );

    typename coordinate_sequence_traits<CoordinateSequence>::coordinate_type dy =
        access_traits::get<1>( B ) - access_traits::get<1>( A );

    return ( dx * dx + dy * dy );
}

//! \brief Compute the distance between two points A-B in the 2D Cartesian plane.
template <typename CoordinateSequence>
typename coordinate_sequence_traits<CoordinateSequence>::coordinate_type 
euclidean_distance( const CoordinateSequence& A,
                    const CoordinateSequence& B,
                    typename boost::enable_if<
                        boost::is_same< 
                            typename coordinate_sequence_traits<CoordinateSequence>::dimension_type,
                            dimension_traits<2> 
                        >
                    >::type* = 0 )
{
    return math_functions< coordinate_sequence_traits<CoordinateSequence>::coordinate_type >::sqrt( euclidean_distance_squared( A, B ) );
}

//! \brief Calculate the distance between two points in any dimensional space for a cartesian reference frame.
template <typename CoordinateSequence>
typename coordinate_sequence_traits<CoordinateSequence>::coordinate_type 
euclidean_distance_squared( const CoordinateSequence& A, 
                            const CoordinateSequence& B,
                            typename boost::disable_if<
                                boost::is_same< 
                                    typename coordinate_sequence_traits<CoordinateSequence>::dimension_type,
                                    dimension_traits<2> 
                                >
                            >::type* = 0 )
{
    return magnitude_squared( vector< coordinate_sequence_traits<CoordinateSequence>::coordinate_type,
        coordinate_sequence_traits<CoordinateSequence>::dimension_type::value >(A-B) );
}

//! \brief Compute the distance between two points A-B in the ND Cartesian plane.
template <typename CoordinateSequence>
typename coordinate_sequence_traits<CoordinateSequence>::coordinate_type 
euclidean_distance( const CoordinateSequence& A,
                    const CoordinateSequence& B,
                    typename boost::disable_if<
                        boost::is_same< 
                            typename coordinate_sequence_traits<CoordinateSequence>::dimension_type,
                            dimension_traits<2> 
                        >
                    >::type* = 0 )
{
    return magnitude
    ( 
        vector< coordinate_sequence_traits<CoordinateSequence>::coordinate_type,
                coordinate_sequence_traits<CoordinateSequence>::dimension_type::value >(B) - 
        vector< coordinate_sequence_traits<CoordinateSequence>::coordinate_type,
                coordinate_sequence_traits<CoordinateSequence>::dimension_type::value >(A)
    );
}

//! \struct distance_compare
//! \brief A predicate to compare two numeric sequences by their distance from a fixed origin.    
template <typename CoordinateSequence, typename NumberComparisonPolicy>
struct distance_compare
{
    distance_compare( const CoordinateSequence& p, const NumberComparisonPolicy& compare )
        : m_compare( compare )
        , m_p( p )
    {}
    
    template <typename NumericSequence>
    typename bool operator()( const NumericSequence& lhs, const NumericSequence& rhs ) const
    {
        //! Sequences are compared by dimension specified. In the case where coordinates at D are equal, the sequences are
        //! given a total order lexicographically.
        typedef typename boost::remove_const< NumericSequence >::type sequence_type;
        typedef typename numeric_sequence_traits< NumericSequence >::numeric_type numeric_type;
        numeric_type distanceLHS = euclidean_distance_squared( m_p, lhs );
        numeric_type distanceRHS = euclidean_distance_squared( m_p, rhs );
        return m_compare.less_than( distanceLHS, distanceRHS );
    }

    NumberComparisonPolicy m_compare;
    CoordinateSequence     m_p;
}; 

}}}//namespace generative::numeric::geometry

#endif //GENERATIVE_GEOMETRY_EUCLIDEAN_DISTANCE_HPP
