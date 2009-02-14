//
//! Copyright © 2008
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _GENERATIVE_GEOMETRY_POINT_SEQUENCE_TRAITS_HPP
#define _GENERATIVE_GEOMETRY_POINT_SEQUENCE_TRAITS_HPP
#pragma once

#include "point_traits.hpp"
#include <vector>

namespace generative
{
namespace numeric
{
namespace geometry
{

//! \brief Tag to check if a type is a point sequence type.

//! \ingroup Type Traits
template <typename PointSequence>
struct is_point_sequence : boost::false_type{};

//! \brief point_sequence_traits define the typedef and access interface to an ordered collection of points.

//! \ingroup PrimitiveTraits
//! \ingroup Primitives
//! The point_sequence_traits interface requires an stl conforming iterator iterface for the sequence type.
template <typename PointSequence>
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
    void constraints()
    {
        //! traits must define point type
        typedef typename point_sequence_traits< PointSequence >::point_type     point_type;

        //! Check that is is indeed a point.
        boost::function_requires< PointConcept< point_type > >();

        //! traits define iterator access?.. or should there be access traits?
        typedef typename point_sequence_traits< PointSequence >::iterator               iterator;
        typedef typename point_sequence_traits< PointSequence >::const_iterator         const_iterator;
        typedef typename point_sequence_traits< PointSequence >::reverse_iterator       reverse_iterator;
        typedef typename point_sequence_traits< PointSequence >::const_reverse_iterator const_reverse_iterator;

        //! Check the access interface.
        PointSequence* pSequence = 0;

        //! iterator access must be defined for both const_iterator and iterator types
        iterator it = point_sequence_traits< PointSequence >::begin( *pSequence );
        it = point_sequence_traits< PointSequence >::end( *pSequence );

        const_iterator cit = point_sequence_traits< PointSequence >::begin( *pSequence );
        cit = point_sequence_traits< PointSequence >::end( *pSequence );

        //! iterator access must be defined for both reverse_const_iterator and reverse_iterator types
        reverse_iterator rit = point_sequence_traits< PointSequence >::rbegin( *pSequence );
        rit = point_sequence_traits< PointSequence >::rend( *pSequence );

        const_reverse_iterator rcit = point_sequence_traits< PointSequence >::rbegin( *pSequence );
        rcit = point_sequence_traits< PointSequence >::rend( *pSequence );

        //! random access.
        const point_type& point1 = point_sequence_traits< PointSequence >::get_point( *pSequence, 0 );

        //! access the front
        const point_type& point2 = point_sequence_traits< PointSequence >::front( *pSequence );

        //! access the back
        const point_type& point3 = point_sequence_traits< PointSequence >::back( *pSequence );

        //! stl type stuff
        size_t s = point_sequence_traits< PointSequence >::size( *pSequence );
        bool empty = point_sequence_traits< PointSequence >::empty( *pSequence );
    }
};

//! Macro assumes point sequence can construct from input iterators pointing to points.
//! Assumes container has stl iterator interface + empty() and size().
#define BOOST_DEFINE_POINT_SEQUENCE_TRAITS( Point, Container )         \
template <> struct is_point_sequence< Container > : boost::true_type{};\
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
    typedef point_traits< point_type >::coordinate_type coordinate_type;         \
    typedef point_traits< point_type >::dimension_type  dimension_type;          \
    typedef container_type::iterator                    iterator;                \
    typedef container_type::const_iterator              const_iterator;          \
    typedef container_type::reverse_iterator            reverse_iterator;        \
    typedef container_type::const_reverse_iterator      const_reverse_iterator;  \
 	static inline iterator                              begin( container_type& p ) { return p.begin(); }         \
    static inline const_iterator                        begin( const container_type& p ) { return p.begin(); }   \
 	static inline iterator                              end( container_type& p ) { return p.end(); }             \
    static inline const_iterator                        end( const container_type& p ) { return p.end(); }       \
 	static inline reverse_iterator                      rbegin( container_type& p ) { return p.rbegin(); }       \
    static inline const_reverse_iterator                rbegin( const container_type& p ) { return p.rbegin(); } \
 	static inline reverse_iterator                      rend( container_type& p ) { return p.rend(); }           \
    static inline const_reverse_iterator                rend( const container_type& p ) { return p.rend(); }     \
 	static inline size_t                                size( const container_type& p ) { return p.size(); }     \
    static inline bool                                  empty( const container_type& p ) { return p.empty(); }   \
    static inline const point_type&                     get_point( const container_type& pointSequence, size_t index ) { const_iterator iter( pointSequence.begin() ); std::advance( iter, index ); return *iter; } \
    static inline point_type&                           get_point( container_type& pointSequence, size_t index ) { iterator iter( pointSequence.begin() ); std::advance( iter, index ); return *iter; }       \
    static inline const point_type&                     front( const container_type& pointSequence ) { return *pointSequence.begin(); }  \
    static inline point_type&                           front( container_type& pointSequence ) { return *pointSequence.begin(); }        \
    static inline const point_type&                     back( const container_type& pointSequence ) { const_iterator iter( pointSequence.begin() ); std::advance( iter, pointSequence.size() - 1 ); return *iter;} \
    static inline point_type&                           back( container_type& pointSequence ) { iterator iter( pointSequence.begin() ); std::advance( iter, pointSequence.size() - 1 ); return *iter; } \
    template <typename InputIterator>                   \
    static inline boost::shared_ptr< container_type >   construct( InputIterator first, InputIterator last ){ boost::shared_ptr< container_type > p( new container_type( first, last ) ); return p; } \
};

//! specialize vector.
#define BOOST_DEFINE_VECTOR_POINT_SEQUENCE_TRAITS( Point )                       \
template <> struct is_point_sequence< std::vector<Point> > : boost::true_type{}; \
template <>                                                                      \
struct point_sequence_traits< std::vector< Point > >                             \
{                                                                                \
 	typedef Point                                       point_type;              \
    typedef std::vector< point_type >                   container_type;          \
    typedef point_traits< point_type >::coordinate_type coordinate_type;         \
    typedef point_traits< point_type >::dimension_type  dimension_type;          \
    typedef container_type::iterator                    iterator;                \
    typedef container_type::const_iterator              const_iterator;          \
    typedef container_type::reverse_iterator            reverse_iterator;        \
    typedef container_type::const_reverse_iterator      const_reverse_iterator;  \
 	static inline iterator                              begin( container_type& p ) { return p.begin(); }        \
    static inline const_iterator                        begin( const container_type& p ) { return p.begin(); }  \
 	static inline iterator                              end( container_type& p ) { return p.end(); }            \
    static inline const_iterator                        end( const container_type& p ) { return p.end(); }      \
 	static inline reverse_iterator                      rbegin( container_type& p ) { return p.rbegin(); }      \
    static inline const_reverse_iterator                rbegin( const container_type& p ) { return p.rbegin(); }\
 	static inline reverse_iterator                      rend( container_type& p ) { return p.rend(); }          \
    static inline const_reverse_iterator                rend( const container_type& p ) { return p.rend(); }    \
    static inline size_t                                size( const container_type& p ) { return p.size(); }    \
    static inline bool                                  empty( const container_type& p ) { return p.empty(); }  \
    static inline const point_type&                     get_point( const container_type& pointSequence, size_t index ) { return pointSequence[index]; } \
    static inline point_type&                           get_point( container_type& pointSequence, size_t index ) { return pointSequence[index]; }       \
    static inline const point_type&                     front( const container_type& pointSequence ) { return pointSequence.front(); }                  \
    static inline point_type&                           front( container_type& pointSequence ) { return pointSequence.front(); }                        \
    static inline const point_type&                     back( const container_type& pointSequence ) { return pointSequence.back(); }                    \
    static inline point_type&                           back( container_type& pointSequence ) { return pointSequence.back(); }                          \
};

}}}//namespace generative::numeric::geometry;

#endif //_GENERATIVE_GEOMETRY_POINT_SEQUENCE_TRAITS_HPP
