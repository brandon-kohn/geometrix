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

#include <geometrix/primitive/point_sequence.hpp>
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

public:

    typedef Point                                                  point_type;
    typedef typename dimension_of< point_type >::type              dimension_type;
	typedef typename geometric_traits<point_type>::arithmetic_type arithmetic_type;
    
	template <typename ...Args>
    rectangle( Args&&... a )
        : std::array<Point, 4>( std::forward<Args>(a)... )
    {}

	rectangle(std::initializer_list<Point> l)
		: std::array<Point, 4>(l)
	{}
    
    ~rectangle(){}
	
private:

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
        return rectangle< Point >( args... );
    }
};

}//namespace geometrix;

#endif //GEOMETRIX_RECTANGLE_HPP
