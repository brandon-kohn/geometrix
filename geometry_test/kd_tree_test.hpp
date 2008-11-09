//
//! Copyright © 2008
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_GEOMETRY_KD_TREE_TEST_HPP
#define _BOOST_GEOMETRY_KD_TREE_TEST_HPP
#pragma once

#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include "../geometry/point.hpp"
#include "../geometry/utilities.hpp"
#include "../geometry/point_sequence.hpp"
#include "../geometry/point_sequence_utilities.hpp"
#include "../geometry/random_generator.hpp"
#include "../geometry/kd_tree.hpp"

template <typename PointSet>
struct PointVisitor
{
    PointVisitor( PointSet& points )
        :m_pSet( points )
    {}

    template <typename Point>
    void operator()( const Point& p ) const 
    {
        m_pSet.erase( p );
    }

    mutable PointSet& m_pSet;
};

//! Test a 2D case.
BOOST_AUTO_TEST_CASE( TestKDTree2D )
{
	using namespace boost::numeric::geometry;

	typedef point_double_2d CPoint2D;
	typedef point_double_3d CPoint3D;
	
    std::vector< CPoint2D > polygon;
    random_real_generator< boost::mt19937 > rnd(10.0);
    fraction_tolerance_comparison_policy<double> compare(1e-10);
    typedef std::set< CPoint2D, lexicographical_compare< fraction_tolerance_comparison_policy<double> > > PointSet;

    PointSet points( compare );
    for( size_t i=0;i < 1000; ++i )
    {
        double x = rnd();
        double y = rnd();
        if( x <= 5.0 && y <= 5.0 )
        {
            points.insert( CPoint2D( x, y ) );
        }
        polygon.push_back( CPoint2D( x, y ) ); 
    }
    
    kd_tree< CPoint2D > tree( polygon, compare, median_partitioning_strategy() );

    //! Specify a surface (square) with diagonal vector from 0,0, to 5,5 for the search range.
    orthogonal_range< CPoint2D > range( CPoint2D( 0.0, 0.0 ), CPoint2D( 5.0, 5.0 ) );

    //! Visit all the points inside the surface and remove them from the set.
    PointVisitor< PointSet > visitor( points );
    tree.search( range, visitor, compare );   

    //! If it worked, points should be empty.
    BOOST_CHECK( points.empty() );
}

//! Test a 3D case.
BOOST_AUTO_TEST_CASE( TestKDTree3D )
{
	using namespace boost::numeric::geometry;

	typedef point_double_3d CPoint3D;
	
    std::vector< CPoint3D > polygon;
    random_real_generator< boost::mt19937 > rnd(10.0);
    fraction_tolerance_comparison_policy<double> compare(1e-10);
    typedef std::multiset< CPoint3D, lexicographical_compare< fraction_tolerance_comparison_policy<double> > > PointSet;

    PointSet points( compare );
    for( size_t i=0;i < 1000; ++i )
    {
        double x = rnd();
        double y = rnd();
        double z = rnd();
        if( x <= 5.0 && y <= 5.0 && z <= 5.0 )
        {
            points.insert( CPoint3D( x, y, z ) );
        }
        polygon.push_back( CPoint3D( x, y, z ) ); 
    }

    kd_tree< CPoint3D > tree( polygon, compare, median_partitioning_strategy() );

    //! Specify a volume (box) with diagonal vector from 0,0,0, to 5,5,5 for the search range.
    orthogonal_range< CPoint3D > range( CPoint3D( 0.0, 0.0, 0.0 ), CPoint3D( 5.0, 5.0, 5.0 ) );

    //! Visit all the points inside the volume and remove them from the set.
    PointVisitor< PointSet > visitor( points );
    tree.search( range, visitor, compare );   

    //! If it worked, points should be empty.
    BOOST_CHECK( points.empty() );

    BOOST_FOREACH( const CPoint3D& p, points )
    {
        std::cout << p.get<0>() << ", " << p.get<1>() << ", " << p.get<2>() << std::endl;
    }
}

#endif //_BOOST_GEOMETRY_KD_TREE_TEST_HPP
