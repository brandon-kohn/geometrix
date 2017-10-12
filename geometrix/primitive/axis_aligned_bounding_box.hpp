//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_AXIS_ALIGNED_BOUNDING_BOX_HPP
#define GEOMETRIX_AXIS_ALIGNED_BOUNDING_BOX_HPP

#include <geometrix/primitive/point.hpp>
#include <geometrix/tensor/vector.hpp>
#include <geometrix/algorithm/segment_mid_point.hpp>
#include <geometrix/numeric/number_comparison_policy.hpp>
#include <geometrix/primitive/point_sequence_utilities.hpp>
#include <array>
#include <tuple>

namespace geometrix {
    
    template <typename Point>
    class axis_aligned_bounding_box;

    //! Return a tuple in (xlow, xhi, ylo, yhi) format.
    namespace result_of {
        template <typename T>
        struct to_tuple : expand_to_tuple<typename arithmetic_type_of<T>::type, 2 * dimension_of<T>::value> {};
    }//! namespace result_of

    //! \brief a class to define a bounding_range in N dimensional space.
    //! Constructs from two points in space which define the min bound and max bound of a(in 2D the lower left and upper right corner of a square are these.)
    namespace bounding_box {
        namespace detail {

            template <typename NumericType, std::size_t Dimension, typename NumberComparisonPolicy>
            struct numeric_sequence_bounds
            {
                typedef std::array< NumericType, Dimension > numeric_array;

                numeric_sequence_bounds(numeric_array& low, numeric_array& high, const NumberComparisonPolicy& compare)
                    : m_low(low)
                    , m_high(high)
                    , m_compare(compare)
                    , m_it(0)
                {}

                template <typename T>
                void operator() (const T& n)
                {
                    if (m_compare.less_than(n, m_low[m_it]))
                        m_low[m_it] = n;
                    if (m_compare.greater_than(n, m_high[m_it]))
                        m_high[m_it] = n;

                    m_it = (m_it + 1) % Dimension;
                }

                NumberComparisonPolicy m_compare;
                std::size_t    m_it;
                numeric_array& m_low;
                numeric_array& m_high;

            };

            template <std::size_t D>
            struct dimension_processor
            {
                template <typename Point, typename NumberComparisonPolicy>
                static bool compare(const Point& n, const Point& low, const Point& high, const NumberComparisonPolicy& nCompare, typename boost::enable_if<is_point<Point>>::type* = 0)
                {
                    if (nCompare.less_than(get<D>(n), get<D>(low)) || nCompare.greater_than(get<D>(n), get<D>(high)))
                        return false;
                    return dimension_processor<D - 1>::compare(n, low, high, nCompare);
                }

                template <typename Point1, typename Point2, typename NumberComparisonPolicy>
                static bool compare(const axis_aligned_bounding_box<Point1>& a, const axis_aligned_bounding_box<Point2>& b, const NumberComparisonPolicy& nCompare)
                {
                    if (nCompare.less_than(get<D>(a.get_upper_bound()), get<D>(b.get_lower_bound())) || nCompare.greater_than(get<D>(a.get_lower_bound()), get<D>(b.get_upper_bound())))
                        return false;
                    return dimension_processor<D - 1>::compare(a, b, nCompare);
                }
            };

            template <>
            struct dimension_processor<0>
            {
                template <typename Point, typename NumberComparisonPolicy>
                static bool compare(const Point& n, const Point& low, const Point& high, const NumberComparisonPolicy& nCompare, typename boost::enable_if<is_point<Point>>::type* = 0)
                {
                    if (nCompare.less_than(get<0>(n), get<0>(low)) || nCompare.greater_than(get<0>(n), get<0>(high)))
                        return false;
                    else
                        return true;
                }

                template <typename Point1, typename Point2, typename NumberComparisonPolicy>
                static bool compare(const axis_aligned_bounding_box<Point1>& a, const axis_aligned_bounding_box<Point2>& b, const NumberComparisonPolicy& nCompare)
                {
                    return !(nCompare.less_than(get<0>(a.get_upper_bound()), get<0>(b.get_lower_bound())) || nCompare.greater_than(get<0>(a.get_lower_bound()), get<0>(b.get_upper_bound())));
                }
            };

