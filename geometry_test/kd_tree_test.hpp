//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_KD_TREE_TEST_HPP
#define GEOMETRIX_KD_TREE_TEST_HPP

#include <boost/test/included/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <geometrix/primitive/point.hpp>
#include <geometrix/utility/utilities.hpp>
#include <geometrix/primitive/vector_point_sequence.hpp>
#include <geometrix/primitive/point_sequence_utilities.hpp>
#include <geometrix/utility/random_generator.hpp>
#include <geometrix/algorithm/euclidean_distance.hpp>
#include <geometrix/algorithm/kd_tree.hpp>
#include <geometrix/algorithm/median_partitioning_strategy.hpp>

#include <set>

template <typename PointSet>
struct point_visitor
{
    using point_set = PointSet;
    
    point_visitor( point_set& points )
        :m_pSet( points )
    {}

    template <typename Point>
    void operator()( const Point& p )
    {
        m_pSet.erase( p );
    }

    point_set& m_pSet;
};

//! Test a 2d case.
BOOST_AUTO_TEST_CASE( TestKDTree2d )
{
    using namespace geometrix;

    typedef point_double_2d point_2d;
    typedef point_double_3d point_3d;

    std::vector< point_2d > polygon;
    random_real_generator< boost::mt19937 > rnd(10.0);
    fraction_tolerance_comparison_policy<double> compare(1e-10);
    typedef std::set< point_2d, lexicographical_comparer< fraction_tolerance_comparison_policy<double> > > point_set;

    point_set points( compare );
    for( std::size_t i=0;i < 1000; ++i )
    {
        double x = rnd();
        double y = rnd();
        if( x <= 5.0 && y <= 5.0 )
        {
            points.insert( point_2d( x, y ) );
        }
        polygon.push_back( point_2d( x, y ) );
    }

    points.insert( point_2d( 6.0, 6.0 ) );
    polygon.push_back( point_2d( 6.0, 6.0 ) );

    kd_tree< point_2d > tree( polygon, compare, median_partitioning_strategy() );

    //! Specify a surface (square) with diagonal vector from 0,0, to 5,5 for the search range.
    axis_aligned_bounding_box< point_2d > range( point_2d( 0.0, 0.0 ), point_2d( 5.0, 5.0 ) );

    //! Visit all the points inside the surface and remove them from the set.
    point_visitor< point_set > visitor( points );
    tree.search( range, visitor, compare );

    //! If it worked, points should be only 1.
    BOOST_CHECK( points.size() == 1 );
}

//! Test a 3d case.
BOOST_AUTO_TEST_CASE( TestKDTree3d )
{
    using namespace geometrix;

    typedef point_double_3d point_3d;

    std::vector< point_3d > polygon;
    random_real_generator< boost::mt19937 > rnd(10.0);
    fraction_tolerance_comparison_policy<double> compare(1e-10);
    typedef std::multiset< point_3d, lexicographical_comparer< fraction_tolerance_comparison_policy<double> > > point_set;

    point_set points( compare );
    for( std::size_t i=0;i < 1000; ++i )
    {
        double x = rnd();
        double y = rnd();
        double z = rnd();
        if( x <= 5.0 && y <= 5.0 && z <= 5.0 )
        {
            points.insert( point_3d( x, y, z ) );
        }
        polygon.push_back( point_3d( x, y, z ) );
    }

    kd_tree< point_3d > tree( polygon, compare, median_partitioning_strategy() );

    //! Specify a volume (box) with diagonal vector from 0,0,0, to 5,5,5 for the search range.
    axis_aligned_bounding_box< point_3d > range( point_3d( 0.0, 0.0, 0.0 ), point_3d( 5.0, 5.0, 5.0 ) );

    //! Visit all the points inside the volume and remove them from the set.
    point_visitor< point_set > visitor( points );
    tree.search( range, visitor, compare );

    //! If it worked, points should be empty.
    BOOST_CHECK( points.empty() );

    BOOST_FOREACH( const point_3d& p, points )
    {
        std::cout << p.get<0>() << ", " << p.get<1>() << ", " << p.get<2>() << std::endl;
    }
}

