//
//! Copyright © 2008-2013
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_COHEN_SUTHERLAND_LINE_CLIPPING_HPP
#define GEOMETRIX_COHEN_SUTHERLAND_LINE_CLIPPING_HPP
#pragma once

#include <geometrix/numeric/number_comparison_policy.hpp>

namespace geometrix 
{
    //! From: http://en.wikipedia.org/wiki/Cohen%E2%80%93Sutherland#Example_C.2FC.2B.2B_implementation
    namespace detail 
    {
        struct out_classification
        {
            enum Enum
            {
                  inside = 0 // 0000
                , left = 1   // 0001
                , right = 2  // 0010
                , bottom = 4 // 0100
                , top = 8    // 1000
                , invalid = -1
            };

            out_classification(int v = invalid)
                : flags(v)
            {}
            
            operator int() const
            {
                return flags;
            }

            template <typename T>
            out_classification& operator =(T v)
            {
                flags = v;
                return *this;
            }

            friend out_classification& operator |=(out_classification& lhs, const out_classification& rhs)
            {
                lhs.flags |= rhs.flags;
                return lhs;
            }

			friend bool operator ==(const out_classification& lhs, const out_classification& rhs){ return lhs.flags == rhs.flags; }

        private:

            int flags;
        };

        // Compute the bit code for a point (x, y) using the clip rectangle
        // bounded diagonally by (xmin, ymin), and (xmax, ymax)
        template <typename Coordinate, typename NumberComparisonPolicy>
        detail::out_classification compute_out_classification( Coordinate x, Coordinate y
                                                             , Coordinate xmin, Coordinate xmax
                                                             , Coordinate ymin, Coordinate ymax, const NumberComparisonPolicy& cmp)
        {
            BOOST_CONCEPT_ASSERT((NumberComparisonPolicyConcept<NumberComparisonPolicy, Coordinate>));
            using namespace geometrix::detail;
            out_classification code = out_classification::inside;          // initialised as being inside of clip window

            if (cmp.less_than(x, xmin))           // to the left of clip window
                code |= out_classification::left;
            else if (cmp.greater_than(x, xmax))      // to the right of clip window
                code |= out_classification::right;
            if (cmp.less_than(y, ymin))           // below the clip window
                code |= out_classification::bottom;
            else if (cmp.greater_than(y, ymax))      // above the clip window
                code |= out_classification::top;

            return code;
        }
    }//! namespace detail;

    // Cohen–Sutherland clipping algorithm clips a line from
    // P0 = (x0, y0) to P1 = (x1, y1) against a rectangle with 
    // diagonal from (xmin, ymin) to (xmax, ymax).
    template <typename Coordinate, typename NumberComparisonPolicy>
    bool cohen_sutherland_line_clip( Coordinate& x0, Coordinate& y0, Coordinate& x1, Coordinate& y1
                                   , Coordinate xmin, Coordinate xmax, Coordinate ymin, Coordinate ymax, const NumberComparisonPolicy& cmp)
    {
        BOOST_CONCEPT_ASSERT((NumberComparisonPolicyConcept<NumberComparisonPolicy, Coordinate>));
        using namespace geometrix::detail;
        // compute out_classifications for P0, P1, and whatever point lies outside the clip rectangle
        out_classification out_classification0 = compute_out_classification(x0, y0, xmin, xmax, ymin, ymax, cmp);
        out_classification out_classification1 = compute_out_classification(x1, y1, xmin, xmax, ymin, ymax, cmp);
        bool accept = false;

        while (true) 
        {
            if (!(out_classification0 | out_classification1)) 
            {
                // Bitwise OR is 0. Trivially accept and get out of loop
                accept = true;
                break;
            } 
            else if (out_classification0 & out_classification1) 
            {
                // Bitwise AND is not 0. Trivially reject and get out of loop
                break;
            } 
            else 
            {
                // failed both tests, so calculate the line segment to clip
                // from an outside point to an intersection with clip edge
                Coordinate x = 0, y = 0;

                // At least one endpoint is outside the clip rectangle; pick it.
                out_classification out_classificationOut = out_classification0 ? out_classification0 : out_classification1;

                // Now find the intersection point;
                // use formulas y = y0 + slope * (x - x0), x = x0 + (1 / slope) * (y - y0)
                if (out_classificationOut & out_classification::top)
                {   
                    // point is above the clip rectangle
                    x = x0 + (x1 - x0) * (ymax - y0) / (y1 - y0);
                    y = ymax;
                } 
                else if (out_classificationOut & out_classification::bottom)
                {
                    // point is below the clip rectangle
                    x = x0 + (x1 - x0) * (ymin - y0) / (y1 - y0);
                    y = ymin;
                } 
                else if (out_classificationOut & out_classification::right)
                {
                    // point is to the right of clip rectangle
                    y = y0 + (y1 - y0) * (xmax - x0) / (x1 - x0);
                    x = xmax;
                }
                else if (out_classificationOut & out_classification::left) 
                {
                    // point is to the left of clip rectangle
                    y = y0 + (y1 - y0) * (xmin - x0) / (x1 - x0);
                    x = xmin;
                }

                // Now we move outside point to intersection point to clip
                // and get ready for next pass.
                if (out_classificationOut == out_classification0) 
                {
                    x0 = x;
                    y0 = y;
                    out_classification0 = compute_out_classification(x0, y0, xmin, xmax, ymin, ymax, cmp);
                }
                else 
                {
                    x1 = x;
                    y1 = y;
                    out_classification1 = compute_out_classification(x1, y1, xmin, xmax, ymin, ymax, cmp);
                }
            }
        }

        return accept;
    }

}//! namespace geometrix;

#endif//GEOMETRIX_COHEN_SUTHERLAND_LINE_CLIPPING_HPP
