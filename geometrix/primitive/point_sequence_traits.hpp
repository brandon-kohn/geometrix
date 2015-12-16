//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_POINT_SEQUENCE_TRAITS_HPP
#define GEOMETRIX_POINT_SEQUENCE_TRAITS_HPP

#include <geometrix/primitive/point_traits.hpp>
#include <vector>

namespace geometrix {

//! \brief Tag to check if a type is a point sequence type.

//! \ingroup Type Traits
template <typename PointSequence, typename Enable = void>
struct is_point_sequence : boost::false_type{};

//! \brief point_sequence_traits define the typedef and access interface to an ordered collection of points.

//! \ingroup PrimitiveTraits
//! \ingroup Primitives
//! The point_sequence_traits interface requires an stl conforming iterator iterface for the sequence type.
template <typename PointSequence, typename EnableIf=void>
struct point_sequence_traits
{
    BOOST_MPL_ASSERT_MSG( 
          ( false )
        , POINT_SEQUENCE_TRAITS_NOT_DEFINED
        , (PointSequence) );	
};

//! \brief Concept of a sequence of points.

//! \ingroup Concepts
//! \ingroup PrimitiveConcepts
//! Point sequences are constrained to define a point_type as well as to provide an iterator interface via the point_sequence_traits specialization.
template <typename PointSequence>
struct PointSequenceConcept
{
    //! traits must define point type
    typedef typename point_sequence_traits< PointSequence >::point_type     point_type;

    //! Check that is is indeed a point.
    BOOST_CONCEPT_ASSERT((PointConcept< point_type >));

    //! traits define iterator access?.. or should there be access traits?
    typedef typename point_sequence_traits< PointSequence >::iterator               iterator;
    typedef typename point_sequence_traits< PointSequence >::const_iterator         const_iterator;

#if defined(GEOMETRIX_USE_REVERSIBLE_POINT_SEQUENCE_TRAITS)
    typedef typename point_sequence_traits< PointSequence >::reverse_iterator       reverse_iterator;
    typedef typename point_sequence_traits< PointSequence >::const_reverse_iterator const_reverse_iterator;
#endif