template <std::size_t N, typename Point, typename NumberComparisonPolicy>
struct n_nearest_neighbor_search
{
    BOOST_MPL_ASSERT_MSG(
        (N>0),
        N_MUST_BE_GREATER_THAN_ZERO,
        (n_nearest_neighbor_search<N,Point,NumberComparisonPolicy>) );

    n_nearest_neighbor_search( const Point& p, const NumberComparisonPolicy& compare )
        : m_compare( p, compare )
    {}

    //! method to visit the nearest after the search.
    template <typename Visitor>
    void visit_nearest( Visitor& v ) const
    {
        std::for_each( m_nNearest.begin(), m_nNearest.end(), v );
    }

    const std::vector<Point>& get_points() const { return m_nNearest; }

    //! Operator to test each point found in the range on the tree.
    template <typename T>
    void operator()( const T& p ) const
    {
        if( m_nNearest.size() < N )
        {
            m_nNearest.push_back( p );

            //! if the size is now N.. sort it.
            if( m_nNearest.size() == N )
            {
                std::sort( m_nNearest.begin(), m_nNearest.end(), m_compare );
            }
        }
        else
        {
            //!Check if the value is less than the end.
            if( m_compare( p, m_nNearest.back() ) )
            {
                m_nNearest.pop_back();

                //! insert the new point
                auto iter = std::lower_bound( m_nNearest.begin(), m_nNearest.end(), p, m_compare );
                m_nNearest.insert( iter, p );
            }
        }
    }

    mutable std::vector< Point >                                 m_nNearest;
    geometrix::distance_compare< Point, NumberComparisonPolicy > m_compare;

};

struct point_printer
{
    point_printer( std::ostream& os )
        : m_os( os )
    {}

    template <typename Point>
    void operator()( const Point& p ) const
    {
        m_os << p.get<0>() << ", " << p.get<1>() << ", " << p.get<2>() << std::endl;
    }

    std::ostream& m_os;
};

//! Test a 3d case and report n nearest neighbors.
BOOST_AUTO_TEST_CASE( TestKDTreeNearest3d )
{
    using namespace geometrix;

    typedef point_double_3d point_3d;

    std::vector< point_3d > polygon;
    random_real_generator< boost::mt19937 > rnd(10.0);
    fraction_tolerance_comparison_policy<double> compare(1e-10);
    distance_compare< point_3d, fraction_tolerance_comparison_policy<double> > dCompare( point_3d(0.,0.,0.), compare );
    typedef std::multiset< point_3d, distance_compare< point_3d, fraction_tolerance_comparison_policy<double> > > point_set;
    point_set points( dCompare );
    for( std::size_t i=0;i < 1000; ++i )
    {
        double x = rnd();
        double y = rnd();
        double z = rnd();
        points.insert( point_3d( x, y, z ) );
        polygon.push_back( point_3d( x, y, z ) );
    }

    points.insert( point_3d( std::numeric_limits<double>::epsilon(), std::numeric_limits<double>::epsilon(), std::numeric_limits<double>::epsilon() ) );
    polygon.push_back( point_3d( std::numeric_limits<double>::epsilon(), std::numeric_limits<double>::epsilon(), std::numeric_limits<double>::epsilon() ) );

    kd_tree< point_3d > tree( polygon, compare, median_partitioning_strategy() );

    //! Specify a volume (box) with diagonal vector over the entire range.
    //! TODO: This is a bit awkward... should create a search with no input range to assume all.
    axis_aligned_bounding_box< point_3d > range( point_3d( 0.0, 0.0, 0.0 ), point_3d( 10.0, 10.0, 10.0 ) );

    //! Visit all the points inside the volume
    n_nearest_neighbor_search< 4, point_3d, fraction_tolerance_comparison_policy<double> > n_search( point_3d( 0.0, 0.0, 0.0 ), compare );
    tree.search( range, n_search, compare );

    //! Print the points.
    n_search.visit_nearest( point_printer( std::cout ) );

    double distanceA = point_point_distance( point_3d(0.,0.,0.), *n_search.get_points().begin() );
    double distanceB = point_point_distance( point_3d(0.,0.,0.), *points.begin() );
    BOOST_CHECK( compare.equals( distanceA, distanceB ) );
}

#endif //GEOMETRIX_KD_TREE_TEST_HPP
