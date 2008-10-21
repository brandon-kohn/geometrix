//
//!  Copyright (c) 2008
//!  Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_GEOMETRY_POINT_SEQUENCE_HPP
#define _BOOST_GEOMETRY_POINT_SEQUENCE_HPP
#pragma once

#include "geometric_concepts.hpp"
#include "point_sequence_traits.hpp"
#include "construction_traits.hpp"
#include "point.hpp"

namespace boost
{
namespace numeric
{
namespace geometry
{  
    //! Define vector sequences for a few common types.
    BOOST_DEFINE_VECTOR_POINT_SEQUENCE_TRAITS( point_float_2d );
    BOOST_DEFINE_VECTOR_POINT_SEQUENCE_TRAITS( point_float_3d );
    BOOST_DEFINE_VECTOR_POINT_SEQUENCE_TRAITS( point_double_2d );
    BOOST_DEFINE_VECTOR_POINT_SEQUENCE_TRAITS( point_double_3d );
    BOOST_DEFINE_VECTOR_POINT_SEQUENCE_TRAITS( point_int_2d );
    BOOST_DEFINE_VECTOR_POINT_SEQUENCE_TRAITS( point_int_3d );
    BOOST_DEFINE_VECTOR_POINT_SEQUENCE_TRAITS( point_int64_2d );
    BOOST_DEFINE_VECTOR_POINT_SEQUENCE_TRAITS( point_int64_3d );

    template <typename Point>
    struct construction_traits< std::vector< Point > >
    {   
        template <typename Range>
        static inline boost::shared_ptr< std::vector< Point > > construct( const Range& pRange ) 
        {
            boost::shared_ptr< std::vector< Point > > pP( new std::vector< Point >( boost::begin( pRange ), boost::end( pRange ) ) ); 
            return pP; 
        }
    };

}}}//namespace boost::numeric::geometry;

#endif //_BOOST_GEOMETRY_POINT_SEQUENCE_HPP
