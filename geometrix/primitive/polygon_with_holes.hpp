//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_POLYGON_WITH_HOLES_HPP
#define GEOMETRIX_POLYGON_WITH_HOLES_HPP

#include <geometrix/primitive/vector_point_sequence.hpp>

namespace geometrix {

//! \class polygon_with_holes
//! \brief A class for specifying a polygon with holes.
template <typename Point, typename Allocator = std::allocator<Point>>
class polygon_with_holes
{
    BOOST_CLASS_REQUIRE( Point, geometrix, PointConcept );

public:

    using point_type = Point;
	using polygon_type = polygon<point_type, Allocator>;
    using dimension_type = typename dimension_of< point_type >::type;
	using arithmetic_type = typename geometric_traits<point_type>::arithmetic_type;

	polygon_with_holes() = default;
	polygon_with_holes(const polygon_with_holes& other) = default;
	polygon_with_holes(polygon_with_holes&& other)
		: polygon_with_holes(std::move(other.m_outer), std::move(other.m_holes))
	{}
	~polygon_with_holes() = default;
    
	template <typename ...Holes>
    polygon_with_holes( polygon_type&& outer )
        : m_outer(std::forward<polygon_type>(outer))
    {
		
	}

	polygon_with_holes(const polygon_type& outer)
		: m_outer(outer)
	{

	}
	
	polygon_with_holes(polygon_type&& outer, std::vector<polygon_type>&& holes)
		: m_outer(std::forward<polygon_type>(outer))
		, m_holes(std::forward<std::vector<polygon_type>>(holes))
	{

	}

	polygon_with_holes(const polygon_type& outer, std::vector<polygon_type>&& holes)
		: m_outer(outer)
		, m_holes(std::forward<std::vector<Polygon>>(holes))
	{

	}
	
	polygon_with_holes(polygon_type&& outer, const std::vector<polygon_type>& holes)
		: m_outer(std::forward<polygon_type>(outer))
		, m_holes(holes)
	{

	}

	polygon_with_holes(const polygon_type& outer, const std::vector<polygon_type>& holes)
		: m_outer(outer)
		, m_holes(holes)
	{

	}

	polygon_with_holes& operator = (const polygon_with_holes& other) = default;
	polygon_with_holes& operator = (polygon_with_holes&& other)
	{
		m_outer = std::move(other.m_outer);
		m_holes = std::move(other.m_holes);
		return *this;
	}

	polygon_type& get_outer() { return m_outer; }
	const polygon_type& get_outer() const { return m_outer; }
	
	template <typename OuterPolygon>
	void set_outer(OuterPolygon&& outer)
	{
		m_outer = std::move(outer);
	}

	template <typename OuterPolygon>
	void set_outer(const OuterPolygon& outer)
	{
		m_outer = outer;
	}

	std::vector<polygon_type>& get_holes() 
	{
		return m_holes;
	}

	const std::vector<polygon_type>& get_holes() const
	{
		return m_holes;
	}

	template <typename HolePolygon>
	void add_hole(HolePolygon&& outer)
	{
		m_holes.emplace_back(std::move(outer));
	}

	template <typename HolePolygon>
	void add_hole(const HolePolygon& outer)
	{
		m_holes.emplace_back(outer);
	}
		
private:

	polygon_type              m_outer;
	std::vector<polygon_type> m_holes;

};

template <typename Point>                                                               
struct geometric_traits< polygon_with_holes<Point> >                                                
{ 
    typedef Point                                     point_type;     
    typedef polygon<Point>                            polygon_type;      
    typedef typename dimension_of< point_type >::type dimension_type; 
};                                                                        

}//namespace geometrix;

#endif //GEOMETRIX_POLYGON_WITH_HOLES_HPP
