//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_BOOLEAN_OPERATION_BSP_TEST_HPP
#define GEOMETRIX_BOOLEAN_OPERATION_BSP_TEST_HPP

#include <boost/test/unit_test.hpp>
#include <geometrix/numeric/number_comparison_policy.hpp>
#include <geometrix/primitive/point.hpp>
#include <geometrix/utility/utilities.hpp>
#include <geometrix/arithmetic/vector_arithmetic.hpp>
#include <geometrix/primitive/segment.hpp>
#include <geometrix/boolean_operations_bsp_2d.hpp>

#include <iostream>

template <typename Polygon>
void print_polygon( const Polygon& polygon )
{
    using namespace geometrix;
    typedef typename point_sequence_traits< Polygon >::point_type point_type;
    typedef cartesian_access_traits< point_type > point_access;

    std::cout << "-----------------------" << std::endl;
    BOOST_FOREACH( const point_type& p, polygon )
    {
        std::cout << "( " << point_access::get<0>( p ) << "," << point_access::get<1>( p ) << " )" << std::endl;
    }
}

BOOST_AUTO_TEST_CASE( TestBooleanBSP )
{
    using namespace geometrix;

    typedef point_double_2d point_2d;
    
    point_2d p1( 0., 0. );
    point_2d p2( 10., 0. );
    point_2d p3( 10., 10. );
    point_2d p4( 0., 10. );

    std::vector< point_2d > box1;
    box1.push_back( p1 );
    box1.push_back( p2 );
    box1.push_back( p3 );
    box1.push_back( p4 );
    box1.push_back( p1 );
    
    point_2d p5( 5., 5. );
    point_2d p6( 15., 5. ); 
    point_2d p7( 15., 15. );
    point_2d p8( 5., 15. );

    std::vector< point_2d > box2;
    box2.push_back( p5 );
    box2.push_back( p6 );
    box2.push_back( p7 );
    box2.push_back( p8 );
    box2.push_back( p5 );
    
    typedef doubly_connected_edge_list< point_2d, fraction_tolerance_comparison_policy<double> > dcel;

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

#endif //GEOMETRIX_BOOLEAN_OPERATION_BSP_TEST_HPP


