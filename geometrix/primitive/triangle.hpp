//
//! Copyright © 2018
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_TRIANGLE_HPP
#define GEOMETRIX_TRIANGLE_HPP

#include <geometrix/primitive/array_point_sequence.hpp>
#include <geometrix/primitive/point_traits.hpp>
#include <geometrix/utility/construction_policy.hpp>
#include <array>

namespace geometrix {

//! \class triangle
//! \brief A class for specifying a triangle.
template <typename Point>
class triangle : public std::array<Point, 3>
{
    BOOST_CLASS_REQUIRE( Point, geometrix, PointConcept );
    typedef std::array<Point, 3> array_type;

public:

    BOOST_CONSTEXPR triangle(const std::array<Point, 3>& a)
        : std::array<Point, 3>(a)
    {}

    template <typename Point1, typename Point2, typename Point3, typename Point4>
    BOOST_CONSTEXPR triangle(const Point1& p1, const Point2& p2, const Point3& p3, const Point4& p4)
        : triangle(array_type{ {construct<Point>(p1), construct<Point>(p2), construct<Point>(p3), construct<Point>(p4)} })
    {}

    typedef Point                                                  point_type;
    typedef typename dimension_of< point_type >::type              dimension_type;
    typedef typename geometric_traits<point_type>::arithmetic_type arithmetic_type;

    static_assert(sizeof(triange<Point>) == 3 * sizeof(Point), "Triangle should be the size of 3 points.");
};

template <typename Point>
struct point_sequence_traits<triangle<Point>>
    : point_sequence_traits<std::array<Point, 3>>
{};

template <typename Point>
struct geometric_traits<triangle<Point>>
{
    typedef void                                    is_point_sequence;
    typedef void                                    is_triangle;
    typedef Point                                   point_type;
    typedef triangle<Point>                         triangle_type;
    typedef typename dimension_of<point_type>::type dimension_type;
};

template <typename Point>
struct construction_policy<triangle<Point>>
{
    template <typename ...Args>
    static triangle< Point > construct(Args... args)
    {
        return std::array<Point, 3>{ args... };
    }
};

}//namespace geometrix;

#endif //GEOMETRIX_TRIANGLE_HPP
