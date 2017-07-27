//
//! Copyright © 2017
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_HOMOGENEOUS_COORDINATES_TESTS_HPP
#define GEOMETRIX_HOMOGENEOUS_COORDINATES_TESTS_HPP

#include <boost/test/included/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/units/physical_dimensions.hpp>
#include <boost/units/dimensionless_quantity.hpp>
#include <boost/units/io.hpp>
#include <boost/fusion/include/adapt_adt.hpp>

#include <geometrix/tensor/vector_traits.hpp>
#include <geometrix/tensor/homogeneous_vector_adaptor.hpp>
#include <geometrix/primitive/point.hpp>
#include <geometrix/tensor/vector.hpp>
#include <geometrix/utility/utilities.hpp>
#include "2d_kernel_units_fixture.hpp"

using namespace geometrix;

BOOST_FIXTURE_TEST_CASE(UnitedVectorHomogeneousCoordinateAdaptorTest, geometry_kernel_2d_units_fixture)
{
    using namespace boost::units::si;
    auto v = vector2{ 1.0 * meters, 1.0 * meters};
    
    auto hv = as_positional_homogeneous(hv);
}

#endif //GEOMETRIX_HOMOGENEOUS_COORDINATES_TESTS_HPP