            template <typename Point, typename NumberComparisonPolicy>
            inline bool compare(const Point& n, const Point& low, const Point& high, const NumberComparisonPolicy& nCompare, typename boost::enable_if<is_point<Point>>::type* = 0)
            {
                return dimension_processor<geometric_traits<Point>::dimension_type::value - 1>::compare(n, low, high, nCompare);
            }

            template <typename Point1, typename Point2, typename NumberComparisonPolicy>
            inline bool compare(const axis_aligned_bounding_box<Point1>& a, const axis_aligned_bounding_box<Point2>& b, const NumberComparisonPolicy& nCompare)
            {
                return dimension_processor<geometric_traits<Point1>::dimension_type::value - 1>::compare(a, b, nCompare);
            }

            template <typename Point>
            inline typename result_of::to_tuple<Point>::type to_tuple(const Point& lo, const Point& hi, typename boost::enable_if_c<dimension_of<Point>::value == 2>::type* = nullptr)
            {
                return std::make_tuple(get<0>(lo), get<0>(hi), get<1>(lo), get<1>(hi));
            }

            //! Return a tuple in (xlow, xhi, ylo, yhi) format.
            template <typename Point>
            inline typename result_of::to_tuple<Point>::type to_tuple(const Point& lo, const Point& hi, typename boost::enable_if_c<dimension_of<Point>::value == 3>::type* = nullptr)
            {
                return std::make_tuple(get<0>(lo), get<0>(hi), get<1>(lo), get<1>(hi), get<2>(lo), get<2>(hi));
            }
			
            template <typename Point, typename Tuple>
            inline Point to_low_point(const Tuple& b, typename boost::enable_if_c<dimension_of<Point>::value == 2>::type* = nullptr)
            {
                return construct<Point>(std::get<e_xmin>(bounds), std::get<e_ymin>(bounds));
            };

			template <typename Point, typename Tuple>
			inline Point to_low_point(const Tuple& b, typename boost::enable_if_c<dimension_of<Point>::value == 3>::type* = nullptr)
			{
				return construct<Point>(std::get<e_xmin>(bounds), std::get<e_ymin>(bounds), std::get<e_zmin>(bounds));
			};

			template <typename Point, typename Tuple>
			inline Point to_hi_point(const Tuple& b, typename boost::enable_if_c<dimension_of<Point>::value == 2>::type* = nullptr)
			{
				return construct<Point>(std::get<e_xmax>(bounds), std::get<e_ymax>(bounds));
			};

			template <typename Point, typename Tuple>
			inline Point to_hi_point(const Tuple& b, typename boost::enable_if_c<dimension_of<Point>::value == 3>::type* = nullptr)
			{
				return construct<Point>(std::get<e_xmax>(bounds), std::get<e_ymax>(bounds), std:get<e_zmax>(bounds));
			};
            
        }//! namespace detail;
    }//namespace bounding_box::detail;
        
    template <typename Point>
    class axis_aligned_bounding_box
    {
        using length_type = typename arithmetic_type_of<Point>::type;
    public:

        typedef Point point_type;

        axis_aligned_bounding_box(const point_type& low, const point_type& high)
            : m_low(low)
            , m_high(high)
        {}
		
        template <typename... Ts>
        axis_aligned_bounding_box(const std::tuple<Ts...>& bounds)
            : m_low( bounding_box::detail::to_low_point<Point>(bounds) )
            , m_high( bounding_box::detail::to_hi_point<Point>(bounds))
        {
            static_assert(dimension_of<Point>::value * 2 == sizeof...(Ts), "Tuple dimensions should be twice the point dimensions.");
        }

		template <typename... Ts>
		axis_aligned_bounding_box& operator =(const std::tuple<Ts...>& bounds)
		{
			static_assert(dimension_of<Point>::value * 2 == sizeof...(Ts), "Tuple dimensions should be twice the point dimensions.");
			m_low = bounding_box::detail::to_low_point<Point>(bounds);
			m_high = bounding_box::detail::to_hi_point<Point>(bounds);
			return *this;
		}
		        
