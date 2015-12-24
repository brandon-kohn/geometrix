//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_SEGMENT_TRAITS_HPP
#define GEOMETRIX_SEGMENT_TRAITS_HPP

#include <geometrix/primitive/point_traits.hpp>

namespace geometrix {

//! \brief Tag to check if a type is a segment type.
template <typename Segment, typename EnableIf=void>
struct is_segment : boost::false_type{};

template <typename Segment>
struct is_segment<Segment, typename geometric_traits<Segment>::is_segment> : boost::true_type{};

//! \brief Concept of a segment which is constrained to define point_type via the geometric_traits class (specialization).
template <typename Segment>
struct SegmentConcept
{
    GEOMETRIX_STATIC_ASSERT(( is_segment<Segment>::value ));
    
    //! traits must define point type.
    typedef typename geometric_traits< Segment >::point_type      point_type;
    typedef typename geometric_traits< Segment >::dimension_type  dimension_type;
        
    //! Check that is is indeed a point.
    BOOST_CONCEPT_ASSERT((PointConcept< point_type >));
};

//! \brief Concept of a segment whose points are 2d.
template <typename Segment>
struct Segment2DConcept
{
    BOOST_CONCEPT_ASSERT((SegmentConcept<Segment>));
    BOOST_CONCEPT_ASSERT((DimensionConcept<Segment, 2>));
};

//! \brief Concept of a segment whose points are 3d.
template <typename Segment>
struct Segment3DConcept
{
    BOOST_CONCEPT_ASSERT((SegmentConcept<Segment>));
    BOOST_CONCEPT_ASSERT((DimensionConcept<Segment, 3>));
};

//! \brief Macro for defining a segment type traits.
#define GEOMETRIX_DEFINE_SEGMENT_TRAITS( Point, Segment )                \
namespace geometrix {                                                    \
template <>                                                              \
struct geometric_traits< Segment >                                       \
{                                                                        \
    typedef Point                                        point_type;     \
    typedef Segment                                      segment_type;   \
    typedef geometric_traits<point_type>::dimension_type dimension_type; \
    typedef void                                         is_segment;     \
};                                                                       \
}                                                                        \
/***/

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
    typedef typename geometric_traits< segment_type >::point_type    point_type;   
                                                                                                        
    static const point_type& get_start( const segment_type& s ) { return s.get_start(); }        
    static const point_type& get_end( const segment_type& s )   { return s.get_end(); }          
    static void              set_start( segment_type& s, const point_type& start ) { s.set_start( start ); } 
    static void              set_end( segment_type& s, const point_type& end ) { s.set_end( end ); }

};

//! \brief Concept of segment access interface which requires typedef of segment_type point_type and accessors to both points via a specialization of segment_access_traits.
template <typename AccessInterface>
struct SegmentAccessorConcept
{
    typedef typename AccessInterface::segment_type                segment_type;  
    typedef typename geometric_traits< segment_type >::point_type point_type;
    BOOST_CONCEPT_ASSERT((SegmentConcept< segment_type >));

    BOOST_CONCEPT_USAGE(SegmentAccessorConcept)
    {        
        segment_type* s = 0;
        point_type start = AccessInterface::get_start( *s );
        point_type end = AccessInterface::get_end( *s );

        AccessInterface::set_start( *s, start );
        AccessInterface::set_end( *s, end );       

        segment_type p = construct<segment_type>( start, end );
    }
};

template <typename Segment>
const typename geometric_traits<Segment>::point_type& get_start( const Segment& s ) 
{
    return segment_access_traits<Segment>::get_start( s );
}

template <typename Segment>
const typename geometric_traits<Segment>::point_type& get_end( const Segment& s ) 
{
    return segment_access_traits<Segment>::get_end( s );
}

template <typename Segment, typename Point>
void set_start( Segment& s, const Point& p ) 
{
    return segment_access_traits<Segment>::set_start( s, construct< typename geometric_traits<Segment>::point_type >( p ) );
}

template <typename Segment, typename Point>
void set_end( Segment& s, const Point& p ) 
{
    return segment_access_traits<Segment>::set_end( s, construct< typename geometric_traits<Segment>::point_type >( p ) );
}

}//namespace geometrix;

#endif //GEOMETRIX_SEGMENT_TRAITS_HPP
