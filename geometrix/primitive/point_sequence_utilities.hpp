//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_POINT_SEQUENCE_UTILITIES_HPP
#define GEOMETRIX_POINT_SEQUENCE_UTILITIES_HPP
#pragma once

#include <geometrix/primitive/point_sequence_traits.hpp>
#include <geometrix/algebra/exterior_product.hpp>

#include <boost/foreach.hpp>
#include <boost/tuple/tuple.hpp>

namespace geometrix {
    //! Function to calculate the centroid of a point sequence.
    template <typename PointSequence>
    inline typename point_sequence_traits<PointSequence>::point_type 
    get_centroid( const PointSequence& polygon, 
                  typename boost::enable_if
                  <
                    boost::is_same
                    <
                        typename geometric_traits
                        <
                            typename point_sequence_traits<PointSequence>::point_type
                        >::dimension_type,
                        dimension<2>
                    >
                  >::type* = 0 )
    {
        BOOST_CONCEPT_ASSERT((PointSequenceConcept< PointSequence >));

        typedef typename point_sequence_traits<PointSequence>::point_type point_type;		
        typedef typename geometric_traits<point_type>::arithmetic_type arithmetic_type;        

        arithmetic_type mX = 0.;
        arithmetic_type mY = 0.;
        arithmetic_type area = 0.;
        for( typename PointSequence::const_iterator it( point_sequence_traits< PointSequence >::begin( polygon ) ), 
             nextIt( point_sequence_traits< PointSequence >::begin( polygon ) + 1),
             end( point_sequence_traits< PointSequence >::end( polygon ) );
             nextIt != end; 
             ++it, ++nextIt )
        {
            const point_type& currentPoint = *it;
            const point_type& nextPoint = *nextIt;
            typename geometric_traits<point_type>::arithmetic_type ai = exterior_product_area( as_vector( currentPoint ), as_vector( nextPoint ) );
            area += ai;
            mX += ai * ( get<0>( currentPoint ) + get<0>( nextPoint ) );
            mY += ai * ( get<1>( currentPoint ) + get<1>( nextPoint ) );	
        }

        {
            const point_type& backPoint = point_sequence_traits< PointSequence >::back( polygon );
            const point_type& frontPoint = point_sequence_traits< PointSequence >::front( polygon );

            typename geometric_traits<point_type>::arithmetic_type ai = exterior_product_area( as_vector( backPoint ), as_vector( frontPoint ) );
            area += ai;
            mX += ai * ( get<0>( backPoint ) + get<0>( frontPoint ) );
            mY += ai * ( get<1>( backPoint ) + get<1>( frontPoint ) );	
        }
        
        area *= 0.5;
        arithmetic_type q = 1. /( 6.0 * area);		
        return construction_policy<point_type>::construct( mX * q, mY * q );
    }

    //! Function to calculate the centroid of a point sequence.
    template <typename PointSequence>
    inline double get_area( const PointSequence& polygon,
                              typename boost::enable_if
                              <
                                boost::is_same
                                <
                                    typename geometric_traits
                                    <
                                        typename point_sequence_traits<PointSequence>::point_type
                                    >::dimension_type,
                                    dimension<2>
                                >
                              >::type* = 0 )
    {
        BOOST_CONCEPT_ASSERT((PointSequenceConcept< PointSequence >));
        typedef typename point_sequence_traits<PointSequence>::point_type point_type;
        typename geometric_traits<point_type>::arithmetic_type area = 0;
        for( typename PointSequence::const_iterator it( point_sequence_traits< PointSequence >::begin( polygon ) ),
             nextIt( point_sequence_traits< PointSequence >::begin( polygon ) + 1), end( point_sequence_traits< PointSequence >::end( polygon ) );
             nextIt != end; ++it, ++nextIt )
        {
            area += exterior_product_area( as_vector(*it), as_vector(*nextIt) );			
        }

        area += exterior_product_area( as_vector( point_sequence_traits< PointSequence >::back( polygon ) ), as_vector( point_sequence_traits< PointSequence >::front( polygon ) ) );
        area *= 0.5;	
        return area;
    }
    