        template <typename T, typename NumberComparisonPolicy>
        bool intersects(const T& t, const NumberComparisonPolicy& compare, typename boost::enable_if< is_numeric_sequence<T> >::type* = 0) const
        {
            return bounding_box::detail::compare(t, m_low, m_high, compare);
        }

        template <typename T>
        bool intersects(const T& t, typename boost::enable_if< is_numeric_sequence<T> >::type* = 0) const
        {
            return bounding_box::detail::compare(t, m_low, m_high, direct_comparison_policy());
        }
        
        template <typename Sequence, typename NumberComparisonPolicy>
        bool intersects(const axis_aligned_bounding_box<Sequence>& range, const NumberComparisonPolicy& compare) const
        {
            return bounding_box::detail::compare(*this, range, compare);
        }

        //! Check if another range intersects this range.
        template <typename Sequence>
        bool intersects(const axis_aligned_bounding_box<Sequence>& range) const
        {
            return bounding_box::detail::compare(*this, range, direct_comparison_policy());
        }

        //! Check if another range is wholly contained by this range.
        template <typename Sequence, typename NumberComparisonPolicy>
        bool contains(const axis_aligned_bounding_box<Sequence>& range, const NumberComparisonPolicy& compare) const
        {
            bool upperIntersects = bounding_box::detail::compare(range.get_upper_bound(), m_low, m_high, compare);
            bool lowerIntersects = bounding_box::detail::compare(range.get_lower_bound(), m_low, m_high, compare);
            return (upperIntersects && lowerIntersects);
        }

        template <typename Sequence>
        bool contains(const axis_aligned_bounding_box<Sequence>& range) const
        {
            bool upperIntersects = bounding_box::detail::compare(range.get_upper_bound(), m_low, m_high, direct_comparison_policy());
            bool lowerIntersects = bounding_box::detail::compare(range.get_lower_bound(), m_low, m_high, direct_comparison_policy());
            return (upperIntersects && lowerIntersects);
        }

        const point_type& get_lower_bound() const { return m_low; }
        const point_type& get_upper_bound() const { return m_high; }

        std::size_t size() const { return 4; }

        //! Convenience border point accessor.
        point_type operator[](std::size_t i) const
        {
            GEOMETRIX_ASSERT(i < 4);
            switch (i)
            {
            case 0:
                return m_low;
            case 1:
                return construct<point_type>(get<0>(m_high), get<1>(m_low));
            case 2:
                return m_high;
            case 3:
                return construct<point_type>(get<0>(m_low), get<1>(m_high));
            };

            throw std::out_of_range("box has 4 points");
        }
    
		inline typename result_of::to_tuple<Point>::type to_tuple() const
        {
            return bounding_box::detail::to_tuple(get_lower_bound(), get_upper_bound());
        }

        template <typename... Ts>
        inline void expand(const std::tuple<Ts...>& b2)
        {
            auto b1 = to_tuple();
            update_bound(b1, b2);
            *this = b1;
        }
        
        template <typename U, typename Tuple>
        inline void expand(const axis_aligned_bounding_box<U>& aabb)
        {
            expand(aabb.to_tuple());
        }

    private:

        Point m_low;
        Point m_high;

    };

    template <typename T>
    struct geometric_traits<axis_aligned_bounding_box<T>, void>
    {
        using is_aabb = void;
        using point_type = T;
    };

    //! Construct a range from a point sequence by finding the min/max values on each dimension.
    template <typename Point, typename PointSequence, typename NumberComparisonPolicy>
    inline axis_aligned_bounding_box<Point> make_aabb(const PointSequence& pointSequence, const NumberComparisonPolicy& compare, typename std::enable_if<is_point_sequence<PointSequence>::value && dimension_of<PointSequence>::value==2>::type* = nullptr)
    {
        typedef Point point_type;
        auto bounds = get_bounds(pointSequence, compare);

        auto lo = construct<point_type>(std::get<e_xmin>(bounds), std::get<e_ymin>(bounds));
        auto hi = construct<point_type>(std::get<e_xmax>(bounds), std::get<e_ymax>(bounds));

        return axis_aligned_bounding_box<point_type>(lo, hi);
    }
    
