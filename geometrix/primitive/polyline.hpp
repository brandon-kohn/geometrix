//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_POLYLINE_HPP
#define GEOMETRIX_POLYLINE_HPP

#include <geometrix/primitive/vector_point_sequence.hpp>

namespace geometrix {

	template <typename Point, typename Allocator>
	class polygon;

	//! \class polyline
	//! \brief A class for specifying a polyline.
	template <typename Point, typename Allocator = std::allocator<Point>>
	class polyline : public std::vector<Point, Allocator>
	{
		BOOST_CLASS_REQUIRE(Point, geometrix, PointConcept);
		using container_type = std::vector<Point, Allocator>;
	public:

		using point_type = Point;
		using polyline_type = polyline<point_type, Allocator>;
		using dimension_type = typename dimension_of< point_type >::type;
		using arithmetic_type = typename geometric_traits<point_type>::arithmetic_type;

		polyline() = default;

		template <typename Alloc>
		polyline(const polyline<Point, Alloc>& pgon)
			: container_type(pgon)
		{}

		polyline(const polyline_type& other) = default;

		polyline(polyline&& pgon)
			: container_type(std::forward<container_type>(pgon))
		{}

		polyline(std::initializer_list<Point> l)
			: container_type(l)
		{}

		polyline(container_type&& vec)
			: container_type(std::forward<container_type>(vec))
		{}

		template <typename Alloc>
		polyline(const std::vector<Point, Alloc>& vec)
			: container_type(vec)
		{}

		template <typename InputIt>
		polyline(InputIt first, InputIt last)
			: container_type(first, last)
		{}

		~polyline() = default;

		//! Don't construct from polylines.
		polyline(polygon<Point, Allocator>&&) = delete;
		template <typename Alloc>
		polyline(const polygon<Point, Alloc>&) = delete;

		//! Assignment
		polyline_type& operator =(polyline_type&& pgon)
		{
			static_cast<container_type&>(*this) = std::forward<container_type>(pgon);
			return *this;
		}

		template <typename Alloc>
		polyline_type& operator =(const polyline<Point, Alloc>& pgon)
		{
			static_cast<container_type&>(*this) = pgon;
			return *this;
		}

		polyline_type& operator =(const polyline_type& rhs) = default;

		polyline_type& operator =(container_type&& pgon)
		{
			static_cast<container_type&>(*this) = std::forward<container_type>(pgon);
			return *this;
		}

		polyline_type& operator =(const container_type& pgon)
		{
			static_cast<container_type&>(*this) = pgon;
			return *this;
		}

		polyline_type& operator =(std::initializer_list<Point> l)
		{
			static_cast<container_type&>(*this) = l;
			return *this;
		}

		//! Don't assign from polygon.
		polyline_type& operator =(polygon<Point, Allocator>&& pline) = delete;

		template <typename Alloc>
		polyline_type& operator =(const polygon<Point, Alloc>& pline) = delete;
	};

	template <typename Point, typename Allocator>
	struct is_point_sequence< polyline<Point, Allocator> > : boost::true_type {};

	template <typename Point, typename Allocator>
	struct point_sequence_traits<polyline<Point, Allocator>>
		: point_sequence_traits<std::vector<Point, Allocator>>
	{};

	template <typename Point, typename Allocator>
	struct geometric_traits< polyline<Point, Allocator> >
	{
		typedef void                                      is_polyline;
		typedef Point                                     point_type;
		typedef polyline<Point, Allocator>                polyline_type;
		typedef typename dimension_of< point_type >::type dimension_type;
	};

	template <typename Point, typename Allocator>
	struct construction_policy< polyline< Point, Allocator > >
	{
		template <typename ...Args>
		static polyline< Point, Allocator> construct(Args&&... a)
		{
			return polyline< Point, Allocator >(std::forward<Args>(a)...);
		}
	};

}//namespace geometrix;

#endif //GEOMETRIX_POLYLINE_HPP
