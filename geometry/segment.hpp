//
//! Copyright © 2008-2009
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GENERATIVE_GEOMETRY_SEGMENT_HPP
#define GENERATIVE_GEOMETRY_SEGMENT_HPP
#pragma once

#include "segment_traits.hpp"

namespace generative
{
namespace numeric
{
namespace geometry
{

/////////////////////////////////////////////////////////////////////////////
//
// CLASS segment
// A template class for specifying a line segment with a start and end point
// which models a point concept.
template <typename Point>
class segment
{
    BOOST_CLASS_REQUIRE( Point, generative::numeric::geometry, PointConcept );

public:

	typedef Point                                              point_type;
	typedef typename point_traits<point_type>::coordinate_type coordinate_type;
    typedef typename point_traits<point_type>::dimension_type  dimension_type;

    segment()
    {}

	segment( const point_type& start, const point_type& end )
		: m_start( start )
		, m_end( end )
	{}
	
	~segment(){}

	inline const point_type& get_start() const { return m_start; }
	inline const point_type& get_end()   const { return m_end; }
	inline void  set_start( const point_type& start ) { m_start = start; }
	inline void  set_end( const point_type& end ) { m_end = end; }

private:

	point_type m_start;
	point_type m_end;

};

//! Specialize the coordinate accessors
#define BOOST_DEFINE_SEGMENT_ACCESS_TRAITS( Segment )                                                              \
template <>                                                                                                        \
struct segment_access_traits< Segment >                                                                            \
{                                                                                                                  \
    typedef Segment                               segment_type;                                                    \
    typedef segment_traits< Segment >::point_type point_type;                                                      \
                                                                                                                   \
    static inline const point_type& get_start( const segment_type& s ) { return s.get_start(); }                   \
	static inline const point_type& get_end( const segment_type& s )   { return s.get_end(); }                     \
    static inline void              set_start( segment_type& s, const point_type& start ) { s.set_start( start ); }\
	static inline void              set_end( segment_type& s, const point_type& end ) { s.set_end( end ); }        \
};

template <typename Point>
struct construction_traits< segment< Point > >
{    
    static inline segment< Point > construct( const Point& start, const Point& end ) 
    {
        return segment< Point >( start, end );
    }
};

//! Define some default traits.
BOOST_DEFINE_SEGMENT_TRAITS( point_double_2d, segment< point_double_2d > );
BOOST_DEFINE_SEGMENT_TRAITS( point_double_3d, segment< point_double_3d > );
BOOST_DEFINE_SEGMENT_TRAITS( point_float_2d, segment< point_float_2d > );
BOOST_DEFINE_SEGMENT_TRAITS( point_float_3d, segment< point_float_3d > );

BOOST_DEFINE_SEGMENT_ACCESS_TRAITS( segment< point_double_2d > );
BOOST_DEFINE_SEGMENT_ACCESS_TRAITS( segment< point_double_3d > );
BOOST_DEFINE_SEGMENT_ACCESS_TRAITS( segment< point_float_2d > );
BOOST_DEFINE_SEGMENT_ACCESS_TRAITS( segment< point_float_3d > );

BOOST_DEFINE_SEGMENT_TRAITS( point_int_2d, segment< point_int_2d > );
BOOST_DEFINE_SEGMENT_TRAITS( point_int_3d, segment< point_int_3d > );
BOOST_DEFINE_SEGMENT_TRAITS( point_int64_2d, segment< point_int64_2d > );
BOOST_DEFINE_SEGMENT_TRAITS( point_int64_3d, segment< point_int64_3d > );

BOOST_DEFINE_SEGMENT_ACCESS_TRAITS( segment< point_int_2d > );
BOOST_DEFINE_SEGMENT_ACCESS_TRAITS( segment< point_int_3d > );
BOOST_DEFINE_SEGMENT_ACCESS_TRAITS( segment< point_int64_2d > );
BOOST_DEFINE_SEGMENT_ACCESS_TRAITS( segment< point_int64_3d > );

}}}//namespace generative::numeric::geometry;

#endif //GENERATIVE_GEOMETRY_SEGMENT_HPP
