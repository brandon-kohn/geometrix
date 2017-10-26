//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_ARRAY_POINT_SEQUENCE_HPP
#define GEOMETRIX_ARRAY_POINT_SEQUENCE_HPP

#include <geometrix/primitive/point_sequence_traits.hpp>
#include <geometrix/utility/construction_policy.hpp>
#include <geometrix/primitive/point.hpp>
#include <array>

#include <boost/range.hpp>

namespace geometrix {
    template <typename Point, std::size_t Dimension>
    struct construction_policy< std::array< Point, Dimension >, typename geometric_traits<typename std::decay<Point>::type>::is_point >
    {
        template <typename Range>
        static std::array<Point, Dimension> construct( const Range& pRange )
        {
            return std::array< Point, Dimension>( boost::begin( pRange ), boost::end( pRange ) );
        }
    };

    template <typename Point, std::size_t N>
    struct point_sequence_traits<std::array<Point, N>, typename geometric_traits<Point>::is_point>
    {
        static_assert(N != 0, "array point sequence must have non-zero dimension.");
        typedef Point                                        point_type;
        typedef std::array< point_type, N>                   container_type;
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
        static bool                                  empty(const container_type& /*p*/) { return false; }
        static const point_type&                     get_point(const container_type& pointSequence, std::size_t index) { return pointSequence[index]; }
        static point_type&                           get_point(container_type& pointSequence, std::size_t index) { return pointSequence[index]; }
        static const point_type&                     front(const container_type& pointSequence) { return pointSequence[0]; }
        static point_type&                           front(container_type& pointSequence) { return pointSequence[0]; }
        static const point_type&                     back(const container_type& pointSequence) { return pointSequence[N - 1]; }
        static point_type&                           back(container_type& pointSequence) { return pointSequence[N - 1]; }
    };
    
    template <typename Point, std::size_t N>
    struct geometric_traits<std::array<Point, N>, typename geometric_traits<typename std::decay<Point>::type>::is_point>
    {
        using is_point_sequence = void;
        using point_type = typename std::decay<Point>::type;
        using dimension_type = typename dimension_of<point_type>::type;
    };

}//! namespace geometrix;

#endif //GEOMETRIX_ARRAY_POINT_SEQUENCE_HPP
