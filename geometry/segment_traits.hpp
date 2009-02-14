//
//! Copyright © 2008-2009
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _GENERATIVE_GEOMETRY_SEGMENT_TRAITS_HPP
#define _GENERATIVE_GEOMETRY_SEGMENT_TRAITS_HPP
#pragma once

#include "point_traits.hpp"

namespace generative
{
namespace numeric
{
namespace geometry
{

//! \brief Tag to check if a type is a segment type.
template <typename Segment>
struct is_segment : boost::false_type{};

//! \brief segment traits struct. 
template <typename Segment>
struct segment_traits
{
	BOOST_MPL_ASSERT_MSG( 
		  ( false )
		, SEGMENT_TRAITS_NOT_DEFINED
		, (Segment) );	
};

//! \brief Concept of a segment which is constrained to define point_type via the segment_traits class (specialization).
template <typename Segment>
struct SegmentConcept
{
    void constraints()
    {
        //! traits must define point type.
        typedef typename segment_traits< Segment >::point_type      point_type;
        typedef typename segment_traits< Segment >::coordinate_type coordinate_type;
        typedef typename segment_traits< Segment >::dimension_type  dimension_type;
        
        //! Check that is is indeed a point.
        boost::function_requires< PointConcept< point_type > >();

        //!TODO: What else?
    }
};

//! \brief Macro for defining a segment type traits.
#define BOOST_DEFINE_SEGMENT_TRAITS( Point, Segment )                   \
template <> struct is_segment< Segment > : boost::true_type{};          \
template <>                                                             \
struct segment_traits< Segment >                                        \
{                                                                       \
 	typedef Point                                       point_type;     \
    typedef Segment                                     segment_type;   \
    typedef point_traits< point_type >::dimension_type  dimension_type; \
    typedef point_traits< point_type >::coordinate_type coordinate_type;\
};

//! \brief segment access traits struct
//! NOTE: must be specialized for user types.
template <typename Segment>
struct segment_access_traits
{
	BOOST_MPL_ASSERT_MSG( 
		  ( false )
		, SEGMENT_ACCESS_TRAITS_NOT_DEFINED
		, (Segment) );	
 
    typedef Segment                                                  segment_type;                                           
    typedef typename segment_traits< segment_type >::point_type      point_type;   
    typedef typename segment_traits< segment_type >::coordinate_type coordinate_type;
                                                                                                        
    static inline const point_type& get_start( const segment_type& s ) { return s.get_start(); }        
	static inline const point_type& get_end( const segment_type& s )   { return s.get_end(); }          
    static inline void              set_start( segment_type& s, const point_type& start ) { s.set_start( start ); } 
	static inline void              set_end( segment_type& s, const point_type& end ) { s.set_end( end ); }

};

//! \brief Concept of segment access interface which requires typedef of segment_type point_type and accessors to both points via a specialization of segment_access_traits.
template <typename AccessInterface>
struct SegmentAccessorConcept
{
    typedef typename AccessInterface::segment_type               segment_type;  
    typedef typename segment_traits< segment_type >::point_type  point_type;

    void constraints()
    {            
        boost::function_requires< SegmentConcept< segment_type > >();
        segment_type* s = 0;
        point_type start = AccessInterface::get_start( *s );
        point_type end = AccessInterface::get_end( *s );

        AccessInterface::set_start( *s, start );
        AccessInterface::set_end( *s, end );       

        segment_type p = construction_traits< segment_type >::construct( start, end );
    }

};

}}}//namespace generative::numeric::geometry;

#endif //_GENERATIVE_GEOMETRY_SEGMENT_TRAITS_HPP
