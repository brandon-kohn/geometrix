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

#include <geometrix/primitive/point_sequence_traits.hpp>
#include <geometrix/primitive/point_traits.hpp>
#include <geometrix/utility/construction_policy.hpp>
#include <vector>
#include <initializer_list>

namespace geometrix {

//! \class polygon
//! \brief A class for specifying a polygon.
template <typename Point>
class polygon : public std::vector<Point>
{
    BOOST_CLASS_REQUIRE( Point, geometrix, PointConcept );

public:

    typedef Point                                                  point_type;
    typedef typename dimension_of< point_type >::type              dimension_type;
	typedef typename geometric_traits<point_type>::arithmetic_type arithmetic_type;
    
	template <typename ...Args>
    polygon( Args&&... a )
        : std::vector<Point>( std::forward<Args>(a)... )
    {}

	polygon(std::initializer_list<Point> l)
		: std::vector<Point>(l)
	{}
    
    ~polygon(){}
	
private:

};

template <typename Point>
struct is_point_sequence< polygon<Point> > : boost::true_type {};

template <typename Point>
struct point_sequence_traits<polygon<Point>>
	: point_sequence_traits<std::vector<Point>>
{};

template <typename Point>                                                               
struct geometric_traits< polygon<Point> >                                                
{ 
    typedef Point                                     point_type;     
    typedef polygon<Point>                            polygon_type;      
    typedef typename dimension_of< point_type >::type dimension_type; 
};                                                                        

template <typename Point>
struct construction_policy< polygon< Point > >
{   
	template <typename ...Args>
    static polygon< Point > construct(Args... args) 
    {
        return polygon< Point >( args... );
    }
};

}//namespace geometrix;

#endif //GEOMETRIX_POLYGON_HPP
