//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_2D_KERNEL_HPP
#define GEOMETRIX_2D_KERNEL_HPP

#include <geometrix/primitive/point.hpp>
#include <geometrix/primitive/segment.hpp>
#include <geometrix/tensor/vector.hpp>
#include <geometrix/tensor/matrix.hpp>
#include <geometrix/primitive/polygon.hpp>
#include <geometrix/primitive/polyline.hpp>

struct geometry_kernel_2d_fixture
{
	typedef geometrix::point_double_2d point2;
	typedef geometrix::vector_double_2d vector2;
	typedef geometrix::segment<point2> segment2;
	typedef geometrix::polygon<point2> polygon2;
	typedef geometrix::polyline<point2> polyline2;
	typedef geometrix::matrix<double, 2, 2> matrix22;
	typedef geometrix::matrix<double, 3, 3> matrix33;
	typedef geometrix::matrix<double, 4, 4> matrix44;

	geometrix::absolute_tolerance_comparison_policy<double> cmp;
};

#endif //GEOMETRIX_2D_KERNEL_HPP
