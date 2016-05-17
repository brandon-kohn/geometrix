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
#include <geometrix/primitive/point_traits.hpp>
#include <geometrix/utility/construction_policy.hpp>
#include <vector>
#include <initializer_list>

namespace geometrix {

	//! \class polyline
	//! \brief A class for specifying a polyline.
	template <typename Point>
	class polyline : public std::vector<Point>
	{
		BOOST_CLASS_REQUIRE(Point, geometrix, PointConcept);

	public:

		typedef Point                                                  point_type;
		typedef typename dimension_of< point_type >::type              dimension_type;
		typedef typename geometric_traits<point_type>::arithmetic_type arithmetic_type;
		
		template <typename ...Args>
		polyline(Args&&... a)
			: std::vector<Point>(std::forward<Args>(a)...)
		{}

		polyline(std::initializer_list<Point> l)
			: std::vector<Point>(l)
		{}

		~polyline() {}

	private:

	};

	template <typename Point>
	struct is_point_sequence< polyline<Point> > : boost::true_type {};
	
	template <typename Point>
	struct point_sequence_traits<polyline<Point>>
		: point_sequence_traits<std::vector<Point>>
	{};

	template <typename Point>
	struct geometric_traits< polyline<Point> >
	{
		typedef Point                                     point_type;
		typedef polyline<Point>                            polyline_type;
		typedef typename dimension_of< point_type >::type dimension_type;
	};

	template <typename Point>
	struct construction_policy< polyline< Point > >
	{
		template <typename ...Args>
		static polyline< Point > construct(Args... args)
		{
			return polyline< Point >(args...);
		}
	};

}//namespace geometrix;

#endif //GEOMETRIX_POLYLINE_HPP
