//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_2D_KERNEL_UNITS_FIXTURE_HPP
#define GEOMETRIX_2D_KERNEL_UNITS_FIXTURE_HPP

#include <geometrix/primitive/point.hpp>
#include <geometrix/primitive/segment.hpp>
#include <geometrix/primitive/capsule.hpp>
#include <geometrix/primitive/sphere.hpp>
#include <geometrix/tensor/vector.hpp>
#include <geometrix/tensor/matrix.hpp>
#include <geometrix/primitive/polygon.hpp>
#include <geometrix/primitive/polyline.hpp>
#include <geometrix/primitive/polygon_with_holes.hpp>

#include <geometrix/primitive/axis_aligned_bounding_box.hpp>
#include <geometrix/primitive/oriented_bounding_box.hpp>

#include <boost/units/physical_dimensions.hpp>
#include <boost/units/dimensionless_quantity.hpp>
#include <boost/units/io.hpp>
#include <boost/units/pow.hpp>
#include <boost/units/limits.hpp>
#include <geometrix/arithmetic/boost_units_arithmetic.hpp>

#include <ostream>

#include <gtest/gtest.h>

struct geometry_kernel_3d_units_fixture : ::testing::Test
{
    using dimensionless_t = boost::units::quantity<boost::units::si::dimensionless, double>;
    using length_t = boost::units::quantity<boost::units::si::length, double>;
    using area_t = decltype(length_t()*length_t());
    using volume_t = decltype(area_t()*length_t());
    using area3_t = decltype(area_t()*area_t());
    using angle_t = boost::units::quantity<boost::units::si::plane_angle, double>;
    using time_t = boost::units::quantity<boost::units::si::time, double>;
    using speed_t = boost::units::quantity<boost::units::si::velocity, double>;
    using kinematic_viscosity_t = boost::units::quantity<boost::units::si::kinematic_viscosity, double>;//! m^2/s ... some motion calcs have this term.

    using vector3 = geometrix::vector<length_t, 3>;
    using dimensionless_vector3 = geometrix::vector<dimensionless_t, 3>;
    using dimensionless3 = dimensionless_vector3;
    using velocity3 = geometrix::vector<speed_t, 3>;

    using point3 = geometrix::point<length_t, 3>;
    using segment3 = geometrix::segment<point3>;
    using capsule3 = geometrix::capsule<point3>;
    using polygon3 = geometrix::polygon<point3>;
    using polyline3 = geometrix::polyline<point3>;

	using comparison_policy = geometrix::mapped_tolerance_comparison_policy<geometrix::absolute_tolerance_comparison_policy<double>, boost::fusion::pair<angle_t, geometrix::relative_tolerance_comparison_policy<double>>>;
	inline comparison_policy make_tolerance_policy(double generalTol = 1e-10, double angleTol = 1e-6)
	{
		using namespace geometrix;
		return comparison_policy{ absolute_tolerance_comparison_policy<double>(generalTol), boost::fusion::make_pair<angle_t>(relative_tolerance_comparison_policy<double>(angleTol)) };
	}

    geometry_kernel_3d_units_fixture()
        : cmp(make_tolerance_policy())
    {}

	virtual void TestBody() override {}

    comparison_policy cmp;
};

GEOMETRIX_DEFINE_POINT_TRAITS(geometry_kernel_3d_units_fixture::point3, (geometry_kernel_3d_units_fixture::length_t), 3, geometry_kernel_3d_units_fixture::dimensionless_t, geometry_kernel_3d_units_fixture::length_t, neutral_reference_frame_3d, index_operator_vector_access_policy<geometry_kernel_3d_units_fixture::point3>);
GEOMETRIX_DEFINE_VECTOR_TRAITS(geometry_kernel_3d_units_fixture::dimensionless_vector3, (geometry_kernel_3d_units_fixture::dimensionless_t), 3, geometry_kernel_3d_units_fixture::dimensionless_t, geometry_kernel_3d_units_fixture::dimensionless_t, neutral_reference_frame_3d, index_operator_vector_access_policy<geometry_kernel_3d_units_fixture::dimensionless_vector3>);
GEOMETRIX_DEFINE_VECTOR_TRAITS(geometry_kernel_3d_units_fixture::velocity3, (geometry_kernel_3d_units_fixture::speed_t), 3, geometry_kernel_3d_units_fixture::dimensionless_t, geometry_kernel_3d_units_fixture::speed_t, neutral_reference_frame_3d, index_operator_vector_access_policy<geometry_kernel_3d_units_fixture::velocity3>);

inline bool operator ==(const geometry_kernel_3d_units_fixture::point3& s1, const geometry_kernel_3d_units_fixture::point3& s3)
{
    using namespace geometrix;
    absolute_tolerance_comparison_policy<geometry_kernel_3d_units_fixture::length_t> cmp;
    return numeric_sequence_equals(s1, s3, cmp);
}

inline bool operator !=(const geometry_kernel_3d_units_fixture::point3& s1, const geometry_kernel_3d_units_fixture::point3& s2)
{
    using namespace geometrix;
    absolute_tolerance_comparison_policy<geometry_kernel_3d_units_fixture::length_t> cmp;
    return !numeric_sequence_equals(s1, s2, cmp);
}

inline bool operator ==(const geometry_kernel_3d_units_fixture::segment3& s1, const geometry_kernel_3d_units_fixture::segment3& s2)
{
    return s1.get_start() == s2.get_start() && s1.get_end() == s2.get_end();
}

inline bool operator !=(const geometry_kernel_3d_units_fixture::segment3& s1, const geometry_kernel_3d_units_fixture::segment3& s2)
{
    return s1.get_start() != s2.get_start() || s1.get_end() != s2.get_end();
}

namespace geometrix
{
    inline std::ostream& operator << (std::ostream& os, const geometry_kernel_3d_units_fixture::point3& p)
    {
        using namespace geometrix;
        os.precision(std::numeric_limits<double>::max_digits10);
        os << "point3{" << get<0>(p) << ", " << get<1>(p) << "}";
        return os;
    }

    inline std::ostream& operator << (std::ostream& os, const geometry_kernel_3d_units_fixture::vector3& p)
    {
        using namespace geometrix;
        os.precision(std::numeric_limits<double>::max_digits10);
        os << "vector3{" << get<0>(p) << ", " << get<1>(p) << "}";
        return os;
    }

    inline std::ostream& operator << (std::ostream& os, const geometry_kernel_3d_units_fixture::segment3& s)
    {
        using namespace geometrix;
        os.precision(std::numeric_limits<double>::max_digits10);
        os << "segment3{" << get<0>(s.get_start()) << ", " << get<1>(s.get_start()) << ", " << get<0>(s.get_end()) << ", " << get<1>(s.get_end()) << "}";
        return os;
    }

    inline std::ostream& operator << (std::ostream& os, const geometry_kernel_3d_units_fixture::polygon3& p)
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

    inline std::ostream& operator << (std::ostream& os, const geometry_kernel_3d_units_fixture::polyline3& p)
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
}//! namespace geometrix;

#endif //GEOMETRIX_2D_KERNEL_UNITS_FIXTURE_HPP
