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
        static std::vector< Point > construct( const Range& pRange ) 
        {
            return std::vector< Point >( boost::begin( pRange ), boost::end( pRange ) );
        }
    };

}//namespace geometrix;

//! Define vector sequences for a few common types.
GEOMETRIX_DEFINE_VECTOR_POINT_SEQUENCE_TRAITS( point_float_2d );
GEOMETRIX_DEFINE_VECTOR_POINT_SEQUENCE_TRAITS( point_float_3d );
GEOMETRIX_DEFINE_VECTOR_POINT_SEQUENCE_TRAITS( point_double_2d );
GEOMETRIX_DEFINE_VECTOR_POINT_SEQUENCE_TRAITS( point_double_3d );
GEOMETRIX_DEFINE_VECTOR_POINT_SEQUENCE_TRAITS( point_int_2d );
GEOMETRIX_DEFINE_VECTOR_POINT_SEQUENCE_TRAITS( point_int_3d );
GEOMETRIX_DEFINE_VECTOR_POINT_SEQUENCE_TRAITS( point_int64_2d );
GEOMETRIX_DEFINE_VECTOR_POINT_SEQUENCE_TRAITS( point_int64_3d );

//! Define array sequences for a few common types.
GEOMETRIX_DEFINE_STD_ARRAY_POINT_SEQUENCE_TRAITS(point_float_2d);
GEOMETRIX_DEFINE_STD_ARRAY_POINT_SEQUENCE_TRAITS(point_float_3d);
GEOMETRIX_DEFINE_STD_ARRAY_POINT_SEQUENCE_TRAITS(point_double_2d);
GEOMETRIX_DEFINE_STD_ARRAY_POINT_SEQUENCE_TRAITS(point_double_3d);
GEOMETRIX_DEFINE_STD_ARRAY_POINT_SEQUENCE_TRAITS(point_int_2d);
GEOMETRIX_DEFINE_STD_ARRAY_POINT_SEQUENCE_TRAITS(point_int_3d);
GEOMETRIX_DEFINE_STD_ARRAY_POINT_SEQUENCE_TRAITS(point_int64_2d);
GEOMETRIX_DEFINE_STD_ARRAY_POINT_SEQUENCE_TRAITS(point_int64_3d);


#endif //GEOMETRIX_POINT_SEQUENCE_HPP
