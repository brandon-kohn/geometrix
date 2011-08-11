// Generic Geometry Library test file
//
// Copyright Barend Gehrels, 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef GGL_TEST_UTIL_SVG_MAPPER_HPP
#define GGL_TEST_UTIL_SVG_MAPPER_HPP

#include <cstdio>

#include <ggl/ggl.hpp>
#include <ggl/algorithms/transform.hpp>
#include <ggl/geometries/box.hpp>
#include <ggl/geometries/linestring.hpp>
#include <ggl/geometries/linear_ring.hpp>
#include <ggl/geometries/polygon.hpp>

#include <ggl/multi/core/tags.hpp>

#include <ggl/extensions/io/svg/write_svg.hpp>


#ifndef DOXYGEN_NO_DISPATCH
namespace dispatch
{

template <typename GeometryTag, bool IsMulti, typename Geometry>
struct svg_map
{
};


template <typename Point>
struct svg_map<ggl::point_tag, false, Point>
{
    template <typename TransformStrategy>
    static inline void apply(std::ostream& stream,
                    std::string const& style, int size,
                    Point const& point, TransformStrategy const& strategy)
    {
        ggl::point_xy<int> p;
        ggl::transform(point, p, strategy);
        stream << ggl::svg(p, style, size) << std::endl;
    }
};

template <typename Box>
struct svg_map<ggl::box_tag, false, Box>
{
    template <typename TransformStrategy>
    static inline void apply(std::ostream& stream,
                    std::string const& style, int size,
                    Box const& box, TransformStrategy const& strategy)
    {

        typename ggl::point_type<Box>::type p1, p2;
        ggl::set<0>(p1, ggl::get<ggl::min_corner, 0>(box));
        ggl::set<1>(p1, ggl::get<ggl::min_corner, 1>(box));
        ggl::set<0>(p2, ggl::get<ggl::max_corner, 0>(box));
        ggl::set<1>(p2, ggl::get<ggl::max_corner, 1>(box));

        ggl::box<ggl::point_xy<int> > ibox;
        ggl::transform(box, ibox, strategy);

        stream << ggl::svg(ibox, style, size) << std::endl;
    }
};


template <typename Range1, typename Range2>
struct svg_map_range
{
    template <typename TransformStrategy>
    static inline void apply(std::ostream& stream,
                std::string const& style, int size,
                Range1 const& range, TransformStrategy const& strategy)
    {
        Range2 irange;
        ggl::transform(range, irange, strategy);
        stream << ggl::svg(irange, style, size) << std::endl;
    }
};



template <typename Ring>
struct svg_map<ggl::ring_tag, false, Ring>
    : svg_map_range<Ring, ggl::linear_ring<ggl::point_xy<int> > >
{};

template <typename Linestring>
struct svg_map<ggl::linestring_tag, false, Linestring>
    : svg_map_range<Linestring, ggl::linestring<ggl::point_xy<int> > >
{};


template <typename Polygon>
struct svg_map<ggl::polygon_tag, false, Polygon>
{
    template <typename TransformStrategy>
    static inline void apply(std::ostream& stream,
                    std::string const& style, int size,
                    Polygon const& polygon, TransformStrategy const& strategy)
    {
        ggl::polygon<ggl::point_xy<int> > ipoly;
        ggl::transform(polygon, ipoly, strategy);
        stream << ggl::svg(ipoly, style, size) << std::endl;
    }
};

template <typename Tag, typename Multi>
struct svg_map<Tag, true, Multi>
{
    template <typename TransformStrategy>
    static inline void apply(std::ostream& stream,
                    std::string const& style, int size,
                    Multi const& multi, TransformStrategy const& strategy)
    {
        for (typename boost::range_const_iterator<Multi>::type it
            = boost::begin(multi);
            it != boost::end(multi);
            ++it)
        {
            svg_map
                <
                    typename ggl::single_tag<Tag>::type,
                    false,
                    typename boost::range_value<Multi>::type
                >::apply(stream, style, size, *it, strategy);
        }
    }
};



} // namespace dispatch
#endif



template <typename Geometry, typename TransformStrategy>
inline void svg_map(std::ostream& stream,
            std::string const& style, int size,
            Geometry const& geometry, TransformStrategy const& strategy)
{
    dispatch::svg_map
        <
            typename ggl::tag<Geometry>::type,
            ggl::is_multi<Geometry>::type::value,
            typename boost::remove_const<Geometry>::type
        >::apply(stream, style, size, geometry, strategy);
}


template <typename P>
struct svg_mapper
{
    typedef ggl::strategy::transform::map_transformer<P, ggl::point_xy<int>, true, true> transformer_type;
    ggl::box<P> bbox;
    transformer_type* matrix;
    std::ostream& stream;
    int width, height;

    svg_mapper(std::ostream& s, int w, int h)
        : matrix(NULL)
        , stream(s)
        , width(w)
        , height(h)
    {
        ggl::assign_inverse(bbox);
    }

    virtual ~svg_mapper()
    {
        stream << "</svg>" << std::endl;
        if (matrix) delete matrix;
    }

    template <typename Geometry>
    void add(Geometry const& geometry)
    {
        ggl::combine(bbox, ggl::make_envelope<ggl::box<P> >(geometry));
    }


    template <typename Geometry>
    void map(Geometry const& geometry, std::string const& style, int size = -1)
    {
        if (! matrix)
        {
            matrix = new transformer_type(bbox, width, height);

            stream << "<?xml version=\"1.0\" standalone=\"no\"?>" << std::endl;
            stream << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\"" << std::endl;
            stream << "\"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">" << std::endl;

            stream << "<svg width=\"100%\" height=\"100%\" version=\"1.1\"" << std::endl;
            stream << "xmlns=\"http://www.w3.org/2000/svg\">" << std::endl;
        }
        svg_map(stream, style, size, geometry, *matrix);
    }


};


#endif // GGL_TEST_UTIL_SVG_MAPPER_HPP
