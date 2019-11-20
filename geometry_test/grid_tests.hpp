//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_GRID_TESTS_HPP
#define GEOMETRIX_GRID_TESTS_HPP

#include <boost/test/included/unit_test.hpp>
#include <geometrix/test/test.hpp>
#include <geometrix/numeric/number_comparison_policy.hpp>
#include <geometrix/primitive/point.hpp>
#include <geometrix/algorithm/grid_traits.hpp>
#include <geometrix/algorithm/grid_2d.hpp>
#include <geometrix/algorithm/hash_grid_2d.hpp>
#include <geometrix/algorithm/orientation_grid_traversal.hpp>
#include <geometrix/algorithm/fast_voxel_grid_traversal.hpp>
#include <geometrix/algorithm/floodfill_grid_traversal.hpp>
#include <geometrix/primitive/segment.hpp>
#include <geometrix/utility/ignore_unused_warnings.hpp>
#include <iostream>

BOOST_AUTO_TEST_CASE( TestGrid )
{
    using namespace geometrix;

    double xmin = 0;
    double xmax = 16100;
    double ymin = 0;
    double ymax = 16100;

    //! 10x10 miles.
    absolute_tolerance_comparison_policy<double> cmp(1e-10);
    grid_traits<double> grid(xmin, xmax, ymin, ymax, 2.0);

    //! Test Centroid
    {
        BOOST_AUTO(centroid, grid.get_cell_centroid(0, 0));
        GEOMETRIX_CHECK_SEQUENCE_CLOSE_2D(centroid, (1., 1.), 1e-10);
    }
    
    {
        BOOST_AUTO(centroid, grid.get_cell_centroid(1, 1));
        GEOMETRIX_CHECK_SEQUENCE_CLOSE_2D(centroid, (3., 3.), 1e-10);
    }

    //! Test corner points
    {
        BOOST_AUTO(point0, grid.get_cell_corner0(0, 0));
        GEOMETRIX_CHECK_SEQUENCE_CLOSE_2D(point0, (0., 0.), 1e-10);

        BOOST_AUTO(point1, grid.get_cell_corner1(0, 0));
        GEOMETRIX_CHECK_SEQUENCE_CLOSE_2D(point1, (2., 0.), 1e-10);

        BOOST_AUTO(point2, grid.get_cell_corner2(0, 0));
        GEOMETRIX_CHECK_SEQUENCE_CLOSE_2D(point2, (2., 2.), 1e-10);
        
        BOOST_AUTO(point3, grid.get_cell_corner3(0, 0));
        GEOMETRIX_CHECK_SEQUENCE_CLOSE_2D(point3, (0., 2.), 1e-10);
    }

    //! memory test.
    //struct Cell { std::vector<int>* agents; std::vector<int>* obstacles; };
    //std::vector< std::vector<Cell> > data(grid.get_height(), std::vector<Cell>(grid.get_width()));//! ~8GB to build.
    //BOOST_CHECK(data.size() == grid.get_height());
    //BOOST_CHECK(data[0].size() == grid.get_width());
}

