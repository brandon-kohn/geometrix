//
//! Copyright © 2019
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include <geometrix/algorithm/intersection/segment_polygon_intersection.hpp>
#include <geometrix/primitive/point_sequence_traits.hpp>
#include <geometrix/primitive/point.hpp>
#include <geometrix/arithmetic/arithmetic_promotion_policy.hpp>
#include <geometrix/utility/utilities.hpp>

#include <boost/container/flat_set.hpp>

/////////////////////////////////////////////////////////////////////////////
//
// NAMESPACE
//
/////////////////////////////////////////////////////////////////////////////
namespace geometrix {

    namespace detail {

    }//! namespace detail;
	
//! \brief Compute whether the segment defined by A->B intersects a polyline.
//! The type Visitor must define bool operator()(intersection_type, size_t i, size_t j, point<arithmetic_type_of<PointA,PointB>::type, 2>[2]) which processes 
//! an intersection event between the segment and the i-j segment of the polyline 
//! along with the resulting intersection points. The return value is whether the algorithm should cease.
template <typename Polyline, typename Polygon, typename Visitor, typename NumberComparisonPolicy>
inline bool polyline_polygon_intersection( const Polyline& A, const Polygon& B, Visitor&& visitor, const NumberComparisonPolicy& cmp )
{
	BOOST_CONCEPT_ASSERT((PolylineConcept<Polyline>));
	BOOST_CONCEPT_ASSERT((PolygonConcept<Polygon>));
	BOOST_CONCEPT_ASSERT((NumberComparisonPolicyConcept<NumberComparisonPolicy>));
	using access1 = point_sequence_traits<Polyline>;
	using access2 = point_sequence_traits<Polygon>;
	using length_t = typename select_arithmetic_type_from_sequences<typename access1::point_type, typename access2::point_type>::type;
	using point_type = point<length_t, 2>;

    bool intersected = false;
    enum vertex_class{ Original, Enter, Exit };
    struct vertex_desc 
    { 
        vector_desc(point_type const& p, polygon_containment contain, vertex_class c, std::size_t index, length_t offset)
        : p{p}
        , contain{contain}
        , c{c}
        , index{index}
        , offset{offset}
        {} 
 
        bool operator <(const vertex_desc& rhs) const 
        {
            return lexicographical_compare(index, rhs.index, offset, rhs.offset, c, rhs.c);
        }

        point_type p;
        polygon_containment contain;
        vertex_class c;
        std::size_t index;
        length_t offset; 
    };

    std::vector<polygon_containment> contains;
    contains.reserve(access1::size(A));

    length_t tLength = constants::zero<length_t>();
    contains.push_back(point_polygon_containment_or_on_border(access1::get_point(A,0), B, cmp); 
    boost::container::flat_multiset<vertex_desc> AList = { vertex_desc(access1::get_point(A,0), contains.back(), Original, std::size_t{}, tLength) };
	for (std::size_t j1 = 1; j1 < access1::size(A); ++j1)
    {
        point_type pj{access1::get_point(A, j1)};
        contains.push_back(point_polygon_containment_or_on_border(pj, B, cmp)); 
		auto d = point_point_distance(access1::get_point(A, j1 - 1), pj);
		tLength += d;
        AList.emplace(a, contains.back(), Original, pj, tLength);
    }

    polygon_containment state = contains[0];
    tLength = constants::zero<length_t>();
	for (std::size_t i1 = 0, j1 = 1; j1 < access1::size(A); i1 = j1++)
	{
        point_type pi{access1::get_point(A, i1)};
        point_type pj{access1::get_point(A, j1)};

		auto iVisitor = [&](intersection_type iType, std::size_t i, std::size_t j, const point_type& x1, const point_type& x2) -> bool
		{
			if (iType == e_non_crossing || iType == e_invalid_intersection)
				return false;

            intersected = true;
        
			switch (iType)
			{
			case e_endpoint:
				BOOST_FALLTHROUGH;
			case e_crossing:
            {
                auto d = point_point_distance(pi, x1);
				auto c = point_polygon_containment_or_on_border(x1, B, cmp);
				AList.emplace(x1, c, istate, i1, tLength + d);
				break;
            }
			case e_overlapping:
            {
				//! Pick the furthest and use it.
                auto d1 = point_point_distance(pi, x1);
                auto d2 = point_point_distance(pi, x2);
				length_t d;
				point_type pf;
				std::tie(d, pf) = d1 > d2 ? {d1, x1} : {d2, x2};
				
				auto c = point_polygon_containment_or_on_border(pf, B, cmp);
				AList.emplace(pf, c, istate, i1, tLength + d);
				break;
            }
			default:
			}

		};

		segment_polygon_border_intersection(pi, pj, B, iVisitor, cmp);
		
		auto d = point_point_distance(access1::get_point(A, j1 - 1), pj);
		tLength += d;
	}

	return intersected;
}

}//namespace geometrix;

