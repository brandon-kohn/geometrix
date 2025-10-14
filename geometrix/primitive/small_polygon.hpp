//
//! Copyright � 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once 
#include <geometrix/primitive/polygon.hpp>
#include <geometrix/primitive/small_polyline.hpp>
#include <type_traits>

namespace geometrix {

    //! \class small_polygon
    //! \brief A class for specifying a polygon.
    template <concepts::Point T, std::size_t N>
	class small_polygon : public std::array<T, N>
	{
		using container_type = std::array<T, N>;
        static_assert(N > 2, "small_polygon must have at least 3 points to form a polygon.");

	public:
		using point_type = T;
		using polygon_type = small_polygon<point_type, N>;
		using dimension_type = typename dimension_of<point_type>::type;
		using arithmetic_type = typename geometric_traits<point_type>::arithmetic_type;

		small_polygon() = default;

		template <concepts::Point... Ts>
		small_polygon( Ts const&... points )
			: container_type{ detail::make_array_helper( points...) }
		{
			GEOMETRIX_ASSERT_MSG( sizeof...( Ts ) <= N, "small_polygon cannot hold more points than its size." );
		}

        small_polygon(std::initializer_list<T> l)
            : container_type(l)
        {}

        ~small_polygon() = default;
    };

    template <concepts::Point Point, std::size_t N>
    struct point_sequence_traits<small_polygon<Point, N>>
        : point_sequence_traits<std::array<Point, N>>
    {};

    template <concepts::Point Point, std::size_t N>
    struct geometric_traits< small_polygon<Point, N> >
    {
        typedef void                                      is_point_sequence;
        typedef void                                      is_polygon;
        typedef Point                                     point_type;
        typedef small_polygon<Point, N>                   polygon_type;
        typedef typename dimension_of< point_type >::type dimension_type;
    };

    template <concepts::Point Point, std::size_t N>
    struct construction_policy< small_polygon< Point, N> >
    {
        template <typename ...Args>
        static small_polygon< Point, N> construct(Args&&... a)
        {
            return small_polygon< Point, N>(std::forward<Args>(a)...);
        }
    };

}//namespace geometrix;

