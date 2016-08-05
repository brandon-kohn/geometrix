//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_POLYGON_HPP
#define GEOMETRIX_POLYGON_HPP

#include <geometrix/primitive/vector_point_sequence.hpp>

namespace geometrix {

	template <typename Point, typename Allocator>
	class polyline;

	//! \class polygon
	//! \brief A class for specifying a polygon.
	template <typename Point, typename Allocator = std::allocator<Point>>
	class polygon : public std::vector<Point, Allocator>
	{
		BOOST_CLASS_REQUIRE(Point, geometrix, PointConcept);
		using container_type = std::vector<Point, Allocator>;
	public:

		using point_type = Point;
		using polygon_type = polygon<point_type, Allocator>;
		using dimension_type = typename dimension_of< point_type >::type;
		using arithmetic_type = typename geometric_traits<point_type>::arithmetic_type;

		polygon() = default;

		template <typename Alloc>
		polygon(const polygon<Point, Alloc>& pgon)
			: container_type(pgon)
		{}

		polygon(polygon&& pgon)
			: container_type(std::forward<container_type>(pgon))
		{}
		
		polygon(std::initializer_list<Point> l)
			: container_type(l)
		{}

		polygon(container_type&& vec)
			: container_type(std::forward<container_type>(vec))
		{}

		template <typename Alloc>
		polygon(const std::vector<Point, Alloc>& vec)
			: container_type(vec)
		{}

		template <typename InputIt>
		polygon(InputIt first, InputIt last)
			: container_type(first, last)
		{}

		~polygon() = default;
		
		//! Don't construct from polylines.
		polygon(polyline<Point, Allocator>&&) = delete;
		template <typename Alloc>
		polygon(const polyline<Point, Alloc>&) = delete;

		//! Assignment
		polygon_type& operator =(polygon_type&& pgon)
		{
			static_cast<container_type&>(*this) = std::forward<container_type>(pgon);
			return *this;
		}

		template <typename Alloc>
		polygon_type& operator =(const polygon<Point, Alloc>& pgon)
		{
			static_cast<container_type&>(*this) = pgon;
			return *this;
		}

		polygon_type& operator =(container_type&& pgon)
		{
			static_cast<container_type&>(*this) = std::forward<container_type>(pgon);
			return *this;
		}

		polygon_type& operator =(const container_type& pgon)
		{
			static_cast<container_type&>(*this) = pgon;
			return *this;
		}

		polygon_type& operator =(std::initializer_list<Point> l)
		{
			static_cast<container_type&>(*this) = l;
			return *this;
		}
				
		//! Don't assign from polyline.
		polygon_type& operator =(polyline<Point, Allocator>&& pline) = delete;
		template <typename Alloc>
		polygon_type& operator =(const polyline<Point, Alloc>& pline) = delete;
	};

	template <typename Point, typename Allocator>
	struct is_point_sequence< polygon<Point, Allocator> > : boost::true_type {};

	template <typename Point, typename Allocator>
	struct point_sequence_traits<polygon<Point, Allocator>>
		: point_sequence_traits<std::vector<Point, Allocator>>
	{};

	template <typename Point, typename Allocator>
	struct geometric_traits< polygon<Point, Allocator> >
	{
		typedef void                                      is_polygon;
		typedef Point                                     point_type;
		typedef polygon<Point, Allocator>                 polygon_type;
		typedef typename dimension_of< point_type >::type dimension_type;
	};

	template <typename Point, typename Allocator>
	struct construction_policy< polygon< Point, Allocator > >
	{
		template <typename ...Args>
		static polygon< Point, Allocator> construct(Args&&... a)
		{
			return polygon< Point, Allocator >(std::forward<Args>(a)...);
		}
	};

}//namespace geometrix;

#endif //GEOMETRIX_POLYGON_HPP
