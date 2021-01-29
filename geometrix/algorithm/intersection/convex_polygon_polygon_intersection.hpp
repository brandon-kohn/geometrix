//
//! Copyright © 2021
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_CONVEX_POLYGON_POLYGON_INTERSECTION_HPP
#define GEOMETRIX_CONVEX_POLYGON_POLYGON_INTERSECTION_HPP
#pragma once

#include <geometrix/algorithm/intersection/segment_segment_intersection.hpp>
#include <geometrix/primitive/point_sequence_traits.hpp>
#include <geometrix/primitive/point.hpp>
#include <geometrix/arithmetic/arithmetic_promotion_policy.hpp>
#include <geometrix/utility/utilities.hpp>
#include <boost/concept_check.hpp>
#include <boost/logic/tribool.hpp>

/////////////////////////////////////////////////////////////////////////////
//
// NAMESPACE
//
/////////////////////////////////////////////////////////////////////////////
namespace geometrix {

    namespace detail {

    }//! namespace detail;
	
namespace geometrix {
    
    enum class polygon_intersection_type
    {
        invalid_intersection = -1,
        none = 0,
        face = 1,
        vertex = 2,
        overlapping = 3
    };

    //! From Computational Geometry in C, O'Rourke.
    template <typename Polygon1, typename Polygon2, typename Visitor, typename NumberComparisonPolicy>
    inline polygon_intersection_type convex_polygon_polygon_intersection( const Polygon1& pgon1, const Polygon2& pgon2, Visitor&& visitor, const NumberComparisonPolicy& cmp)
    {
        using namespace convex_polygon_polygon_intersection_detail;
        BOOST_CONCEPT_ASSERT((PolygonConcept<Polygon1>));
        BOOST_CONCEPT_ASSERT((PolygonConcept<Polygon2>));
        BOOST_CONCEPT_ASSERT((NumberComparisonPolicyConcept<NumberComparisonPolicy>));
        using access1 = point_sequence_traits<Polygon1>;
        using access2 = point_sequence_traits<Polygon2>;
        using length_t = typename select_arithmetic_type_from_sequences<typename access1::point_type, typename access2::point_type>::type;
        using area_t = decltype(std::declval<length_t>()*std::declval<length_t>());
        using point_t = point<length_t, 2>;
        using vector_t = vector<length_t, 2>;
        enum in_out_state{ pgon1_in, pgon2_in, unknown };
        auto get_point1 = [&](std::size_t i){ return access1::get_point(pgon1, i); }
        auto get_point2 = [&](std::size_t i){ return access2::get_point(pgon2, i); }
        auto advance = []( std::size_t& a, std::size_t& aa, int n ) { ++aa; ++a %= n; };
        auto a = std::size_t{};
        auto b = std::size_t{};
        auto aa = std::size_t{};
        auto ba = std::size_t{};
        auto inflag = unknown;

        auto n = access1::size(pgon1);
        auto n2 = n*2;
        auto m = access1::size(pgon2);
        auto m2 = m*2;

        auto nVisited = 0;
       do 
       {
          auto a1 = (a + n - 1) % n;
          auto b1 = (b + m - 1) % m;

          vector_t A = get_point1(a) - get_point1(a1);
          vector_t B = get_point2(b) - get_point2(b1);

          auto ABOrientation = get_orientation( A, B, cmp );
          auto aHB = get_orientation( get_point2(b1), get_point2(b), get_point1(a), cmp );
          auto bHA = get_orientation( get_point1(a1), get_point1(a), get_point2(b), cmp );

          point_type xpoints[2];
          auto iType = segment_segment_intersection( get_point1(a1), get_point1(a), get_point2(b1), get_point2(b), xpoints, cmp );
          if ( iType == e_crossing || iType == e_endpoint ) 
          {
             visitor(xpoints[0]);
             ++nVisited;
             
             if ( inflag == Unknown && nVisited == 1 ) 
             {
                aa = ba = 0;
             }
             if(aHB == oriented_left)
                inflag = Pin;
             else if(bHA == oriented_left)
                inflag = Qin;
          } 
          else if ( iType == e_overlapping && dot_product( A, B ) < area_t{} )
          {
             //++nVisited;
              visitor(xPoints[0]);
              visitor(xPoints[1]);
              return polygon_intersection_type::face;
          }

          if (ABOrientation == oriented_collinear && aHB == oriented_left && bHA == oriented_right )
              return polygon_intersection_type::none;
          else if ( ABOrientation == oriented_collinear && aHB == oriented_collinear && bHA == oriented_collinear )
          {
            if ( inflag == Pin )
                advance( b, ba, m );
            else
                advance( a, aa, n );
          }
          else if ( cross != oriented_right ) 
          {
             if ( bHA == oriented_left )
             {
                 if(inflag == pgon1_in)
                 {
                     ++nVisited;
                     visitor(get_point1(a));
                 }
                 advance( a, aa, n );
             }
             else
             {
                 if(inflag == pgon2_in)
                 {
                     ++nVisited;
                     visitor(get_point2(b));
                 }
                 advance( b, ba, m );
             }
          }
          else
          {
             if ( aHB == oriented_left )
             {
                 if( inflag == pgon2_in )
                 {
                     ++nVisited;
                     visitor(get_point2(b));
                 }
                advance( b, ba, m );
             }
             else
             {
                 if( inflag == pgon1_in )
                 {
                     ++nVisited;
                     visitor(get_point1(a));
                 }
                 advance( a, aa, n );
             }
          }
       } while( (aa < n || ba < m) && aa < n2 && ba < m2 );

       if( inflag == unknown || nVisited == 0 )
           return polygon_intersection_type::none;
       else if(nVisited > 1)
            return polygon_intersection_type::overlapping;

       //! If here then intersects at 1 point.
       return polygon_intersection_type::vertex;
    }

}//! namespace geometrix;

#endif//! GEOMETRIX_CONVEX_POLYGON_POLYGON_INTERSECTION_HPP