BOOST_AUTO_TEST_CASE(TestGridOrientationTraversal)
{
    using namespace geometrix;

    double xmin = -20;
    double xmax = 20;
    double ymin = -20;
    double ymax = 20;

    absolute_tolerance_comparison_policy<double> cmp(1e-10);
    grid_traits<double> grid(xmin, xmax, ymin, ymax, 1.0);
    struct CheckingVisitor 
    {
        CheckingVisitor(std::vector<std::pair<std::size_t, std::size_t> >& indices)
            : indices(indices)
        {}
        
        void operator()(std::size_t i, std::size_t j)
        {
            indices.emplace_back(i, j);
        }

        std::vector<std::pair<std::size_t, std::size_t> >& indices; 
    };

    {
        std::vector<std::pair<std::size_t, std::size_t> > indices;
        orientation_segment_traversal(grid, segment<point_double_2d>(point_double_2d(0., 0.), point_double_2d(1., 1.)), CheckingVisitor(indices), cmp);
        BOOST_CHECK(indices.size() == 2);
        BOOST_CHECK(indices[0].first == 20 && indices[0].second == 20);
        BOOST_CHECK(indices[1].first == 21 && indices[1].second == 21);
    }

    {
        std::vector<std::pair<std::size_t, std::size_t> > indices;
        orientation_segment_traversal(grid, segment<point_double_2d>(point_double_2d(0., 0.), point_double_2d(.5, .5)), CheckingVisitor(indices), cmp);
        BOOST_CHECK(indices.size() == 1);
        BOOST_CHECK(indices[0].first == 20 && indices[0].second == 20);
    }

    //! segment along +x-axis.
    {
        std::vector<std::pair<std::size_t, std::size_t> > indices;
        orientation_segment_traversal(grid, segment<point_double_2d>(point_double_2d(0., 0.), point_double_2d(2., 0.)), CheckingVisitor(indices), cmp);
        BOOST_CHECK(indices.size() == 3);
        BOOST_CHECK(indices[0].first == 20 && indices[0].second == 20);        
        BOOST_CHECK(indices[1].first == 21 && indices[1].second == 20);
        BOOST_CHECK(indices[2].first == 22 && indices[2].second == 20);
    }

    //! reversed segment along +x-axis.
    {
        std::vector<std::pair<std::size_t, std::size_t> > indices;
        orientation_segment_traversal(grid, segment<point_double_2d>(point_double_2d(2., 0.), point_double_2d(0., 0.)), CheckingVisitor(indices), cmp);
        BOOST_CHECK(indices.size() == 3);
        BOOST_CHECK(indices[0].first == 20 && indices[0].second == 20);
        BOOST_CHECK(indices[1].first == 21 && indices[1].second == 20);
        BOOST_CHECK(indices[2].first == 22 && indices[2].second == 20);
    }
    
    //! segment along -x-axis.
    {
        std::vector<std::pair<std::size_t, std::size_t> > indices;
        orientation_segment_traversal(grid, segment<point_double_2d>(point_double_2d(-2., 0.), point_double_2d(0., 0.)), CheckingVisitor(indices), cmp);
        BOOST_CHECK(indices.size() == 3);
        BOOST_CHECK(indices[0].first == 18 && indices[0].second == 20);
        BOOST_CHECK(indices[1].first == 19 && indices[1].second == 20);
        BOOST_CHECK(indices[2].first == 20 && indices[2].second == 20);
    }
    
    //! segment along +y-axis.
    {
        std::vector<std::pair<std::size_t, std::size_t> > indices;
        orientation_segment_traversal(grid, segment<point_double_2d>(point_double_2d(0., 0.), point_double_2d(0., 2.)), CheckingVisitor(indices), cmp);
        BOOST_CHECK(indices.size() == 3);
        BOOST_CHECK(indices[0].first == 20 && indices[0].second == 20);
        BOOST_CHECK(indices[1].first == 20 && indices[1].second == 21);
        BOOST_CHECK(indices[2].first == 20 && indices[2].second == 22);
    }

    //! reversed segment along +y-axis.
    {
        std::vector<std::pair<std::size_t, std::size_t> > indices;
        orientation_segment_traversal(grid, segment<point_double_2d>(point_double_2d(0., 2.), point_double_2d(0., 0.)), CheckingVisitor(indices), cmp);
        BOOST_CHECK(indices.size() == 3);
        BOOST_CHECK(indices[0].first == 20 && indices[0].second == 20);
        BOOST_CHECK(indices[1].first == 20 && indices[1].second == 21);
        BOOST_CHECK(indices[2].first == 20 && indices[2].second == 22);
    }

    //! segment in range of (45,90) degrees.
    {
        std::vector<std::pair<std::size_t, std::size_t> > indices;
        orientation_segment_traversal(grid, segment<point_double_2d>(point_double_2d(0.4, 0.5), point_double_2d(1.1, 3.)), CheckingVisitor(indices), cmp);
        BOOST_CHECK(indices.size() == 5);
        BOOST_CHECK(indices[0].first == 20 && indices[0].second == 20);
        BOOST_CHECK(indices[1].first == 20 && indices[1].second == 21);
        BOOST_CHECK(indices[2].first == 20 && indices[2].second == 22);
        BOOST_CHECK(indices[3].first == 21 && indices[3].second == 22);
        BOOST_CHECK(indices[4].first == 21 && indices[4].second == 23);
    }

    //! segment in range of (270,315) degrees.
    {
        std::vector<std::pair<std::size_t, std::size_t> > indices;
        orientation_segment_traversal(grid, segment<point_double_2d>(point_double_2d(0.4, -0.5), point_double_2d(1.1, -3.)), CheckingVisitor(indices), cmp);
        BOOST_CHECK(indices.size() == 4);
        BOOST_CHECK(indices[0].first == 20 && indices[0].second == 19);
        BOOST_CHECK(indices[1].first == 20 && indices[1].second == 18);
        BOOST_CHECK(indices[2].first == 20 && indices[2].second == 17);
        BOOST_CHECK(indices[3].first == 21 && indices[3].second == 17);
    }

    //! fast voxel method.
    {
        std::vector<std::pair<std::size_t, std::size_t> > indices;
        fast_voxel_grid_traversal(grid, segment<point_double_2d>(point_double_2d(0., 0.), point_double_2d(1., 1.)), CheckingVisitor(indices), cmp);
        BOOST_CHECK(indices.size() == 3);
        BOOST_CHECK(indices[0].first == 20 && indices[0].second == 20);
        BOOST_CHECK(indices[1].first == 20 && indices[1].second == 21);
        BOOST_CHECK(indices[2].first == 21 && indices[2].second == 21);
    }

    {
        std::vector<std::pair<std::size_t, std::size_t> > indices;
        fast_voxel_grid_traversal(grid, segment<point_double_2d>(point_double_2d(0., 0.), point_double_2d(.5, .5)), CheckingVisitor(indices), cmp);
        BOOST_CHECK(indices.size() == 1);
        BOOST_CHECK(indices[0].first == 20 && indices[0].second == 20);
    }

    //! segment along +x-axis.
    {
        std::vector<std::pair<std::size_t, std::size_t> > indices;
        fast_voxel_grid_traversal(grid, segment<point_double_2d>(point_double_2d(0., 0.), point_double_2d(2., 0.)), CheckingVisitor(indices), cmp);
        BOOST_CHECK(indices.size() == 3);
        BOOST_CHECK(indices[0].first == 20 && indices[0].second == 20);
        BOOST_CHECK(indices[1].first == 21 && indices[1].second == 20);
        BOOST_CHECK(indices[2].first == 22 && indices[2].second == 20);
    }

    //! reversed segment along +x-axis.
    {
        std::vector<std::pair<std::size_t, std::size_t> > indices;
        fast_voxel_grid_traversal(grid, segment<point_double_2d>(point_double_2d(2., 0.), point_double_2d(0., 0.)), CheckingVisitor(indices), cmp);
        BOOST_CHECK(indices.size() == 3);
        BOOST_CHECK(indices[2].first == 20 && indices[2].second == 20);
        BOOST_CHECK(indices[1].first == 21 && indices[1].second == 20);
        BOOST_CHECK(indices[0].first == 22 && indices[0].second == 20);
    }

    //! segment along -x-axis.
    {
        std::vector<std::pair<std::size_t, std::size_t> > indices;
        fast_voxel_grid_traversal(grid, segment<point_double_2d>(point_double_2d(-2., 0.), point_double_2d(0., 0.)), CheckingVisitor(indices), cmp);
        BOOST_CHECK(indices.size() == 3);
        BOOST_CHECK(indices[0].first == 18 && indices[0].second == 20);
        BOOST_CHECK(indices[1].first == 19 && indices[1].second == 20);
        BOOST_CHECK(indices[2].first == 20 && indices[2].second == 20);
    }

    //! segment along +y-axis.
    {
        std::vector<std::pair<std::size_t, std::size_t> > indices;
        fast_voxel_grid_traversal(grid, segment<point_double_2d>(point_double_2d(0., 0.), point_double_2d(0., 2.)), CheckingVisitor(indices), cmp);
        BOOST_CHECK(indices.size() == 3);
        BOOST_CHECK(indices[0].first == 20 && indices[0].second == 20);
        BOOST_CHECK(indices[1].first == 20 && indices[1].second == 21);
        BOOST_CHECK(indices[2].first == 20 && indices[2].second == 22);
    }

    //! reversed segment along +y-axis.
    {
        std::vector<std::pair<std::size_t, std::size_t> > indices;
        fast_voxel_grid_traversal(grid, segment<point_double_2d>(point_double_2d(0., 2.), point_double_2d(0., 0.)), CheckingVisitor(indices), cmp);
        BOOST_CHECK(indices.size() == 3);
        BOOST_CHECK(indices[2].first == 20 && indices[2].second == 20);
        BOOST_CHECK(indices[1].first == 20 && indices[1].second == 21);
        BOOST_CHECK(indices[0].first == 20 && indices[0].second == 22);
    }

    //! segment in range of (45,90) degrees.
    {
        std::vector<std::pair<std::size_t, std::size_t> > indices;
        fast_voxel_grid_traversal(grid, segment<point_double_2d>(point_double_2d(0.4, 0.5), point_double_2d(1.1, 3.)), CheckingVisitor(indices), cmp);
        BOOST_CHECK(indices.size() == 5);
        BOOST_CHECK(indices[0].first == 20 && indices[0].second == 20);
        BOOST_CHECK(indices[1].first == 20 && indices[1].second == 21);
        BOOST_CHECK(indices[2].first == 20 && indices[2].second == 22);
        BOOST_CHECK(indices[3].first == 21 && indices[3].second == 22);
        BOOST_CHECK(indices[4].first == 21 && indices[4].second == 23);
    }

    //! segment in range of (270,315) degrees.
    {
        std::vector<std::pair<std::size_t, std::size_t> > indices;
        fast_voxel_grid_traversal(grid, segment<point_double_2d>(point_double_2d(0.4, -0.5), point_double_2d(1.1, -3.)), CheckingVisitor(indices), cmp);
        BOOST_CHECK(indices.size() == 4);
        BOOST_CHECK(indices[0].first == 20 && indices[0].second == 19);
        BOOST_CHECK(indices[1].first == 20 && indices[1].second == 18);
        BOOST_CHECK(indices[2].first == 20 && indices[2].second == 17);
        BOOST_CHECK(indices[3].first == 21 && indices[3].second == 17);
    }
}

