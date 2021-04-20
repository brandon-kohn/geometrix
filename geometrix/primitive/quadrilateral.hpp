//
//! Copyright © 2021
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_QUADRILATERAL_HPP
#define GEOMETRIX_QUADRILATERAL_HPP
#pragma once

#include <geometrix/primitive/vector_point_sequence.hpp>
#include <geometrix/primitive/point_traits.hpp>
#include <geometrix/utility/construction_policy.hpp>
#include <array>

namespace geometrix {

//! \class quadrilateral
//! \brief A class for specifying a quadrilateral.
template <typename Point>
class quadrilateral : public std::array<Point, 4>
{
    BOOST_CLASS_REQUIRE( Point, geometrix, PointConcept );
    typedef std::array<Point, 4> array_type;

public:

    quadrilateral() = default;

    quadrilateral(const std::array<Point, 4>& a)
        : std::array<Point, 4>(a)
    {}

    template <typename Point1, typename Point2, typename Point3, typename Point4>
    quadrilateral(const Point1& p1, const Point2& p2, const Point3& p3, const Point4& p4)
        : quadrilateral(array_type{ {construct<Point>(p1), construct<Point>(p2), construct<Point>(p3), construct<Point>(p4)} })
    {}

    typedef Point                                                  point_type;
    typedef typename dimension_of< point_type >::type              dimension_type;
    typedef typename geometric_traits<point_type>::arithmetic_type arithmetic_type;
};

template <typename Point>
using quad = quadrilateral<Point>;

template <typename Point>
struct point_sequence_traits<quadrilateral<Point>>
    : point_sequence_traits<std::array<Point, 4>>
{};

template <typename Point>
struct geometric_traits< quadrilateral<Point> >
{
    typedef void                                      is_point_sequence;
    typedef void                                      is_quadrilateral;
    typedef Point                                     point_type;
    typedef quadrilateral<Point>                      quadrilateral_type;
    typedef typename dimension_of< point_type >::type dimension_type;
};

template <typename Point>
struct construction_policy< quadrilateral< Point > >
{
    template <typename ...Args>
    static quadrilateral< Point > construct(Args... args)
    {
        return std::array< Point, 4 >{ args... };
    }
};

}//namespace geometrix;

#endif //GEOMETRIX_QUADRILATERAL_HPP
