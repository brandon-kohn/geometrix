
//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_MESH_2D_TESTS_HPP
#define GEOMETRIX_MESH_2D_TESTS_HPP

#include <boost/test/included/unit_test.hpp>
#include <geometrix/test/test.hpp>
#include <geometrix/algorithm/mesh_2d.hpp>
#include <geometrix/utility/random_generator.hpp>
#include <geometrix/algorithm/point_in_polygon.hpp>
#include <geometrix/primitive/point_sequence.hpp>
#include <geometrix/algorithm/eberly_triangle_aabb_intersection.hpp>
#include <geometrix/algorithm/visible_vertices_mesh_search.hpp>

BOOST_AUTO_TEST_CASE( TestRandomPosition )
{
	using namespace geometrix;
	typedef point_double_2d point2;

	std::vector<point2> polygon{point2( 0., 0. ), point2( 10., 0. ), point2( 20., 10. ), point2( 20., 20. ), point2( 10., 20. ), point2( 10., 10. ), point2( 0., 10. )};	
	std::vector<std::size_t> iArray{6, 1, 5, 6, 0, 1, 2, 5, 1, 4, 5, 2, 4, 2, 3};
	std::vector<point<double, 2>> points{point2{0., 0.}, point2{10., 0.}, point2{20., 10.}, point2{20., 20.}, point2{10., 20.}, point2{10., 10.}, point2{0., 10.}};

	absolute_tolerance_comparison_policy<double> cmp( 1e-10 );
	mesh_2d mesh( points, iArray, cmp );

	//! Check the random generation interface.
	random_real_generator<> rnd;

	for( int i = 0; i < 100; ++i )
	{
		auto p = mesh.get_random_position( rnd(), rnd(), rnd() );
		BOOST_CHECK( point_in_polygon( p, polygon ) );
	}
}

BOOST_AUTO_TEST_CASE( TestVisibilitySearch )
{
	using namespace geometrix;
	typedef point_double_2d point2;

	std::vector<point2> polygon{point2( 0., 0. ), point2( 10., 0. ), point2( 20., 10. ), point2( 20., 20. ), point2( 10., 20. ), point2( 10., 10. ), point2( 0., 10. )};
	std::vector<std::size_t> iArray{6, 1, 5, 6, 0, 1, 2, 5, 1, 4, 5, 2, 4, 2, 3};
	std::vector<point<double, 2>> points{point2{0., 0.}, point2{10., 0.}, point2{20., 10.}, point2{20., 20.}, point2{10., 20.}, point2{10., 10.}, point2{0., 10.}};

	absolute_tolerance_comparison_policy<double> cmp( 1e-10 );
	mesh_2d mesh( points, iArray, cmp );

	point2 origin( 3., 8. );
	auto triangle = mesh.find_triangle( origin, cmp );
	BOOST_CHECK( triangle && *triangle != static_cast<std::size_t>(-1) );
	visible_vertices_mesh_search search( origin, *triangle, mesh );
	mesh.search( search );
	std::vector<std::size_t> const& vertices = search.get_vertices();
	std::vector<std::size_t> expected {6, 1, 5, 2, 0};
	BOOST_CHECK( vertices == expected );
}

#endif //GEOMETRIX_MESH_2D_TESTS_HPP
