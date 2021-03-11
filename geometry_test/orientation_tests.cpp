///////////////////////////////////////////////////////////////////////////////
// orientation_tests.cpp
//
//  Copyright 2021 Brandon Kohn. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "./2d_kernel_fixture.hpp"

#include <geometrix/algorithm/orientation.hpp>

#include <geometrix/utility/utilities.hpp>
#include <geometrix/utility/scope_timer.ipp>

#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/range/algorithm_ext/erase.hpp>

#include <exception>
#include <iostream>

//! Google tests
TEST_F(geometry_kernel_2d_fixture, convex_polygon_polygon_intersection_simple_test_not_intersecting)
{
    using namespace geometrix;
