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

struct geometry_kernel_2d_units_fixture : ::testing::Test
{
    using dimensionless_t = boost::units::quantity<boost::units::si::dimensionless, double>;
    using length_t = boost::units::quantity<boost::units::si::length, double>;
    using area_t = decltype(length_t()*length_t());
    using volume_t = decltype(area_t()*length_t());
    using area2_t = decltype(area_t()*area_t());
    using angle_t = boost::units::quantity<boost::units::si::plane_angle, double>;
    using time_t = boost::units::quantity<boost::units::si::time, double>;
    using speed_t = boost::units::quantity<boost::units::si::velocity, double>;
    using kinematic_viscosity_t = boost::units::quantity<boost::units::si::kinematic_viscosity, double>;//! m^2/s ... some motion calcs have this term.

    using vector2 = geometrix::vector<length_t, 2>;
    using dimensionless_vector2 = geometrix::vector<dimensionless_t, 2>;
    using dimensionless2 = dimensionless_vector2;
    using velocity2 = geometrix::vector<speed_t, 2>;

    using point2 = geometrix::point<length_t, 2>;
    using segment2 = geometrix::segment<point2>;
    using polygon2 = geometrix::polygon<point2>;
    using polyline2 = geometrix::polyline<point2>;
    using circle2 = geometrix::sphere<2, point2>;

    using aabb2 = geometrix::axis_aligned_bounding_box<point2>;
    using obb2 = geometrix::oriented_bounding_box<point2, dimensionless_vector2>;

    using matrix2x2 = geometrix::matrix<double, 2, 2>;
    using matrix3x3 = geometrix::matrix<double, 3, 3>;
    using matrix4x4 = geometrix::matrix<double, 4, 4>;

    using comparison_policy = geometrix::compound_comparison_policy
        <
          geometrix::absolute_tolerance_comparison_policy<double>
        , geometrix::absolute_tolerance_comparison_policy<dimensionless_t>
        , geometrix::absolute_tolerance_comparison_policy<length_t>
        , geometrix::absolute_tolerance_comparison_policy<area_t>
        , geometrix::absolute_tolerance_comparison_policy<area2_t>
        , geometrix::absolute_tolerance_comparison_policy<volume_t>
        , geometrix::absolute_tolerance_comparison_policy<angle_t>
        , geometrix::absolute_tolerance_comparison_policy<time_t>
        , geometrix::absolute_tolerance_comparison_policy<speed_t>
        , geometrix::absolute_tolerance_comparison_policy<kinematic_viscosity_t>
        >;

    geometry_kernel_2d_units_fixture()
        : cmp
        (
              geometrix::absolute_tolerance_comparison_policy<double>()
            , geometrix::absolute_tolerance_comparison_policy<dimensionless_t>()
            , geometrix::absolute_tolerance_comparison_policy<length_t>()
            , geometrix::absolute_tolerance_comparison_policy<area_t>()
            , geometrix::absolute_tolerance_comparison_policy<area2_t>()
            , geometrix::absolute_tolerance_comparison_policy<volume_t>()
            , geometrix::absolute_tolerance_comparison_policy<angle_t>(angle_t(1e-6 * boost::units::si::radians))
            , geometrix::absolute_tolerance_comparison_policy<time_t>()
            , geometrix::absolute_tolerance_comparison_policy<speed_t>()
            , geometrix::absolute_tolerance_comparison_policy<kinematic_viscosity_t>()
        )
    {}

    comparison_policy cmp;
};

