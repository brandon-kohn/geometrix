//
//! Copyright © 2019
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include <geometrix/primitive/point_sequence_traits.hpp>
#include <geometrix/utility/utilities.hpp>
#include <geometrix/primitive/point.hpp>
#include <geometrix/tensor/vector.hpp>
#include <geometrix/algorithm/distance/point_point_distance.hpp>
#include <boost/optional.hpp>

#include <algorithm>

namespace geometrix {

    namespace result_of
    {
        template <typename Polyline, typename Length>
        struct polyline_point_at_position
        {
        private:

            using access = point_sequence_traits<Polyline>;
            using point_t = typename access::point_type;
            using length_t = typename geometric_traits<point_t>::arithmetic_type;
            using point_type = point<length_t, dimension_of<point_t>::value>;

        public:

            using point_type = point<length_t, dimension_of<point_t>::value>;
            //! Tuple of split index and fraction of segment from split index if any.
            using type = std::tuple<std::size_t, boost::optional<point_type>>;

        };

    }//! namespace result_of;

    template <typename Polyline, typename Length, typename Visitor>
    inline typename result_of::polyline_point_at_position<Polyline, Length>::type polyline_point_at_position(const Polyline& poly, const Length& pos)
    {
        using access = point_sequence_traits<Polyline>;
        using point_t = typename access::point_type;
        using length_t = typename geometric_traits<point_t>::arithmetic_type;
        using result_t = typename result_of::point_at_position<Polyline, Length>::type;
        result_t result;
        auto length = construct<length_t>(0);
        auto size = access::size(poly);
        for( std::size_t i = 0, j=1; j < size; i = j++ )
        {
            length += point_point_distance(access::get_point(poly, i), access::get_point(poly, j));
            std::get<0>(result) = i;
            if(length > pos)
            {
                auto l = length - pos;
                if (l > constants::zero<length_t>())
                    std::get<1>(result) = construct<result_of::polyline_point_at_position<Polyline, Length>::point_type>( access::get_point(poly, j) + l * normalize(access::get_point(poly, i) - access::get_point(poly, j)));
                break;
            }
        }

        return result;
    }

    namespace result_of
    {
        template <typename Polyline, typename Length>
        struct polyline_split
        {
        private:

            using access = point_sequence_traits<Polyline>;
            using point_t = typename access::point_type;
            using length_t = typename geometric_traits<point_t>::arithmetic_type;

        public:

            using point_type = point<length_t, dimension_of<point_t>::value>;
            using type = std::tuple<polyline<point_type>, polyline<point_type>>;

        };

    }//! namespace result_of;

    template <typename Polyline, typename Length>
    inline typename result_of::polyline_split<Polyline, Length>::type polyline_split(const Polyline& poly, const Length& pos)
    {
        using access = point_sequence_traits<Polyline>;
        using point_t = typename result_of::polyline_split<Polyline, Length>::point_type;
        using length_t = typename geometric_traits<point_t>::arithmetic_type;
        using polyline_t = polyline<point_t>;
        auto length = construct<length_t>(0);
        auto size = access::size(poly);
        polyline_t a;
        std::size_t i, j;
        
		for (i = 0, j = 1; j < size; i = j++)
        {
            auto d = point_point_distance(access::get_point(poly, i), access::get_point(poly, j));
            length += d;
            a.push_back(access::get_point(poly, i));
            if (length > pos)
            {
                auto l = length - pos;
                if (l < d)
                    a.push_back(access::get_point(poly, j) + l * normalize(access::get_point(poly, i) - access::get_point(poly, j)));
                break;
            }
        }

		polyline_t b = { a.back() };
        for (; j < size; ++j)
            b.push_back(access::get_point(poly, j));

		return std::tuple<polyline_t, polyline_t>{std::move(a), std::move(b)};
    }

}//namespace geometrix;