    template <typename Point, typename PointSequence, typename NumberComparisonPolicy>
    inline axis_aligned_bounding_box<Point> make_aabb(const PointSequence& pointSequence, const NumberComparisonPolicy& compare, typename std::enable_if<is_point_sequence<PointSequence>::value && dimension_of<PointSequence>::value==3>::type* = nullptr)
    {
        typedef Point point_type;
        auto bounds = get_bounds(pointSequence, compare);

        auto lo = construct<point_type>(std::get<e_xmin>(bounds), std::get<e_ymin>(bounds), std::get<e_zmin>(bounds));
        auto hi = construct<point_type>(std::get<e_xmax>(bounds), std::get<e_ymax>(bounds), std::get<e_zmax>(bounds));

        return axis_aligned_bounding_box<point_type>(lo, hi);
    }

    template <typename Point, typename PointSequence>
    inline axis_aligned_bounding_box<Point> make_aabb(const PointSequence& pointSequence, typename boost::enable_if<is_point_sequence<PointSequence>>::type* = nullptr)
    {
        return make_aabb<Point>(pointSequence, direct_comparison_policy());
    }

    template <typename Point, typename Segment, typename NumberComparisonPolicy>
    inline axis_aligned_bounding_box<Point> make_aabb(const Segment& seg, const NumberComparisonPolicy& compare, typename boost::enable_if<is_segment<Segment>>::type* = nullptr)
    {
        typedef Point point_type;
        typedef typename geometric_traits< point_type >::arithmetic_type        coordinate_type;
        typedef boost::array< coordinate_type, dimension_of< point_type >::type::value > array_type;
        array_type low = construct<array_type>(get_start(seg));
        array_type high = low;

        //! check bounds against each point in the sequence updating the limits on each dimension.
        {
            auto p = get_start(seg);
            boost::fusion::for_each(p,
                bounding_box::detail::numeric_sequence_bounds
                <
                    coordinate_type
                  , dimension_of< point_type >::type::value
                  , NumberComparisonPolicy
                >(low, high, compare));
        }

        {
            auto p = get_end(seg);
            boost::fusion::for_each(p,
                bounding_box::detail::numeric_sequence_bounds
                <
                    coordinate_type
                  , dimension_of< point_type >::type::value
                  , NumberComparisonPolicy
                >(low, high, compare));
        }

        return axis_aligned_bounding_box<point_type>( construct<point_type>(low),  construct<point_type>(high));
    }

    template <typename Point, typename Segment>
    inline axis_aligned_bounding_box<Point> make_aabb(const Segment& seg, typename boost::enable_if<is_segment<Segment>>::type* = nullptr)
    {
        return make_aabb(seg, direct_comparison_policy());
    }

    template <typename Point, typename T>
    inline axis_aligned_bounding_box<Point> make_aabb(const std::tuple<T, T, T, T>& bounds)
    {
        static_assert(dimension_of<Point>::value == 2, "point dimension should be 2.");
        return axis_aligned_bounding_box<Point>{ construct<Point>(std::get<e_xmin>(bounds), std::get<e_ymin>(bounds)), construct<Point>(std::get<e_xmax>(bounds), std::get<e_ymax>(bounds)) };
    }
        
    template <typename Point, typename T>
    inline axis_aligned_bounding_box<Point> make_aabb(const std::tuple<T, T, T, T, T, T>& bounds)
    {
        static_assert(dimension_of<Point>::value == 3, "point dimension should be 3.");
        return axis_aligned_bounding_box<Point>{ construct<Point>(std::get<e_xmin>(bounds), std::get<e_ymin>(bounds), std::get<e_zmin>(bounds)), construct<Point>(std::get<e_xmax>(bounds), std::get<e_ymax>(bounds), std::get<e_zmax>(bounds)) };
    }
    
    typedef axis_aligned_bounding_box<point_double_2d> aabb_double_2d;

}//namespace geometrix;

#endif //GEOMETRIX_AXIS_ALIGNED_BOUNDING_BOX_HPP
