//
//!  Copyright (c) 2008
//!  Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_GEOMETRY_GEOMETRIC_CONCEPTS_HPP
#define _BOOST_GEOMETRY_GEOMETRIC_CONCEPTS_HPP
#pragma once

#include <boost/concept_check.hpp>
#include <boost/type_traits.hpp>
#include <boost/mpl/assert.hpp>
#include "point_traits.hpp"
#include "segment_traits.hpp"
#include "point_sequence_traits.hpp"

namespace boost
{
namespace numeric
{
namespace geometry
{
	//! A concept check to ensure a type is arithmetic.
    template <class T>
    struct IsArithmeticConcept
    {
		void constraints() 
		{
			BOOST_STATIC_ASSERT( boost::is_arithmetic<T>::value );
		}
	};

	//! Concept to describe a geometric point 
	template <typename P>
	struct PointConcept
	{
		void constraints()
		{
			//! traits must define coordinate type.
			typedef boost::numeric::geometry::point_traits<P>::coordinate_type coordinate_type;

            //! traits must define dimension type;
            typedef boost::numeric::geometry::point_traits<P>::dimension_type dimension_type;

            //! require the coordinate type to be arithmetic (maybe not...what about user types?)...
			//boost::function_requires< IsArithmeticConcept< coordinate_type > >();

            //! Check that there is a greater than zero dimensionality
			BOOST_STATIC_ASSERT( dimension_type::value > 0 );			
		}
	};

    //! Concept to describe a 2D point 
	template <typename P>
	struct Point2DConcept
	{
        typedef typename point_traits<P>::coordinate_type coordinate_type;

		void constraints()
		{
			boost::function_requires< PointConcept< P > >();        
			BOOST_STATIC_ASSERT( boost::numeric::geometry::point_traits<P>::dimension_type::value == 2 );
		}
	};

	//! Concept to describe a 3D point 
	template <typename P>
	struct Point3DConcept
	{
        typedef typename point_traits<P>::coordinate_type coordinate_type;

		void constraints()
		{			
			boost::function_requires< PointConcept< P > >();
			BOOST_STATIC_ASSERT( boost::numeric::geometry::point_traits<P>::dimension_type::value == 3 );
		}
	};	

    //! Concept of cartesian access
    template <typename AccessInterface>
    struct CartesianCoordinateAccessorConcept
    {
        typedef typename AccessInterface::point_type               point_type;
        typedef typename AccessInterface::coordinate_type          coordinate_type;

        BOOST_CLASS_REQUIRE( point_type, boost::numeric::geometry, PointConcept );        
        
        void constraints()
        {
            dimensional_constraints< point_type >();
        }    

        //! older compilers require disambiguation
        template <int> struct disambiguation_tag { disambiguation_tag(int) {} };

        //! 2D access
        template <typename Point>
        typename boost::enable_if< boost::is_same< typename point_traits< Point >::dimension_type, dimension_tag<2> >, void >::type dimensional_constraints( disambiguation_tag<0> = 0 )
        {
            point_type* p = 0;
            coordinate_type x = AccessInterface::get_x( *p );
            coordinate_type y = AccessInterface::get_y( *p );

            //factory accessor
            *p = AccessInterface::construct< point_type >( x, y );
        }

        //! 3D access
        template <typename Point>
        typename boost::disable_if< boost::is_same< typename point_traits< Point >::dimension_type, dimension_tag<2> >, void >::type dimensional_constraints( disambiguation_tag<1> = 0 )
        {            
            point_type* p = 0;
            coordinate_type x = AccessInterface::get_x( *p );
            coordinate_type y = AccessInterface::get_y( *p );
            coordinate_type z = AccessInterface::get_z( *p );

            //factory accessor
            *p = AccessInterface::construct< point_type >( x, y, y );
        }

    };

    //! Concept of polar/spherical coordinate access (3D assumes spherical rather than cylindrical).
    template <typename AccessInterface>
    struct PolarCoordinateAccessorConcept
    {
        typedef typename AccessInterface::point_type               point_type;
        typedef typename AccessInterface::coordinate_type          coordinate_type;

        BOOST_CLASS_REQUIRE( point_type, boost::numeric::geometry, PointConcept ); 

        void constraints()
        {
            dimensional_constraints< point_type >();
        }

        //! older compilers require disambiguation
        template <int> struct disambiguation_tag { disambiguation_tag(int) {} };
        
        //! 2D access
        template <typename Point>
        typename boost::enable_if< boost::is_same< typename point_traits< Point >::dimension_type, dimension_tag<2> >, void >::type dimensional_constraints( disambiguation_tag<0> = 0 )
        {
            boost::function_requires< Point2DConcept< Point > >();
            point_type* p = 0;
            coordinate_type r = AccessInterface::get_radius( *p );
            coordinate_type t = AccessInterface::get_theta( *p );

            *p = AccessInterface::construct< point_type >( r, t );
        }

        //! 3D access
        template <typename Point>
        typename boost::disable_if< boost::is_same< typename point_traits< Point >::dimension_type, dimension_tag<2> >, void >::type dimensional_constraints( disambiguation_tag<1> = 0 )
        {            
            boost::function_requires< Point3DConcept< Point > >();
            point_type* p = 0;
            coordinate_type r = AccessInterface::get_radius( *p );
            coordinate_type t = AccessInterface::get_theta( *p );
            coordinate_type phi = AccessInterface::get_phi( *p );

            *p = AccessInterface::construct< point_type >( r, t, phi );
        }
    };

    //! Concept of a segment
    template <typename Segment>
    struct SegmentConcept
    {
        void constraints()
        {
            //! traits must define point type.
            typedef typename segment_traits< Segment >::point_type point_type;
            
            //! Check that is is indeed a point.
            boost::function_requires< PointConcept< point_type > >();

            //!TODO: What else?
        }
    };

    //! Concept of segment access interface
    template <typename AccessInterface>
    struct SegmentAccessorConcept
    {
        typedef typename AccessInterface::segment_type               segment_type;  
        typedef typename segment_traits< segment_type >::point_type  point_type;
        BOOST_CLASS_REQUIRE( segment_type, boost::numeric::geometry, SegmentConcept );        

        void constraints()
        {            
            segment_type* s = 0;
            point_type start = AccessInterface::get_start( *s );
            point_type end = AccessInterface::get_end( *s );

            AccessInterface::set_start( *s, start );
            AccessInterface::set_end( *s, end );       

            segment_type p = AccessInterface::construct( start, end );
        }

    };

    //! Concept of a point sequence
    template <typename PointSequence>
    struct PointSequenceConcept
    {
        void constraints()
        {
            //! traits must define point type
            typedef typename point_sequence_traits< PointSequence >::point_type     point_type;

            //Check that is is indeed a point.
            boost::function_requires< PointConcept< point_type > >();

            //! traits define iterator acces?.. or should there be access traits?
            typedef typename point_sequence_traits< PointSequence >::iterator       iterator;
            typedef typename point_sequence_traits< PointSequence >::const_iterator const_iterator;

            //Check the access interface.
            PointSequence* pSequence = 0;
            iterator it = pSequence->begin();
            it = pSequence->end();

            const_iterator cit = pSequence->begin();
            cit = pSequence->end();

            //! random access.
            const point_type& point = (*pSequence)[0];

            //! stl type stuff
            size_t s = pSequence->size();
            bool empty = pSequence->empty();
        }
    };

}}}//namespace boost::numeric::geometry;

#endif //_BOOST_GEOMETRY_GEOMETRIC_CONCEPTS_HPP