    BOOST_CONCEPT_USAGE(PointSequenceConcept<PointSequence>)
    {        
        //! Check the access interface.
        PointSequence* pSequence = 0;

        //! iterator access must be defined for both const_iterator and iterator types
        iterator it = point_sequence_traits< PointSequence >::begin( *pSequence );
        it = point_sequence_traits< PointSequence >::end( *pSequence );

        const_iterator cit = point_sequence_traits< PointSequence >::begin( *pSequence );
        cit = point_sequence_traits< PointSequence >::end( *pSequence );

#if defined(GEOMETRIX_USE_REVERSIBLE_POINT_SEQUENCE_TRAITS)
        //! iterator access must be defined for both reverse_const_iterator and reverse_iterator types
        reverse_iterator rit = point_sequence_traits< PointSequence >::rbegin( *pSequence );
        rit = point_sequence_traits< PointSequence >::rend( *pSequence );

        const_reverse_iterator rcit = point_sequence_traits< PointSequence >::rbegin( *pSequence );
        rcit = point_sequence_traits< PointSequence >::rend( *pSequence );
#endif

        //! random access.
        const point_type& point1 = point_sequence_traits< PointSequence >::get_point( *pSequence, 0 );
        boost::ignore_unused_variable_warning(point1);

        //! access the front
        const point_type& point2 = point_sequence_traits< PointSequence >::front( *pSequence );
        boost::ignore_unused_variable_warning(point2);

        //! access the back
        const point_type& point3 = point_sequence_traits< PointSequence >::back( *pSequence );
        boost::ignore_unused_variable_warning(point3);

        //! stl type stuff
        std::size_t s = point_sequence_traits< PointSequence >::size( *pSequence );
        boost::ignore_unused_variable_warning(s);
        bool empty = point_sequence_traits< PointSequence >::empty( *pSequence );
        boost::ignore_unused_variable_warning(empty);
    }
};

enum polygon_vertex_order { e_clockwise, e_counterclockwise };

//! Macro assumes point sequence can construct from input iterators pointing to points.
//! Assumes container has stl iterator interface + empty() and size().
#define GEOMETRIX_DEFINE_POINT_SEQUENCE_TRAITS( Point, Container ) \
namespace geometrix {                                   \
template <> struct is_point_sequence< Container > : boost::true_type{};      \
template <>                                                            \
struct point_sequence_traits< Container >                              \
{                                                                      \
        BOOST_MPL_ASSERT_MSG(                                          \
        ( !boost::is_same< std::vector< Point >, Container >::value )  \
        , USE_BOOST_DEFINE_VECTOR_POINT_SEQUENCE_TRAITS                \
        , (Container) );   	                                           \
                                                                       \
    typedef Point                                       point_type;              \
    typedef Container                                   container_type;          \
    typedef geometric_traits< point_type >::dimension_type  dimension_type;          \
    typedef container_type::iterator                    iterator;                \
    typedef container_type::const_iterator              const_iterator;          \
    static iterator                              begin( container_type& p ) { return p.begin(); }         \
    static const_iterator                        begin( const container_type& p ) { return p.begin(); }   \
    static iterator                              end( container_type& p ) { return p.end(); }             \
    static const_iterator                        end( const container_type& p ) { return p.end(); }       \
    static std::size_t                           size( const container_type& p ) { return p.size(); }     \
    static bool                                  empty( const container_type& p ) { return p.empty(); }   \
    static const point_type&                     get_point( const container_type& pointSequence, std::size_t index ) { const_iterator iter( pointSequence.begin() ); std::advance( iter, index ); return *iter; } \
    static point_type&                           get_point( container_type& pointSequence, std::size_t index ) { iterator iter( pointSequence.begin() ); std::advance( iter, index ); return *iter; }       \
    static const point_type&                     front( const container_type& pointSequence ) { return *pointSequence.begin(); }  \
    static point_type&                           front( container_type& pointSequence ) { return *pointSequence.begin(); }        \
    static const point_type&                     back( const container_type& pointSequence ) { const_iterator iter( pointSequence.begin() ); std::advance( iter, pointSequence.size() - 1 ); return *iter;} \
    static point_type&                           back( container_type& pointSequence ) { iterator iter( pointSequence.begin() ); std::advance( iter, pointSequence.size() - 1 ); return *iter; } \
    template <typename InputIterator>                   \
    static container_type                        construct( InputIterator first, InputIterator last ){ return container_type( first, last ); } \
}; \
}  \
/***/

//! specialize vector.
#define GEOMETRIX_DEFINE_VECTOR_POINT_SEQUENCE_TRAITS( Point )         \
namespace geometrix {                                       \
template <> struct is_point_sequence< std::vector<Point> > : boost::true_type{}; \
template <>                                                                      \
struct point_sequence_traits< std::vector< Point > >                             \
{                                                                                \
    typedef Point                                       point_type;              \
    typedef std::vector< point_type >                   container_type;          \
    typedef geometric_traits< point_type >::dimension_type  dimension_type;          \
    typedef container_type::iterator                    iterator;                \
    typedef container_type::const_iterator              const_iterator;          \
    typedef container_type::reverse_iterator            reverse_iterator;        \
    typedef container_type::const_reverse_iterator      const_reverse_iterator;  \
    static iterator                              begin( container_type& p ) { return p.begin(); }        \
    static const_iterator                        begin( const container_type& p ) { return p.begin(); }  \
    static iterator                              end( container_type& p ) { return p.end(); }            \
    static const_iterator                        end( const container_type& p ) { return p.end(); }      \
    static reverse_iterator                      rbegin( container_type& p ) { return p.rbegin(); }      \
    static const_reverse_iterator                rbegin( const container_type& p ) { return p.rbegin(); }\
    static reverse_iterator                      rend( container_type& p ) { return p.rend(); }          \
    static const_reverse_iterator                rend( const container_type& p ) { return p.rend(); }    \
    static std::size_t                                size( const container_type& p ) { return p.size(); }    \
    static bool                                  empty( const container_type& p ) { return p.empty(); }  \
    static const point_type&                     get_point( const container_type& pointSequence, std::size_t index ) { return pointSequence[index]; } \
    static point_type&                           get_point( container_type& pointSequence, std::size_t index ) { return pointSequence[index]; }       \
    static const point_type&                     front( const container_type& pointSequence ) { return pointSequence.front(); }                  \
    static point_type&                           front( container_type& pointSequence ) { return pointSequence.front(); }                        \
    static const point_type&                     back( const container_type& pointSequence ) { return pointSequence.back(); }                    \
    static point_type&                           back( container_type& pointSequence ) { return pointSequence.back(); }                          \
}; \
}  \
/***/

}//namespace geometrix;

#endif //GEOMETRIX_POINT_SEQUENCE_TRAITS_HPP
