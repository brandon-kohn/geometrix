//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_RECTANGLE_HPP
#define GEOMETRIX_RECTANGLE_HPP

#include <geometrix/primitive/vector_point_sequence.hpp>
#include <geometrix/primitive/point_traits.hpp>
#include <geometrix/utility/construction_policy.hpp>
#include <array>
#include <initializer_list>

namespace geometrix {

//! \class rectangle
//! \brief A class for specifying a rectangle.
template <typename Point>
class rectangle : public std::array<Point, 4>
{
    BOOST_CLASS_REQUIRE( Point, geometrix, PointConcept );
	typedef std::array<Point, 4> array_type;

public:

// 	template <typename T1, typename T2, typename ...Ts>
// 	rectangle(T1&& a, T2&& b, Ts&&... args)
// 		: array_type{{std::forward<T1>(a), std::forward<T2>(b), std::forward<Ts>(args)...}}
// 	{}
	
	rectangle(const std::array<Point, 4>& a)
		: std::array<Point, 4>(a)
	{}

	template <typename Point1, typename Point2, typename Point3, typename Point4>
	rectangle(const Point1& p1, const Point2& p2, const Point3& p3, const Point4& p4)
		: rectangle(array_type{ {construct<Point>(p1), construct<Point>(p2), construct<Point>(p3), construct<Point>(p4)} })
	{}
	
    typedef Point                                                  point_type;
    typedef typename dimension_of< point_type >::type              dimension_type;
	typedef typename geometric_traits<point_type>::arithmetic_type arithmetic_type;
};

template <typename Point>
struct is_point_sequence< rectangle<Point> > : boost::true_type {};

template <typename Point>
struct point_sequence_traits<rectangle<Point>>
	: point_sequence_traits<std::array<Point, 4>>
{};

template <typename Point>                                                               
struct geometric_traits< rectangle<Point> >                                                
{ 
    typedef Point                                     point_type;     
    typedef rectangle<Point>                          rectangle_type;      
    typedef typename dimension_of< point_type >::type dimension_type; 
};                                                                        

template <typename Point>
struct construction_policy< rectangle< Point > >
{   
	template <typename ...Args>
    static rectangle< Point > construct(Args... args) 
    {
		return std::array< Point, 4 >{ args... };
    }
};

}//namespace geometrix;

#endif //GEOMETRIX_RECTANGLE_HPP
