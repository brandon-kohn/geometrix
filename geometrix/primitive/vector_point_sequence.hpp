//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_VECTOR_POINT_SEQUENCE_HPP
#define GEOMETRIX_VECTOR_POINT_SEQUENCE_HPP

#include <geometrix/primitive/point_sequence_traits.hpp>
#include <geometrix/utility/construction_policy.hpp>
#include <boost/range.hpp>
#include <vector>

namespace geometrix {  
    template <typename Point>
    struct construction_policy< std::vector< Point >, typename geometric_traits<Point>::is_point >
    {   
        template <typename Range>
        static std::vector< Point > construct( const Range& pRange ) 
        {
            return std::vector< Point >( boost::begin( pRange ), boost::end( pRange ) );
        }
    };
	
	//! specialize vector.                                                                                                         
	template <typename Point> 
	struct is_point_sequence< std::vector<Point>, typename geometric_traits<Point>::is_point > 
		: boost::true_type {};

	template <typename Point>
	struct point_sequence_traits< std::vector< Point >, typename geometric_traits<Point>::is_point >
	{
		typedef Point                                        point_type;
		typedef std::vector< point_type >                    container_type;
		typedef typename geometric_traits<point_type>::dimension_type dimension_type;
		typedef typename container_type::iterator                     iterator;
		typedef typename container_type::const_iterator               const_iterator;
		typedef typename container_type::reverse_iterator             reverse_iterator;
		typedef typename container_type::const_reverse_iterator       const_reverse_iterator;
		static iterator                              begin(container_type& p) { return p.begin(); }
		static const_iterator                        begin(const container_type& p) { return p.begin(); }
		static iterator                              end(container_type& p) { return p.end(); }
		static const_iterator                        end(const container_type& p) { return p.end(); }
		static reverse_iterator                      rbegin(container_type& p) { return p.rbegin(); }
		static const_reverse_iterator                rbegin(const container_type& p) { return p.rbegin(); }
		static reverse_iterator                      rend(container_type& p) { return p.rend(); }
		static const_reverse_iterator                rend(const container_type& p) { return p.rend(); }
		static std::size_t                           size(const container_type& p) { return p.size(); }
		static bool                                  empty(const container_type& p) { return p.empty(); }
		static const point_type&                     get_point(const container_type& pointSequence, std::size_t index) { return pointSequence[index]; }
		static point_type&                           get_point(container_type& pointSequence, std::size_t index) { return pointSequence[index]; }
		static const point_type&                     front(const container_type& pointSequence) { return pointSequence.front(); }
		static point_type&                           front(container_type& pointSequence) { return pointSequence.front(); }
		static const point_type&                     back(const container_type& pointSequence) { return pointSequence.back(); }
		static point_type&                           back(container_type& pointSequence) { return pointSequence.back(); }
		static void                                  pop_back(container_type& pointSequence) { pointSequence.pop_back(); }
	};

}//namespace geometrix;

#endif //GEOMETRIX_VECTOR_POINT_SEQUENCE_HPP
