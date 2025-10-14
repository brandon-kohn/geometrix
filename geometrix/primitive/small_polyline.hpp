//
//! Copyright � 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once 
#include <geometrix/primitive/polyline.hpp>
#include <geometrix/primitive/array_point_sequence.hpp>
#include <type_traits>

namespace geometrix {

    namespace detail {
		template <geometrix::concepts::Point T0, geometrix::concepts::Point... Ts>
		std::array<T0, sizeof...(Ts) + 1> make_array_helper( T0 const& p0, Ts const&... p)
		{
			return { p0, p... };
		}
    }

    //! \class small_polyline
    //! \brief A class for specifying a polyline.
    template <concepts::Point T, std::size_t N>
	class small_polyline : public std::array<T, N>
	{
		using container_type = std::array<T, N>;

	public:
		using point_type = T;
		using polyline_type = small_polyline<point_type, N>;
		using dimension_type = typename dimension_of<point_type>::type;
		using arithmetic_type = typename geometric_traits<point_type>::arithmetic_type;

		small_polyline() = default;

		template <concepts::Point... Ts>
		small_polyline( Ts const&... points )
			: container_type{ detail::make_array_helper( points...) }
		{
			GEOMETRIX_ASSERT_MSG( sizeof...( Ts ) <= N, "small_polyline cannot hold more points than its size." );
		}

        small_polyline(std::initializer_list<T> l)
            : container_type(l)
        {}

        ~small_polyline() = default;
    };

    template <concepts::Point Point, std::size_t N>
    struct point_sequence_traits<small_polyline<Point, N>>
        : point_sequence_traits<std::array<Point, N>>
    {};

    template <concepts::Point Point, std::size_t N>
    struct geometric_traits< small_polyline<Point, N> >
    {
        typedef void                                      is_point_sequence;
        typedef void                                      is_polyline;
        typedef Point                                     point_type;
        typedef small_polyline<Point, N>                  polyline_type;
        typedef typename dimension_of< point_type >::type dimension_type;
    };

    template <concepts::Point Point, std::size_t N>
    struct construction_policy< small_polyline< Point, N> >
    {
        template <typename ...Args>
        static small_polyline< Point, N> construct(Args&&... a)
        {
            return small_polyline< Point, N>(std::forward<Args>(a)...);
        }
    };

}//namespace geometrix;