GEOMETRIX_DEFINE_POINT_TRAITS(geometry_kernel_2d_units_fixture::point2, (geometry_kernel_2d_units_fixture::length_t), 2, geometry_kernel_2d_units_fixture::dimensionless_t, geometry_kernel_2d_units_fixture::length_t, neutral_reference_frame_2d, index_operator_vector_access_policy<geometry_kernel_2d_units_fixture::point2>);
GEOMETRIX_DEFINE_VECTOR_TRAITS(geometry_kernel_2d_units_fixture::dimensionless_vector2, (geometry_kernel_2d_units_fixture::dimensionless_t), 2, geometry_kernel_2d_units_fixture::dimensionless_t, geometry_kernel_2d_units_fixture::dimensionless_t, neutral_reference_frame_2d, index_operator_vector_access_policy<geometry_kernel_2d_units_fixture::dimensionless_vector2>);
GEOMETRIX_DEFINE_VECTOR_TRAITS(geometry_kernel_2d_units_fixture::velocity2, (geometry_kernel_2d_units_fixture::speed_t), 2, geometry_kernel_2d_units_fixture::dimensionless_t, geometry_kernel_2d_units_fixture::speed_t, neutral_reference_frame_2d, index_operator_vector_access_policy<geometry_kernel_2d_units_fixture::velocity2>);

GEOMETRIX_DEFINE_SPHERE_TRAITS(2, geometry_kernel_2d_units_fixture::point2, geometry_kernel_2d_units_fixture::circle2);
GEOMETRIX_DEFINE_SPHERE_ACCESS_TRAITS(geometry_kernel_2d_units_fixture::circle2);

inline bool operator ==(const geometry_kernel_2d_units_fixture::point2& s1, const geometry_kernel_2d_units_fixture::point2& s2)
{
    using namespace geometrix;
    absolute_tolerance_comparison_policy<geometry_kernel_2d_units_fixture::length_t> cmp;
    return numeric_sequence_equals(s1, s2, cmp);
}

inline bool operator !=(const geometry_kernel_2d_units_fixture::point2& s1, const geometry_kernel_2d_units_fixture::point2& s2)
{
    using namespace geometrix;
    absolute_tolerance_comparison_policy<geometry_kernel_2d_units_fixture::length_t> cmp;
    return !numeric_sequence_equals(s1, s2, cmp);
}

inline bool operator ==(const geometry_kernel_2d_units_fixture::segment2& s1, const geometry_kernel_2d_units_fixture::segment2& s2)
{
    return s1.get_start() == s2.get_start() && s1.get_end() == s2.get_end();
}

inline bool operator !=(const geometry_kernel_2d_units_fixture::segment2& s1, const geometry_kernel_2d_units_fixture::segment2& s2)
{
    return s1.get_start() != s2.get_start() || s1.get_end() != s2.get_end();
}

namespace geometrix
{
    inline std::ostream& operator << (std::ostream& os, const geometry_kernel_2d_units_fixture::point2& p)
    {
        using namespace geometrix;
        os.precision(std::numeric_limits<double>::max_digits10);
        os << "point2{" << get<0>(p) << ", " << get<1>(p) << "}";
        return os;
    }

    inline std::ostream& operator << (std::ostream& os, const geometry_kernel_2d_units_fixture::vector2& p)
    {
        using namespace geometrix;
        os.precision(std::numeric_limits<double>::max_digits10);
        os << "vector2{" << get<0>(p) << ", " << get<1>(p) << "}";
        return os;
    }

    inline std::ostream& operator << (std::ostream& os, const geometry_kernel_2d_units_fixture::segment2& s)
    {
        using namespace geometrix;
        os.precision(std::numeric_limits<double>::max_digits10);
        os << "segment2{" << get<0>(s.get_start()) << ", " << get<1>(s.get_start()) << ", " << get<0>(s.get_end()) << ", " << get<1>(s.get_end()) << "}";
        return os;
    }

    inline std::ostream& operator << (std::ostream& os, const geometry_kernel_2d_units_fixture::polygon2& p)
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

    inline std::ostream& operator << (std::ostream& os, const geometry_kernel_2d_units_fixture::polyline2& p)
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
}//! namespace geometrix;

#endif //GEOMETRIX_2D_KERNEL_UNITS_FIXTURE_HPP
