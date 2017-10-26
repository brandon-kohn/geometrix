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
#include <geometrix/primitive/point_sequence_traits.hpp>
#include <geometrix/primitive/point.hpp>

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

    segment( const typename type_at<point_type, 0>::type& sx, const typename type_at<point_type, 0>::type& sy, const typename type_at<point_type, 0>::type& dx, const typename type_at<point_type, 0>::type& dy )
        : m_start(construct<point_type>( sx, sy ))
        , m_end(construct<point_type>( dx, dy ))
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
    segment reverse() const { return segment( m_end, m_start ); }

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
    
    template <typename PointSequence, typename std::enable_if<is_point_sequence<PointSequence>::value, int>::type = 0>
    static segment< Point > construct( const PointSequence& ps )
    {
        BOOST_CONCEPT_ASSERT((PointSequenceConcept<PointSequence>));
        using access = point_sequence_traits<typename std::decay<PointSequence>::type>;
        GEOMETRIX_ASSERT(access::size(ps) == 2);
        return segment<Point>(access::get_point(ps, 0), access::get_point(ps, 1));
    }
};

template <typename Point>
inline segment<Point> make_segment(const Point& p1, const Point& p2) 
{
    return segment<Point>(p1, p2);
}

typedef segment<point_double_2d> segment_double_2d;
typedef segment<point_double_3d> segment_double_3d;
typedef segment<point_float_2d> segment_float_2d;
typedef segment<point_float_3d> segment_float_3d;

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

GEOMETRIX_DEFINE_SEGMENT_ACCESS_TRAITS( segment< point_int_2d > );
GEOMETRIX_DEFINE_SEGMENT_ACCESS_TRAITS( segment< point_int_3d > );

#endif //GEOMETRIX_SEGMENT_HPP
