//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_POINT_SEQUENCE_HPP
#define GEOMETRIX_POINT_SEQUENCE_HPP

#include <geometrix/primitive/point_sequence_traits.hpp>
#include <geometrix/utility/construction_policy.hpp>
#include <geometrix/primitive/point.hpp>

#include <boost/range.hpp>

namespace geometrix {  
    template <typename Point>
    struct construction_policy< std::vector< Point > >
    {   
        template <typename Range>
        static boost::shared_ptr< std::vector< Point > > construct( const Range& pRange ) 
        {
            boost::shared_ptr< std::vector< Point > > pP( new std::vector< Point >( boost::begin( pRange ), boost::end( pRange ) ) ); 
            return pP; 
        }
    };

}//namespace geometrix;

//! Define vector sequences for a few common types.
GEOMETRIX_DEFINE_VECTOR_POINT_SEQUENCE_TRAITS( point_float_2D );
GEOMETRIX_DEFINE_VECTOR_POINT_SEQUENCE_TRAITS( point_float_3D );
GEOMETRIX_DEFINE_VECTOR_POINT_SEQUENCE_TRAITS( point_double_2D );
GEOMETRIX_DEFINE_VECTOR_POINT_SEQUENCE_TRAITS( point_double_3D );
GEOMETRIX_DEFINE_VECTOR_POINT_SEQUENCE_TRAITS( point_int_2D );
GEOMETRIX_DEFINE_VECTOR_POINT_SEQUENCE_TRAITS( point_int_3D );
GEOMETRIX_DEFINE_VECTOR_POINT_SEQUENCE_TRAITS( point_int64_2D );
GEOMETRIX_DEFINE_VECTOR_POINT_SEQUENCE_TRAITS( point_int64_3D );

#endif //GEOMETRIX_POINT_SEQUENCE_HPP