#include <geometrix/numeric/constants.hpp>
template <typename PointSequence, typename Point, int Divisions = 100, typename std::enable_if<geometrix::is_polygon<PointSequence>::value, int>::type = 0>
inline PointSequence make_circle_as_sequence(const Point& center, double r)
{
	using namespace geometrix;
	auto v = vector_double_2d{ r, 0.0 };
    ignore_unused_warning_of(v);
	auto s = constants::two_pi<double>() / Divisions, t = 0.;
	auto poly = PointSequence{};
	for (auto i = 0UL; i < Divisions; ++i, t += s) 
	{
		poly.emplace_back(center + vector_double_2d{ r * cos(t), r * sin(t) });
	}

	return std::move(poly);
}

BOOST_AUTO_TEST_CASE(TestGridFloodfill)
{
	using namespace geometrix;

	double xmin = -20;
	double xmax = 20;
	double ymin = -20;
	double ymax = 20;

	absolute_tolerance_comparison_policy<double> cmp(1e-10);
	grid_traits<double> grid(xmin, xmax, ymin, ymax, 2.0);

	auto pgon = make_circle_as_sequence<polygon<point<double, 2>>>(point<double, 2>{0.0, 0.0}, 9.);
	using polygon2 = polygon<point<double, 2>>;
	std::vector<polygon2> pgons;
	auto visitor = [&](std::uint32_t i, std::uint32_t j)
	{
		pgons.push_back(grid.get_cell_polygon(i, j));
	};

	floodfill_grid_traversal(grid, pgon, visitor, cmp);

	auto hole0 = make_circle_as_sequence<polygon<point<double, 2>>>(point<double, 2>{4.0, 0.0}, 3.);
	auto hole1 = make_circle_as_sequence<polygon<point<double, 2>>>(point<double, 2>{-4.0, 0.0}, 3.);
	//auto hole2 = make_circle_as_sequence<polygon<point<double, 2>>>(point<double, 2>{0.0, 4.0}, 2.);
	//auto hole3 = make_circle_as_sequence<polygon<point<double, 2>>>(point<double, 2>{0.0, -4.0}, 2.);
	auto pwh = polygon_with_holes<point<double, 2>>{ pgon, {hole0, hole1} };
	pgons.clear();
	floodfill_grid_traversal(grid, pwh, visitor, cmp);
}

#endif //GEOMETRIX_GRID_TESTS_HPP