    //! Function to calculate the min/max bounds of a point sequence.
    enum cartesian_bound
    {
        e_xmin = 0,
        e_ymin = 1,
        e_xmax = 2,
        e_ymax = 3
    };
    template <typename PointSequence>
    struct bounds_tuple
    {
        typedef boost::tuple< typename geometric_traits< typename point_sequence_traits< PointSequence >::point_type >::arithmetic_type,
                              typename geometric_traits< typename point_sequence_traits< PointSequence >::point_type >::arithmetic_type,
                              typename geometric_traits< typename point_sequence_traits< PointSequence >::point_type >::arithmetic_type,
                              typename geometric_traits< typename point_sequence_traits< PointSequence >::point_type >::arithmetic_type > type;
    };
    template <typename PointSequence, typename NumberComparisonPolicy>
    inline typename bounds_tuple< PointSequence >::type 
    get_bounds( const PointSequence& pointSequence, 
                const NumberComparisonPolicy& compare,
                typename boost::enable_if
                <
                    boost::is_same
                    <
                        typename geometric_traits
                        <
                            typename point_sequence_traits<PointSequence>::point_type 
                        >::dimension_type,
                        dimension<2>
                    >
                >::type* = 0 )
    {
        typedef typename point_sequence_traits< PointSequence >::point_type point_type;
        typedef typename geometric_traits< point_type >::arithmetic_type    arithmetic_type;

        typedef boost::tuple< arithmetic_type, 
                              arithmetic_type,
                              arithmetic_type,
                              arithmetic_type > bounds_tuple;

        bounds_tuple bounds;
        if( std::numeric_limits< arithmetic_type >::has_infinity )
        {
            bounds = boost::make_tuple( std::numeric_limits< arithmetic_type >::infinity(),
                                        std::numeric_limits< arithmetic_type >::infinity(),
                                        -std::numeric_limits< arithmetic_type >::infinity(),
                                        -std::numeric_limits< arithmetic_type >::infinity() );
        }
        else
        {
            bounds = boost::make_tuple( (std::numeric_limits< arithmetic_type >::max)(),
                                        (std::numeric_limits< arithmetic_type >::max)(),
                                        -(std::numeric_limits< arithmetic_type >::max)(),
                                        -(std::numeric_limits< arithmetic_type >::max)() );
        }

        typename point_sequence_traits< PointSequence >::const_iterator pIt = point_sequence_traits< PointSequence >::begin( pointSequence ); 
        typename point_sequence_traits< PointSequence >::const_iterator pEnd = point_sequence_traits< PointSequence >::end( pointSequence );        
        while( pIt != pEnd )
        {
            const point_type& p = *pIt++;
            const arithmetic_type& x = get<0>( p );            
            if( compare.less_than( x, bounds.template get<e_xmin>() ) )
                bounds.template get<e_xmin>() = x;
            if( compare.greater_than( x, bounds.template get<e_xmax>() ) )
                bounds.template get<e_xmax>() = x;

            const arithmetic_type& y = get<1>( p );
            if( compare.less_than( y, bounds.template get<e_ymin>() ) )
                bounds.template get<e_ymin>() = y;
            if( compare.greater_than( y, bounds.template get<e_ymax>() ) )
                bounds.template get<e_ymax>() = y;
        }

        return bounds;
    }

	template <typename Polygon>
	inline std::size_t next_index( const Polygon& polygon, std::size_t index )
	{
		return (index + 1) < point_sequence_traits<Polygon>::size(polygon) ? index + 1 : 0;
	}

	template <typename Polygon>
	inline std::size_t prev_index( const Polygon& polygon, std::size_t index )
	{
		return (index > 0) ? index - 1 : (point_sequence_traits<Polygon>::size(polygon) - 1);
	}

	template <typename Polygon>
	inline bool is_point_concave(const Polygon& polygon, std::size_t index)
	{
		const auto& point = point_sequence_traits<Polygon>::get_point( polygon, prev_index( polygon, index ) );
		const auto& prevPoint = point_sequence_traits<Polygon>::get_point( polygon, prev_index( polygon, index ) );
		const auto& nextPoint = point_sequence_traits<Polygon>::get_point( polygon, next_index( polygon, index ) );
		return !(exterior_product_area( point - prevPoint, nextPoint - prevPoint ) > 0);
	}
    
}//namespace geometrix;

#endif //GEOMETRIX_POINT_SEQUENCE_UTILITIES_HPP
