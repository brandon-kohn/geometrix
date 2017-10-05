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
#include <geometrix/primitive/sphere.hpp>
#include <geometrix/tensor/vector.hpp>
#include <geometrix/tensor/matrix.hpp>
#include <geometrix/primitive/polygon.hpp>
#include <geometrix/primitive/polyline.hpp>
#include <geometrix/primitive/polygon_with_holes.hpp>
#include <ostream>

#include <gtest/gtest.h>

struct geometry_kernel_2d_fixture : ::testing::Test
{
	typedef geometrix::point_double_2d point2;
	typedef geometrix::vector_double_2d vector2;
	typedef geometrix::segment<point2> segment2;
	typedef geometrix::polygon<point2> polygon2;
	typedef geometrix::polyline<point2> polyline2;
	typedef geometrix::polygon_with_holes<point2> polygon_with_holes2;

	using circle2 = geometrix::sphere<2, point2>;

	typedef geometrix::matrix<double, 2, 2> matrix22;
	typedef geometrix::matrix<double, 3, 3> matrix33;
	typedef geometrix::matrix<double, 4, 4> matrix44;

	virtual void TestBody() override {}

	geometrix::absolute_tolerance_comparison_policy<double> cmp;	
};

inline bool operator ==(const geometry_kernel_2d_fixture::point2& s1, const geometry_kernel_2d_fixture::point2& s2)
{
	using namespace geometrix;
	absolute_tolerance_comparison_policy<double> cmp;
	return numeric_sequence_equals(s1, s2, cmp);
}

inline bool operator !=(const geometry_kernel_2d_fixture::point2& s1, const geometry_kernel_2d_fixture::point2& s2)
{
	using namespace geometrix;
	absolute_tolerance_comparison_policy<double> cmp;
	return !numeric_sequence_equals(s1, s2, cmp);
}

inline bool operator ==(const geometry_kernel_2d_fixture::segment2& s1, const geometry_kernel_2d_fixture::segment2& s2)
{
	return s1.get_start() == s2.get_start() && s1.get_end() == s2.get_end();
}

inline bool operator !=(const geometry_kernel_2d_fixture::segment2& s1, const geometry_kernel_2d_fixture::segment2& s2)
{
	return s1.get_start() != s2.get_start() || s1.get_end() != s2.get_end();
}

#define GEOMETRIX_TEST_COLLECTIONS_EQUAL(A, B) \
{                                              \
    BOOST_CHECK_EQUAL(A.size(), B.size());     \
	auto ai = std::begin(A), ae = std::end(A); \
	auto bi = std::begin(B), be = std::end(B); \
	while( ai != ae && bi != be )              \
	{                                          \
		BOOST_CHECK(*ai++ == *bi++);           \
	}                                          \
}                                              \
/***/

namespace geometrix
{
	inline std::ostream& operator << (std::ostream& os, const geometry_kernel_2d_fixture::point2& p)
	{
		using namespace geometrix;
		os.precision(std::numeric_limits<double>::max_digits10);
		os << "point2{" << get<0>(p) << ", " << get<1>(p) << "}";
		return os;
	}

	inline std::ostream& operator << (std::ostream& os, const geometry_kernel_2d_fixture::vector2& p)
	{
		using namespace geometrix;
		os.precision(std::numeric_limits<double>::max_digits10);
		os << "vector2{" << get<0>(p) << ", " << get<1>(p) << "}";
		return os;
	}

	inline std::ostream& operator << (std::ostream& os, const geometry_kernel_2d_fixture::segment2& s)
	{
		using namespace geometrix;
		os.precision(std::numeric_limits<double>::max_digits10);
		os << "segment2{" << get<0>(s.get_start()) << ", " << get<1>(s.get_start()) << ", " << get<0>(s.get_end()) << ", " << get<1>(s.get_end()) << "}";
		return os;
	}

	inline std::ostream& operator << (std::ostream& os, const geometry_kernel_2d_fixture::polygon2& p)
	{
		using namespace geometrix;
		os.precision(std::numeric_limits<double>::max_digits10);
		os << "polygon2{ ";
		for (std::size_t i = 0; i < p.size(); ++i) {
			if (i)
				os << ", ";
			os << p[i];
		}
		os << " }";
		return os;
	}
	
	inline std::ostream& operator << (std::ostream& os, const geometry_kernel_2d_fixture::polyline2& p)
	{
		using namespace geometrix;
		os.precision(std::numeric_limits<double>::max_digits10);
		os << "polyline2{ ";
		for (std::size_t i = 0; i < p.size(); ++i) {
			if (i)
				os << ", ";
			os << p[i];
		}
		os << " }";
		return os;
	}
}//! namespace geometrix;

#endif //GEOMETRIX_2D_KERNEL_HPP
