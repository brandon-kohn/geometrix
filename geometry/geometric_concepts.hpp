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

#include "affine_space_traits.hpp"
#include "reference_frame_traits.hpp"
#include "point_traits.hpp"
#include "vector_traits.hpp"
#include "segment_traits.hpp"
#include "point_sequence_traits.hpp"

#include <boost/concept_check.hpp>
#include <boost/type_traits.hpp>

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

    template <typename NumericSequence>
    struct NumericSequenceConcept
    {
        void constraints()
        {
            typedef typename numeric_sequence_traits<NumericSequence>::numeric_sequence_type numeric_sequence_type;
            typedef typename numeric_sequence_traits<NumericSequence>::numeric_type          numeric_type;            
            typedef typename numeric_sequence_traits<NumericSequence>::dimension_type        dimension_type;
            typedef typename numeric_sequence_traits<NumericSequence>::numeric_type          value_type;
        }
    };

    template <typename CoordinateSequence>
    struct CoordinateSequenceConcept
    {
        void constraints()
        {
            boost::function_requires< NumericSequenceConcept< CoordinateSequence > >();
            typedef typename coordinate_sequence_traits<CoordinateSequence>::coordinate_sequence_type coordinate_sequence_type;
            typedef typename coordinate_sequence_traits<CoordinateSequence>::coordinate_type          coordinate_type;
            //!TODO possibly add metric-identifying coordinate traits.
        }
    };

    template <typename IndexedSequence>
    struct RunTimeIndexedAccessConcept
    {
        void constraints()
        {
            typedef IndexedSequence                                                       indexed_sequence_type;
            typedef typename indexed_access_traits< indexed_sequence_type >::indexed_type indexed_type;
            indexed_sequence_type* iSequence = 0;
            indexed_type iValue = indexed_access_traits::get( *iSequence, 0 );///requires run-time indexed access on native integral types.
        }
    };

    template <typename IndexedSequence>
    struct CompileTimeIndexedAccessConcept
    {
        void constraints()
        {
            typedef IndexedSequence                                                       indexed_sequence_type;
            typedef typename indexed_access_traits< indexed_sequence_type >::indexed_type indexed_type;
            indexed_sequence_type* iSequence = 0;
            iValue = indexed_access_traits::get<0>( *iSequence );             ///requires compile-time indexed access on native integral types.
        }
    };

    template <typename IndexedSequence>
    struct IndexedAccessConcept
    {
        void constraints()
        {
            typedef IndexedSequence                                                       indexed_sequence_type;
            typedef typename indexed_access_traits< indexed_sequence_type >::indexed_type indexed_type;
            indexed_sequence_type* iSequence = 0;
            indexed_type iValue = indexed_access_traits::get( *iSequence, 0 );///requires run-time indexed access on native integral types.
            iValue = indexed_access_traits::get<0>( *iSequence );             ///requires compile-time indexed access on native integral types.
        }
    };

    //! Concept to describe a geometric point 
	template <typename Vector>
	struct VectorConcept
	{
		void constraints()
		{
			boost::function_requires< CoordinateSequenceConcept< Vector > >();

            //! Check that there is a greater than zero dimensionality
			BOOST_STATIC_ASSERT( boost::numeric::geometry::vector_traits<Vector>::dimension_type::value > 0 );
		}
	};

    //! Concept to describe a 2D vector
	template <typename Vector>
	struct Vector2DConcept
	{
		void constraints()
		{
			boost::function_requires< VectorConcept< Vector > >();        
			BOOST_STATIC_ASSERT( boost::numeric::geometry::vector_traits<Vector>::dimension_type::value == 2 );
		}
	};

    //! Concept to describe a 2D vector
	template <typename V>
	struct Vector3DConcept
	{
		void constraints()
		{
			boost::function_requires< VectorConcept< V > >();        
			BOOST_STATIC_ASSERT( boost::numeric::geometry::vector_traits<V>::dimension_type::value == 3 );
		}
	};

	//! Concept to describe a geometric point 
	template <typename Point>
	struct PointConcept
	{
		void constraints()
		{
			boost::function_requires< CoordinateSequenceConcept< Point > >();

            //! Check that there is a greater than zero dimensionality
			BOOST_STATIC_ASSERT( boost::numeric::geometry::point_traits<Point>::dimension_type::value > 0 );			
		}
	};

    //! Concept to describe a 2D point 
	template <typename Point>
	struct Point2DConcept
	{
		void constraints()
		{
			boost::function_requires< PointConcept< Point > >();
			BOOST_STATIC_ASSERT( boost::numeric::geometry::point_traits<Point>::dimension_type::value == 2 );
		}
	};

	//! Concept to describe a 3D point 
	template <typename Point>
	struct Point3DConcept
	{
		void constraints()
		{			
			boost::function_requires< PointConcept< Point > >();
			BOOST_STATIC_ASSERT( boost::numeric::geometry::point_traits<Point>::dimension_type::value == 3 );
		}
	};	

    //! \brief Concept definition of an affine space.
    template <typename AffineSpace>
    struct AffineSpaceConcept
    {
        void constraints()
        {
            //! defines a dimensionality greater than 0.
            typedef affine_space_traits< AffineSpace >::dimension_type dimension_type;
            BOOST_STATIC_ASSERT( dimension_type::value > 0 );

            //! defines a point concept.
            typedef affine_space_traits< AffineSpace >::point_type  point_type;
            boost::function_requires< PointConcept< point_type > >();
            
            //! point must have same dimension as affine space.
            BOOST_STATIC_ASSERT( point_traits< point_type >::dimension_type::value == dimension_type::value );
            
            //! defines a vector concept.
            typedef affine_space_traits< AffineSpace >::vector_type vector_type;
            boost::function_requires< VectorConcept< vector_type > >();

            //! vector must have same dimension as affine space.
            BOOST_STATIC_ASSERT( vector_traits< vector_type >::dimension_type::value == dimension_type::value );

            //! point p in {points} is related to vector v in {vectors} such that for any p and any v => p + v is an element of {points}.
            //! Roughly translated this means that the sum of a point and a vector must return a point of the same dimensionality.
            point_type* p;
            vector_type* v;
            point_type p2 = *p + *v;
            point_type p3 = *p - *v;
        }
    };

    template <typename ReferenceFrame>
    struct ReferenceFrameConcept
    {
        void constraints()
        {
            //! defines an affine space (rather has the same traits as one).
            typedef typename reference_frame_traits< ReferenceFrame >::affine_space_type affine_space_type;
            
            //! Associated affine space defines point and vector types.
            typedef typename affine_space_traits< affine_space_type >::point_type        point_type;

            //! defines a basis for the vector space of it's affine space.- implicit in vector type defined in affine space.
            typedef typename affine_space_traits< affine_space_type >::vector_type       vector_type;
            
            //! defines an origin.
            point_type origin = reference_frame_traits::get_origin();
        }
    };

    //! Concept of cartesian access
    template <typename AccessInterface>
    struct CartesianCoordinateAccessorConcept
    {
        typedef typename AccessInterface::sequence_type coordinate_sequence_type;
        typedef typename AccessInterface::indexed_type  coordinate_type;

        BOOST_CLASS_REQUIRE( coordinate_sequence_type, boost::numeric::geometry, CoordinateSequenceConcept );
        
        void constraints()
        {
            dimensional_constraints< coordinate_sequence_type >();
        }    

        //! older compilers require disambiguation
        template <int> struct disambiguation_tag { disambiguation_tag(int) {} };

        //! 2D access
        template <typename CoordinateSequence>
        typename boost::enable_if< boost::is_same< typename coordinate_sequence_traits< CoordinateSequence >::dimension_type, dimension_traits<2> >, void >::type dimensional_constraints( disambiguation_tag<0> = 0 )
        {
            coordinate_sequence_type* p = 0;
            coordinate_type x = AccessInterface::get<0>( *p );
            coordinate_type y = AccessInterface::get<1>( *p );

            //factory accessor
            *p = construction_traits< coordinate_sequence_type >::construct( x, y );
        }

        //! 3D access
        template <typename CoordinateSequence>
        typename boost::disable_if< boost::is_same< typename coordinate_sequence_traits< CoordinateSequence >::dimension_type, dimension_traits<2> >, void >::type dimensional_constraints( disambiguation_tag<1> = 0 )
        {            
            coordinate_sequence_type* p = 0;
            coordinate_type x = AccessInterface::get<0>( *p );
            coordinate_type y = AccessInterface::get<1>( *p );
            coordinate_type z = AccessInterface::get<2>( *p );

            //factory accessor
            *p = construction_traits< coordinate_sequence_type >::construct( x, y, y );
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
        typename boost::enable_if< boost::is_same< typename point_traits< Point >::dimension_type, dimension_traits<2> >, void >::type dimensional_constraints( disambiguation_tag<0> = 0 )
        {
            boost::function_requires< Point2DConcept< Point > >();
            point_type* p = 0;
            coordinate_type r = AccessInterface::get<0>( *p );
            coordinate_type t = AccessInterface::get<1>( *p );

            *p = construction_traits< point_type >::construct( r, t );
        }

        //! 3D access
        template <typename Point>
        typename boost::disable_if< boost::is_same< typename point_traits< Point >::dimension_type, dimension_traits<2> >, void >::type dimensional_constraints( disambiguation_tag<1> = 0 )
        {            
            boost::function_requires< Point3DConcept< Point > >();
            point_type* p = 0;
            coordinate_type r = AccessInterface::get<0>( *p );
            coordinate_type t = AccessInterface::get<1>( *p );
            coordinate_type phi = AccessInterface::get<2>( *p );

            *p = construction_traits< point_type >::construct( r, t, phi );
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

            segment_type p = construction_traits< segment_type >::construct( start, end );
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
