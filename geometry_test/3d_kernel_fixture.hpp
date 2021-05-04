//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_3D_KERNEL_HPP
#define GEOMETRIX_3D_KERNEL_HPP

#include <geometrix/primitive/point.hpp>
#include <geometrix/primitive/segment.hpp>
#include <geometrix/primitive/capsule.hpp>
#include <geometrix/primitive/sphere.hpp>
#include <geometrix/tensor/vector.hpp>
#include <geometrix/tensor/matrix.hpp>
#include <geometrix/primitive/polygon.hpp>
#include <geometrix/primitive/rectangle.hpp>
#include <geometrix/primitive/quadrilateral.hpp>
#include <geometrix/primitive/polyline.hpp>
#include <geometrix/primitive/polygon_with_holes.hpp>
#include <geometrix/primitive/axis_aligned_bounding_box.hpp>
#include <geometrix/primitive/oriented_bounding_box.hpp>
#include <ostream>

#include <gtest/gtest.h>

struct geometry_kernel_3d_fixture : ::testing::Test
{
	typedef geometrix::point_double_3d point3;
	typedef geometrix::vector_double_3d vector3;
	typedef geometrix::vector_double_3d dimensionless3;
	typedef geometrix::segment<point3> segment3;
	typedef geometrix::capsule<point3> capsule3;
	typedef geometrix::polygon<point3> polygon3;
	typedef geometrix::rectangle<point3> rectangle3;
	typedef geometrix::quad<point3> quad3;
	typedef geometrix::polyline<point3> polyline3;
	typedef geometrix::polygon_with_holes<point3> polygon_with_holes3;
	
	virtual void TestBody() override {}

	geometrix::absolute_tolerance_comparison_policy<double> cmp;	
};

inline bool operator ==(const geometry_kernel_3d_fixture::point3& s1, const geometry_kernel_3d_fixture::point3& s2)
{
	using namespace geometrix;
	absolute_tolerance_comparison_policy<double> cmp;
	return numeric_sequence_equals(s1, s2, cmp);
}

inline bool operator !=(const geometry_kernel_3d_fixture::point3& s1, const geometry_kernel_3d_fixture::point3& s2)
{
	using namespace geometrix;
	absolute_tolerance_comparison_policy<double> cmp;
	return !numeric_sequence_equals(s1, s2, cmp);
}

inline bool operator ==(const geometry_kernel_3d_fixture::segment3& s1, const geometry_kernel_3d_fixture::segment3& s2)
{
	return s1.get_start() == s2.get_start() && s1.get_end() == s2.get_end();
}

inline bool operator !=(const geometry_kernel_3d_fixture::segment3& s1, const geometry_kernel_3d_fixture::segment3& s2)
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
	inline std::ostream& operator << (std::ostream& os, const geometry_kernel_3d_fixture::point3& p)
	{
		using namespace geometrix;
		os.precision(std::numeric_limits<double>::max_digits10);
		os << "point3{" << get<0>(p) << ", " << get<1>(p) << "}";
		return os;
	}

	inline std::ostream& operator << (std::ostream& os, const geometry_kernel_3d_fixture::vector3& p)
	{
		using namespace geometrix;
		os.precision(std::numeric_limits<double>::max_digits10);
		os << "vector3{" << get<0>(p) << ", " << get<1>(p) << "}";
		return os;
	}

	inline std::ostream& operator << (std::ostream& os, const geometry_kernel_3d_fixture::segment3& s)
	{
		using namespace geometrix;
		os.precision(std::numeric_limits<double>::max_digits10);
		os << "segment3{" << get<0>(s.get_start()) << ", " << get<1>(s.get_start()) << ", " << get<0>(s.get_end()) << ", " << get<1>(s.get_end()) << "}";
		return os;
	}

	inline std::ostream& operator << (std::ostream& os, const geometry_kernel_3d_fixture::polygon3& p)
	{
		using namespace geometrix;
		os.precision(std::numeric_limits<double>::max_digits10);
		os << "polygon3{ ";
		for (std::size_t i = 0; i < p.size(); ++i) {
			if (i)
				os << ", ";
			os << p[i];
		}
		os << " }";
		return os;
	}
	
	inline std::ostream& operator << (std::ostream& os, const geometry_kernel_3d_fixture::polyline3& p)
	{
		using namespace geometrix;
		os.precision(std::numeric_limits<double>::max_digits10);
		os << "polyline3{ ";
		for (std::size_t i = 0; i < p.size(); ++i) {
			if (i)
				os << ", ";
			os << p[i];
		}
		os << " }";
		return os;
	}
}//! namespace geometrix;

#endif //GEOMETRIX_3D_KERNEL_HPP
