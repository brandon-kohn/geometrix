//
//! Copyright © 2008-2009
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GENERATIVE_GEOMETRY_PRODUCTS_HPP
#define GENERATIVE_GEOMETRY_PRODUCTS_HPP
#pragma once

#include "point_traits.hpp"
#include "vector_traits.hpp"

namespace generative
{
namespace numeric
{
namespace geometry
{
    //! older compilers require disambiguation of like named functions
    template <int> struct disambiguation_tag { disambiguation_tag(int) {} };

    namespace detail_2D
    {
	    //! Function to perform a dot or inner product on two points.
	    template <typename Point>
        typename boost::enable_if< boost::is_same< typename point_traits<Point>::dimension_type, dimension_traits<2> >, typename point_traits<Point>::coordinate_type >::type dot_product( const Point& A, const Point& B/*, disambiguation_tag<0> = 0*/ )
	    {            
            typedef cartesian_access_traits< Point > access_traits;       
            boost::function_requires< Point2DConcept< Point > >();
            boost::function_requires< CartesianCoordinateAccessorConcept< access_traits > >();

            return ( access_traits::get<0>( A ) * access_traits::get<0>( B ) + access_traits::get<1>( A ) * access_traits::get<1>( B ) ); 
        }

        //! Function to find the cross product between two vectors formed by the specified three points which share an endpoint at A.
	    template <typename Point>
	    typename boost::enable_if< boost::is_same< typename point_traits<Point>::dimension_type, dimension_traits<2> >, typename point_traits<Point>::coordinate_type >::type cross_product( const Point& A, const Point& B, const Point& C, disambiguation_tag<1> = 0 )
	    {            
            typedef cartesian_access_traits< Point > access_traits;        
            boost::function_requires< Point2DConcept< Point > >();
            boost::function_requires< CartesianCoordinateAccessorConcept< access_traits > >();

            typename point_traits< Point >::coordinate_type cross = 
                (   
                    ( access_traits::get<0>( B ) - access_traits::get<0>( A ) ) * ( access_traits::get<1>( C ) - access_traits::get<1>( A ) ) 
                  - ( access_traits::get<0>( C ) - access_traits::get<0>( A ) ) * ( access_traits::get<1>( B ) - access_traits::get<1>( A ) ) 
                );
            return cross;        
        }

        //! Function to find the cross product between two vectors
	    template <typename Vector>
	    typename boost::enable_if< boost::is_same< typename point_traits<Vector>::dimension_type, dimension_traits<2> >, typename point_traits<Vector>::coordinate_type >::type cross_product( const Vector& A, const Vector& B, disambiguation_tag<2> = 0 )
	    {            
            typedef cartesian_access_traits<Vector> access_traits;        
            boost::function_requires< Point2DConcept< Vector > >();
            boost::function_requires< CartesianCoordinateAccessorConcept< access_traits > >();

            typename point_traits<Vector>::coordinate_type cross = ( access_traits::get<0>( A ) * access_traits::get<1>( B ) - access_traits::get<1>( A ) * access_traits::get<0>( B ) );
            return cross;        
        }
    }// detail_2D

    namespace detail_3D
    {
        //! Function to perform a dot or inner product on two points.
        template <typename Point>
        typename boost::disable_if< boost::is_same< typename point_traits<Point>::dimension_type, dimension_traits<2> >, typename point_traits<Point>::coordinate_type >::type dot_product( const Point& A, const Point& B  )
        {
            boost::function_requires< Point3DConcept<Point> >();
            typedef cartesian_access_traits< Point > access_traits;
            boost::function_requires< CartesianCoordinateAccessorConcept< access_traits > >();

            return ( access_traits::get<0>( A ) * access_traits::get<0>( B ) + access_traits::get<1>( A ) * access_traits::get<1>( B ) + access_traits::get<2>( A ) * access_traits::get<2>( B ) ); 
        }

        //! Function to find the cross product between two vectors formed by the specified three points which share an endpoint at A.
        template <typename Point>
        typename boost::disable_if< boost::is_same< typename point_traits<Point>::dimension_type, dimension_traits<2> >, typename point_traits<Point>::coordinate_type >::type cross_product( const Point& A, const Point& B, const Point& C, disambiguation_tag<4> = 0 )
        {
            boost::function_requires< Point3DConcept<Point> >();
            typedef cartesian_access_traits<Point> access_traits;
            boost::function_requires< CartesianCoordinateAccessorConcept< access_traits > >();

            typename point_traits<Point>::coordinate_type cross = 
                (
                      ( access_traits::get<1>( B ) - access_traits::get<1>( A ) ) * ( access_traits::get<2>( C ) - access_traits::get<2>( A ) ) - ( access_traits::get<2>( B ) - access_traits::get<2>( A ) ) * ( access_traits::get<1>( C ) - access_traits::get<1>( A ) ) 
                    - ( access_traits::get<0>( B ) - access_traits::get<0>( A ) ) * ( access_traits::get<2>( C ) - access_traits::get<2>( A ) ) - ( access_traits::get<2>( B ) - access_traits::get<2>( A ) ) * ( access_traits::get<0>( C ) - access_traits::get<0>( A ) )
                    + ( access_traits::get<0>( B ) - access_traits::get<0>( A ) ) * ( access_traits::get<1>( C ) - access_traits::get<1>( A ) ) - ( access_traits::get<1>( B ) - access_traits::get<1>( A ) ) * ( access_traits::get<0>( C ) - access_traits::get<0>( A ) )
                );
            return cross;        
        }

        //! Function to find the cross product between two vectors
        template <typename Vector>
        typename boost::disable_if< boost::is_same< typename point_traits<Vector>::dimension_type, dimension_traits<2> >, typename point_traits<Vector>::coordinate_type >::type cross_product( const Vector& A, const Vector& B, disambiguation_tag<5> = 0 )
        {
            boost::function_requires< Point3DConcept<Vector> >();
            typedef cartesian_access_traits<Vector> access_traits;
            boost::function_requires< CartesianCoordinateAccessorConcept< access_traits > >();

            typename point_traits<Vector>::coordinate_type cross = 
                (
                      ( access_traits::get<1>( A ) * access_traits::get<2>( B ) - access_traits::get<2>( A ) * access_traits::get<1>( B ) ) 
                    - ( access_traits::get<0>( A ) * access_traits::get<2>( B ) - access_traits::get<2>( A ) * access_traits::get<0>( B ) )
                    + ( access_traits::get<0>( A ) * access_traits::get<1>( B ) - access_traits::get<1>( A ) * access_traits::get<0>( B ) )
                );
            return cross;        
        }
    }//detail_3D

    //! workaround to disambiguate functions.
    using namespace detail_3D;
    using namespace detail_2D;
    
}}}//namespace generative::numeric::geometry;

#endif //GENERATIVE_GEOMETRY_PRODUCTS_HPP
