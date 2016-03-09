//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_BOUNDING_BOX_INTERSECTION_HPP
#define GEOMETRIX_BOUNDING_BOX_INTERSECTION_HPP
#pragma once

#include <geometrix/utility/array.hpp>
#include <geometrix/primitive/point_sequence_traits.hpp>
#include <geometrix/primitive/segment_traits.hpp>
#include <geometrix/primitive/axis_aligned_bounding_box.hpp>
#include <boost/algorithm/minmax.hpp>

namespace geometrix {

    //! Bounding box intersection test between two boxes (defined by segment diagonals p1->p2 and p3->p4.)
    template <typename NumberComparisonPolicy, typename Point>
    inline bool bounding_box_intersection( const Point& p1, const Point& p2, const Point& p3, const Point& p4, const NumberComparisonPolicy& compare ) 
    {
        typedef typename geometric_traits<Point>::coordinate_type coordinate_type;
        BOOST_CONCEPT_ASSERT((Point2DConcept<Point>));

        coordinate_type xmax1, xmax2, ymax1, ymax2, xmin1, xmin2, ymin1, ymin2;
        boost::tie( xmin1, xmax1 ) = boost::minmax( get<0>( p1 ), get<0>( p2 ) );
		boost::tie( ymin1, ymax1 ) = boost::minmax( get<1>( p1 ), get<1>( p2 ) );
		boost::tie( xmin2, xmax2 ) = boost::minmax( get<0>( p3 ), get<0>( p4 ) );
		boost::tie( ymin2, ymax2 ) = boost::minmax( get<1>( p3 ), get<1>( p4 ) );

        return ( compare.greater_than_or_equal( xmax1, xmin2 ) &&
                 compare.greater_than_or_equal( xmax2, xmin1 ) &&
                 compare.greater_than_or_equal( ymax1, ymin2 ) &&
                 compare.greater_than_or_equal( ymax2, ymin1 ) );
    }

    //! Bounding box test between two segments.
    template <typename NumberComparisonPolicy, typename Segment>
    inline bool bounding_box_intersection( const Segment& s1, const Segment& s2, const NumberComparisonPolicy& compare ) 
    {        
        return bounding_box_intersection( get_start( s1 ), get_end( s1 ), get_start( s2 ), get_end( s2 ), compare );
    }
	
}//namespace geometrix;

#endif //GEOMETRIX_BOUNDING_BOX_INTERSECTION_HPP
