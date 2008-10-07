//
//!  Copyright (c) 2008
//!  Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_GEOMETRY_BOOLEAN_OPERATION_BSP_TEST_HPP
#define _BOOST_GEOMETRY_BOOLEAN_OPERATION_BSP_TEST_HPP
#pragma once

#include <boost/test/unit_test.hpp>
#include "../geometry/number_comparison_policy.hpp"
#include "../geometry/point.hpp"
#include "../geometry/utilities.hpp"
#include "../geometry/products.hpp"
#include "../geometry/segment.hpp"
#include "../geometry/boolean_operations_bsp_2d.hpp"

#include <iostream>

using namespace boost::numeric::geometry;


template <typename Polygon>
void print_polygon( const Polygon& polygon )
{
    typedef typename point_sequence_traits< Polygon >::point_type point_type;
    typedef cartesian_access_traits< point_type > point_access;

    std::cout << "-----------------------" << std::endl;
    BOOST_FOREACH( const point_type& p, polygon )
    {
        std::cout << "( " << point_access::get_x( p ) << "," << point_access::get_y( p ) << " )" << std::endl;
    }
}

BOOST_AUTO_TEST_CASE( TestBooleanBSP )
{
	typedef point_double_2d CPoint2D;
	
	CPoint2D p1( 0., 0. );
    CPoint2D p2( 10., 0. );
    CPoint2D p3( 10., 10. );
    CPoint2D p4( 0., 10. );

    std::vector< CPoint2D > box1;
    box1.push_back( p1 );
    box1.push_back( p2 );
    box1.push_back( p3 );
    box1.push_back( p4 );
    box1.push_back( p1 );
    
    CPoint2D p5( 5., 5. );
    CPoint2D p6( 15., 5. ); 
    CPoint2D p7( 15., 15. );
    CPoint2D p8( 5., 15. );

    std::vector< CPoint2D > box2;
    box2.push_back( p5 );
    box2.push_back( p6 );
    box2.push_back( p7 );
    box2.push_back( p8 );
    box2.push_back( p5 );
    
    typedef doubly_connected_edge_list< CPoint2D, fraction_tolerance_comparison_policy<double> > dcel;

    fraction_tolerance_comparison_policy<double> compare(1e-10);
    dcel::face_collection_ptr pFaces;
    boost::shared_ptr< dcel > intersectionBox;

    std::cout <<"Intersection" << std::endl;
    intersectionBox = boolean_intersection( box1, box2, compare );

    pFaces = intersectionBox->get_faces();
    BOOST_FOREACH( dcel::face_ptr pFace, *pFaces )
    {
        print_polygon( *pFace );
    }  

    std::cout <<"Intersecion_DCEL" << std::endl;
    intersectionBox = dcel_boolean_intersection( *create_dcel( box1, compare ), *create_dcel( box2, compare ), compare ); 
    pFaces = intersectionBox->get_faces();
    BOOST_FOREACH( dcel::face_ptr pFace, *pFaces )
    {
        print_polygon( *pFace );
    }  

    std::cout <<"Union" << std::endl;
    intersectionBox = boolean_union( box1, box2, compare );
    pFaces = intersectionBox->get_faces();
    BOOST_FOREACH( dcel::face_ptr pFace, *pFaces )
    {
        print_polygon( *pFace );
    }  

    std::cout <<"Union_DCEL" << std::endl;
    intersectionBox = dcel_boolean_union( *create_dcel( box1, compare ), *create_dcel( box2, compare ), compare ); 
    pFaces = intersectionBox->get_faces();
    BOOST_FOREACH( dcel::face_ptr pFace, *pFaces )
    {
        print_polygon( *pFace );
    }  

    std::cout <<"Difference" << std::endl;
    intersectionBox = boolean_difference( box1, box2, compare );

    pFaces = intersectionBox->get_faces();
    BOOST_FOREACH( dcel::face_ptr pFace, *pFaces )
    {
        print_polygon( *pFace );
    }  

    std::cout <<"Difference_DCEL" << std::endl;
    intersectionBox = dcel_boolean_difference( *create_dcel( box1, compare ), *create_dcel( box2, compare ), compare ); 
    pFaces = intersectionBox->get_faces();
    BOOST_FOREACH( dcel::face_ptr pFace, *pFaces )
    {
        print_polygon( *pFace );
    }  

    std::cout <<"Exclusive-Or" << std::endl;
    intersectionBox = boolean_exclusive_or( box1, box2, compare );
    pFaces = intersectionBox->get_faces();
    BOOST_FOREACH( dcel::face_ptr pFace, *pFaces )
    {
        print_polygon( *pFace );
    }  

    std::cout <<"Exclusive-Or_DCEL" << std::endl;
    intersectionBox = dcel_boolean_exclusive_or( *create_dcel( box1, compare ), *create_dcel( box2, compare ), compare ); 
    pFaces = intersectionBox->get_faces();
    BOOST_FOREACH( dcel::face_ptr pFace, *pFaces )
    {
        print_polygon( *pFace );
    }  

}

#endif //_BOOST_GEOMETRY_BOOLEAN_OPERATION_BSP_TEST_HPP


