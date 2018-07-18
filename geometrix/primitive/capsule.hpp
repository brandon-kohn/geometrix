//
//! Copyright © 2018
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_CAPSULE_HPP
#define GEOMETRIX_CAPSULE_HPP

#include <geometrix/primitive/segment_traits.hpp>
#include <geometrix/utility/construction_policy.hpp>
#include <geometrix/algebra/expression.hpp>

namespace geometrix {

//! \class capsule 
//! \brief A class for specifying a capsule, c, as a segment with some radius, r.
template <typename Point>
class capsule
{
    BOOST_CLASS_REQUIRE( Point, geometrix, PointConcept );

public:

    typedef Point                                                  point_type;
    typedef segment<Point>                                         segment_type;
    typedef typename dimension_of<point_type>::type                dimension_type;
    typedef typename geometric_traits<point_type>::arithmetic_type length_type;

    capsule()
    {}

    template <typename Point1, typename Point2, typename Length>
    capsule(const Point1& a, const Point2& b, const Length& r)
        : m_segment(make_segment(a, b))
        , m_r(r)
    {}

    template <typename Segment, typename Length>
    capsule(const Segment& segment, const Length& r)
        : m_segment(segment)
        , m_r(r)
    {}

    const segment_type&  get_segment() const { return m_segment; }
    const length_type&   get_radius() const { return m_r; }

private:

    segment_type m_segment;
    length_type  m_r;

};

template <typename Point, typename Vector>
struct is_capsule<capsule<Point>> : boost::true_type{};
template <typename Point>
struct geometric_traits<capsule<Point>>
{
    using hyperplane_dimension = dimension<2>;
    
    typedef Point                                                  point_type;
    typedef segment<Point>                                         segment_type;
    typedef capsule<Point>                                         capsule_type;
    typedef typename geometric_traits<point_type>::arithmetic_type length_type;
    typedef typename dimension_of<Point>::type                     dimension_type;
};

//! Specialize the coordinate accessors
template <typename Point>
struct capsule_access_traits<capsule<Point>>
{
    typedef Point  point_type;
    typedef segment<Point> segment_type;
    typedef typename geometric_traits<point_type>::arithmetic_type length_type;
    typedef typename capsule<Point>::dimension_type dimension_type;

    static const segment<point_type>& get_segment( const capsule<Point>& c ){ return c.get_segment(); }
    static const length_type& get_radius( const capsule<Point>& c ){ return c.get_radius(); }
};

template <typename Point>
struct construction_policy<capsule<Point>>
{
    template <typename Point1, typename Point2, typename Length>
    static capsule<Point> construct(const Point1& a, const Point2& b, const Length& r)
    {
        return capsule<Point>(a, b, r);
    }

    template <typename Segment, typename Length>
    static capsule<Point> construct(const Segment& s, const Length& r)
    {
        return capsule<Point>(s, r);
    }
};

template <typename Point1, typename Point2, typename Length>
inline capsule<Point> make_capsule( const Point1& a, const Point2& b, const Length& r )
{
    return capsule<Point>(a, b, r);
}

template <typename Segment, typename Length>
inline capsule<Point> make_capsule( const Segment& s, const Length& r )
{
    return capsule<Point>(s, r);
}

}//namespace geometrix;

#endif //GEOMETRIX_LINE_HPP
