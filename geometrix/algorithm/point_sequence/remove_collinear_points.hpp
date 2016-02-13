//
//! Copyright © 2008-2016
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_REMOVE_COLLINEAR_POINTS_HPP
#define GEOMETRIX_REMOVE_COLLINEAR_POINTS_HPP
#pragma once

#include <geometrix/primitive/point_sequence_traits.hpp>
#include <geometrix/utility/utilities.hpp>

namespace geometrix {

    template <typename Polygon, typename NumberComparisonPolicy>
    inline Polygon remove_collinear_points( const Polygon& poly, const NumberComparisonPolicy& cmp )
    {
        typedef point_sequence_traits<Polygon> access;
        typedef typename access::point_type point_type;
        
        //! TODO: need to generically construct polygons.
        Polygon newPoly{poly.front()};
        
        std::size_t size = access::size( poly );
        auto next = [size]( std::size_t i ){ return (i + 1) % size; };
        for( std::size_t i = 1; i < size; ++i ){
            const auto& a = newPoly.back();
            const auto& b = poly[i];
            const auto& c = poly[next( i )];
            if( !is_collinear( a, b, c, cmp ) )
                newPoly.push_back( b );
        }

        return std::move( newPoly );
    }

}//namespace geometrix;

#endif //! GEOMETRIX_REMOVE_COLLINEAR_POINTS_HPP
