//
//! Copyright © 2018
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_SEPARATING_AXIS_COMMON_HPP
#define GEOMETRIX_SEPARATING_AXIS_COMMON_HPP

#include <geometrix/primitive/point_traits.hpp>
#include <geometrix/primitive/point_sequence_traits.hpp>
#include <geometrix/tensor/vector.hpp>
#include <geometrix/numeric/constants.hpp>
#include <geometrix/algebra/expression.hpp>
#include <geometrix/arithmetic/vector/perp.hpp>
#include <geometrix/algebra/dot_product.hpp>

namespace geometrix {
	
	namespace detail_separating_axis {

		inline std::size_t get_middle_index(std::size_t i0, std::size_t i1, std::size_t n)
		{
			if(i0 < i1)
				return (i0 + i1) / 2;
			else
				return ((i0 + i1 + n) / 2) % n;
		}

		template <typename Polygon, typename Vector, typename NumberComparisonPolicy>
		inline std::size_t get_extreme_index(const Polygon& c, const Vector& d, const NumberComparisonPolicy& cmp)
		{
			using access = point_sequence_traits<Polygon>;
			using point_type = typename access::point_type;
			using length_t = typename arithmetic_type_of<point_type>::type;
			using vector_t = vector<length_t, dimension_of<point_type>::value>;

			std::size_t i0 = 0;
			std::size_t i1 = 0;
			while(true)
			{
				std::size_t mid = get_middle_index(i0, i1, access::size(c));
				std::size_t next = (mid + 1) % access::size(c);
				vector_t e = access::get_point(c, next) - access::get_point(c, mid);
				auto de = dot_product(d, e);
				if(cmp.greater_than(de, constants::zero<decltype(de)>()))
				{
					if(mid != i0)
						i0 = mid;
					else
						return i1;
				}
				else
				{
					std::size_t prev = (mid + access::size(c) - 1) % access::size(c);
					e = access::get_point(c, mid) - access::get_point(c, prev);
					de = dot_product(d, e);
					if(cmp.less_than(de, constants::zero<decltype(de)>()))
						i1 = mid;
					else
						return mid;
				}
			}
		}

		template <typename Polygon, typename Vector, typename Length, typename NumberComparisonPolicy>
        inline void compute_interval(const Polygon& c, const Vector& d, Length& min, Length& max, const NumberComparisonPolicy& cmp)
        {
            using access = point_sequence_traits<Polygon>;
            min = dot_product(d, access::get_point(c, get_extreme_index(c, -d, cmp))); 
            max = dot_product(d, access::get_point(c, get_extreme_index(c, d, cmp))); 
        }

        template <typename Length>
        struct projection_info
        {
            Length min, max;
            std::size_t index[2];
            bool isUnique[2];
        };

        //! Collision info variant.
        template <typename Time, typename Speed, typename Length>
        inline bool no_intersect(const Time& tmax, const Speed& speed, const Length& min1, const Length& max1, const Length& min2, const Length& max2, int& side, Time& tfirst, Time& tlast)
        {
            auto inv_speed = 1.0 / speed;
			auto t = Time{};
            if(max2 < min1)
            {
                //! Interval from p1 on left of interval p2.
                if(speed <= constants::zero<Speed>())//! moving away
                    return true;

                t = (min1 - max2) * inv_speed;
                if(t > tfirst)
                {
                    tfirst = t;
                    side = -1;
                }
                if(tfirst > tmax)
                    return true;

                t = (max1 - min2) * inv_speed;
                if(t < tlast)
                    tlast = t;
                if(tfirst > tlast)
                    return true;
            }
            else if(max1 < min2)
            {
                //! Interval p1 is right of interval p2.
                if(speed >= constants::zero<Speed>())
                    return true;

                t = (max1 - min2) * inv_speed;
                if(t > tfirst)
                {
                    tfirst = t;
                    side = 1;
                }
                if(tfirst > tmax)
                    return true;

                t = (min1 - max2) * inv_speed;
                if(t < tlast)
                    tlast = t;
                if(tfirst > tlast)
                    return true;
            }
            else
            {
                //! intervals overlap.
                if(speed > constants::zero<Speed>())
                {
                    t = (max1 - min2) * inv_speed;
                    if(t < tlast)
                        tlast = t;
                    if(tfirst > tlast)
                        return true;
                }
                else if(speed < constants::zero<Speed>())
                {
                    t = (min1 - max2) * inv_speed;
                    if(t < tlast)
                        tlast = t;
                    if(tfirst > tlast)
                        return true;
                }
            }

            return false;
        }
	}//! namespace detail;

}//namespace geometrix;

#endif //! GEOMETRIX_SEPARATING_AXIS_COMMON_HPP
