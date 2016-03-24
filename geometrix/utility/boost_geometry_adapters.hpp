//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_ADAPT_BOOST_GEOMETRY_HPP
#define GEOMETRIX_ADAPT_BOOST_GEOMETRY_HPP

#include <geometrix/primitive/point.hpp>
#include <geometrix/primitive/polygon.hpp>
#include <geometrix/primitive/polyline.hpp>
#include <geometrix/tensor/tensor_access_policy.hpp>

#include <boost/geometry.hpp>
#include <boost/range.hpp>

//! NOTE: This is a dead end as geometrix::get/set clash with boost::geometry::get/set.

namespace boost
{
	namespace geometry
	{
		namespace traits
		{
			template<typename T>
			struct tag<T, typename geometrix::geometric_traits<T>::is_point>
			{
				typedef point_tag type;
			};

			template<typename T>
			struct coordinate_type<T, typename geometrix::geometric_traits<T>::is_point>
				: geometrix::type_at<T, 0>
			{};

			template<typename T>
			struct coordinate_system<T, typename geometrix::geometric_traits<T>::is_point>
			{
				typedef cs::cartesian type;
			};

			template<typename T>
			struct dimension<T, typename geometrix::geometric_traits<T>::is_point> 
				: geometrix::dimension_of<T> 
			{};

			template<typename T, std::size_t N>
			struct access<T, N, typename geometrix::geometric_traits<T>::is_point>
			{
				static typename geometrix::type_at<T, N>::type get(T const& p)
				{
					return geometrix::get<N>(p);
				}

				static void set(T& p, typename geometrix::type_at<T,N>::type const& value)
				{
					geometrix::set<N>(p, value);
				}
			};

			template<typename Point>
			struct tag<geometrix::polyline<Point>>
			{
				typedef linestring_tag type;
			};

			template<typename Point>
			struct tag<geometrix::polygon<Point>>
			{
				typedef ring_tag type;
			};

		}//! namespace traits;
	}//! namespace geometry;

	template <typename Point>
	struct range_iterator<geometrix::polyline<Point>>
	{
		typedef typename geometrix::polyline<Point>::iterator type;
	};

	template<typename Point>
	struct range_const_iterator<polyline<Point>>
	{
		typedef typename polyline<Point>::const_iterator type;
	};

	template <typename Point>
	struct range_iterator<geometrix::polygon<Point>>
	{
		typedef typename geometrix::polygon<Point>::iterator type;
	};

	template<typename Point>
	struct range_const_iterator<polygon<Point>>
	{
		typedef typename polygon<Point>::const_iterator type;
	};
} // namespace boost;

//! Polyline
template <typename Point>
inline typename geometrix::polyline<Point>::iterator range_begin(geometrix::polyline<Point>& p) { return p.begin(); }

template <typename Point>
inline typename geometrix::polyline<Point>::iterator range_end(geometrix::polyline<Point>& p) { return p.end(); }

template <typename Point>
inline typename geometrix::polyline<Point>::const_iterator range_begin(const geometrix::polyline<Point>& p) { return p.begin(); }

template <typename Point>
inline typename geometrix::polyline<Point>::const_iterator range_end(const geometrix::polyline<Point>& p) { return p.end(); }

//! Polygon
template <typename Point>
inline typename geometrix::polygon<Point>::iterator range_begin(geometrix::polygon<Point>& p) { return p.begin(); }

template <typename Point>
inline typename geometrix::polygon<Point>::iterator range_end(geometrix::polygon<Point>& p) { return p.end(); }

template <typename Point>
inline typename geometrix::polygon<Point>::const_iterator range_begin(const geometrix::polygon<Point>& p) { return p.begin(); }

template <typename Point>
inline typename geometrix::polygon<Point>::const_iterator range_end(const geometrix::polygon<Point>& p) { return p.end(); }

#endif //GEOMETRIX_ADAPT_BOOST_GEOMETRY_HPP
