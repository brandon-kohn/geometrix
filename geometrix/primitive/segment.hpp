//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_SEGMENT_HPP
#define GEOMETRIX_SEGMENT_HPP

#include <geometrix/primitive/segment_traits.hpp>

namespace geometrix {

/////////////////////////////////////////////////////////////////////////////
//
// CLASS segment
// A template class for specifying a line segment with a start and end point
// which models a point concept.
template <typename Point>
class segment
{
    BOOST_CLASS_REQUIRE( Point, geometrix, PointConcept );

public:

    typedef Point                                                 point_type;
    typedef typename geometric_traits<point_type>::dimension_type dimension_type;

    segment()
    {}

    segment( const point_type& start, const point_type& end )
        : m_start( start )
        , m_end( end )
    {}
    
    ~segment(){}

    const point_type& get_start() const { return m_start; }
    const point_type& get_end()   const { return m_end; }
    void  set_start( const point_type& start ) { m_start = start; }
    void  set_end( const point_type& end ) { m_end = end; }

private:

    point_type m_start;
    point_type m_end;

};

template <typename Point>
struct geometric_traits< segment<Point> >
{
    typedef Point                                                   point_type;
    typedef segment<point_type>                                     segment_type;
    typedef typename geometric_traits< point_type >::dimension_type dimension_type;
    typedef void                                                    is_segment;
};

//! Default access specialization for any point type.
template <typename Point>             
struct segment_access_traits< segment<Point> >
{
    typedef segment<Point> segment_type;
    typedef Point          point_type;

    static const point_type& get_start( const segment_type& s ) { return s.get_start(); }
    static const point_type& get_end( const segment_type& s )   { return s.get_end(); }
    static void              set_start( segment_type& s, const point_type& start ) { s.set_start( start ); }
    static void              set_end( segment_type& s, const point_type& end ) { s.set_end( end ); }
};

template <typename Point>
struct construction_policy< segment< Point > >
{    
    static segment< Point > construct( const Point& start, const Point& end ) 
    {
        return segment< Point >( start, end );
    }
};

}//namespace geometrix;

//! Specialize the coordinate accessors
#define GEOMETRIX_DEFINE_SEGMENT_ACCESS_TRAITS( Segment )                                                   \
namespace geometrix {                                                                                       \
template <>                                                                                                 \
struct segment_access_traits< Segment >                                                                     \
{                                                                                                           \
    typedef Segment                                 segment_type;                                           \
    typedef geometric_traits< Segment >::point_type point_type;                                             \
                                                                                                            \
    static const point_type& get_start( const segment_type& s ) { return s.get_start(); }                   \
    static const point_type& get_end( const segment_type& s )   { return s.get_end(); }                     \
    static void              set_start( segment_type& s, const point_type& start ) { s.set_start( start ); }\
    static void              set_end( segment_type& s, const point_type& end ) { s.set_end( end ); }        \
};                                                                                                          \
}                                                                                                           \
/***/

//! Define some default traits.
GEOMETRIX_DEFINE_SEGMENT_TRAITS( point_double_2d, segment< point_double_2d > );
GEOMETRIX_DEFINE_SEGMENT_TRAITS( point_double_3d, segment< point_double_3d > );
GEOMETRIX_DEFINE_SEGMENT_TRAITS( point_float_2d, segment< point_float_2d > );
GEOMETRIX_DEFINE_SEGMENT_TRAITS( point_float_3d, segment< point_float_3d > );

GEOMETRIX_DEFINE_SEGMENT_ACCESS_TRAITS( segment< point_double_2d > );
GEOMETRIX_DEFINE_SEGMENT_ACCESS_TRAITS( segment< point_double_3d > );
GEOMETRIX_DEFINE_SEGMENT_ACCESS_TRAITS( segment< point_float_2d > );
GEOMETRIX_DEFINE_SEGMENT_ACCESS_TRAITS( segment< point_float_3d > );

GEOMETRIX_DEFINE_SEGMENT_TRAITS( point_int_2d, segment< point_int_2d > );
GEOMETRIX_DEFINE_SEGMENT_TRAITS( point_int_3d, segment< point_int_3d > );
GEOMETRIX_DEFINE_SEGMENT_TRAITS( point_int64_2d, segment< point_int64_2d > );
GEOMETRIX_DEFINE_SEGMENT_TRAITS( point_int64_3d, segment< point_int64_3d > );

GEOMETRIX_DEFINE_SEGMENT_ACCESS_TRAITS( segment< point_int_2d > );
GEOMETRIX_DEFINE_SEGMENT_ACCESS_TRAITS( segment< point_int_3d > );
GEOMETRIX_DEFINE_SEGMENT_ACCESS_TRAITS( segment< point_int64_2d > );
GEOMETRIX_DEFINE_SEGMENT_ACCESS_TRAITS( segment< point_int64_3d > );

#endif //GEOMETRIX_SEGMENT_HPP
