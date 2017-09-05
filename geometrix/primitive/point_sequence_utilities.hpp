//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_POINT_SEQUENCE_UTILITIES_HPP
#define GEOMETRIX_POINT_SEQUENCE_UTILITIES_HPP
#pragma once

#include <geometrix/primitive/point_sequence_traits.hpp>
#include <geometrix/algebra/exterior_product.hpp>
#include <geometrix/tensor/numeric_sequence_compare.hpp>

#include <boost/foreach.hpp>
#include <boost/tuple/tuple.hpp>
#include <algorithm>

namespace geometrix {
    //! Function to calculate the centroid of a point sequence.
    template <typename PointSequence>
    inline typename point_sequence_traits<PointSequence>::point_type
    get_centroid( const PointSequence& polygon,
                  typename boost::enable_if
                  <
                    boost::is_same
                    <
                        typename geometric_traits
                        <
                            typename point_sequence_traits<PointSequence>::point_type
                        >::dimension_type,
                        dimension<2>
                    >
                  >::type* = 0 )
    {
        BOOST_CONCEPT_ASSERT((PointSequenceConcept< PointSequence >));

        using point_type = typename point_sequence_traits<PointSequence>::point_type;
        using length_t = typename geometric_traits<point_type>::arithmetic_type;
        using area_t = decltype(std::declval<length_t>() * std::declval<length_t>());
        using volume_t = decltype(std::declval<area_t>() * std::declval<length_t>());

        volume_t mX = constants::zero<volume_t>();
        volume_t mY = constants::zero<volume_t>();
        area_t area = constants::zero<area_t>();
        for( typename PointSequence::const_iterator it( point_sequence_traits< PointSequence >::begin( polygon ) ),
             nextIt( point_sequence_traits< PointSequence >::begin( polygon ) + 1),
             end( point_sequence_traits< PointSequence >::end( polygon ) );
             nextIt != end;
             ++it, ++nextIt )
        {
            const point_type& currentPoint = *it;
            const point_type& nextPoint = *nextIt;
            area_t ai = exterior_product_area( as_vector( currentPoint ), as_vector( nextPoint ) );
            area += ai;
            mX += ai * ( get<0>( currentPoint ) + get<0>( nextPoint ) );
            mY += ai * ( get<1>( currentPoint ) + get<1>( nextPoint ) );
        }

        {
            const point_type& backPoint = point_sequence_traits< PointSequence >::back( polygon );
            const point_type& frontPoint = point_sequence_traits< PointSequence >::front( polygon );

            area_t ai = exterior_product_area( as_vector( backPoint ), as_vector( frontPoint ) );
            area += ai;
            mX += ai * ( get<0>( backPoint ) + get<0>( frontPoint ) );
            mY += ai * ( get<1>( backPoint ) + get<1>( frontPoint ) );
        }

        area *= 0.5;
        auto q = 1. /( 6.0 * area);
        return construction_policy<point_type>::construct( mX * q, mY * q );
    }

    //! Function to calculate the signed area of a point sequence using the shoelace formula.
    namespace result_of {
        template <typename PointSequence>
        class get_signed_area
        {
            using length_t = typename geometric_traits<typename point_sequence_traits<PointSequence>::point_type>::arithmetic_type;

        public:

            using type = decltype(std::declval<length_t>() * std::declval<length_t>());

        };

        template <typename PointSequence>
        class get_area
        {
            using length_t = typename geometric_traits<typename point_sequence_traits<PointSequence>::point_type>::arithmetic_type;

        public:

            using type = decltype(std::declval<length_t>() * std::declval<length_t>());

        };
    }//! namespace result_of;
    template <typename PointSequence>
    inline typename result_of::get_signed_area<PointSequence>::type get_signed_area( const PointSequence& polygon,
                              typename boost::enable_if
                              <
                                boost::is_same
                                <
                                    typename geometric_traits
                                    <
                                        typename point_sequence_traits<PointSequence>::point_type
                                    >::dimension_type,
                                    dimension<2>
                                >
                              >::type* = 0 )
    {
        BOOST_CONCEPT_ASSERT((PointSequenceConcept< PointSequence >));
        typedef typename point_sequence_traits<PointSequence>::point_type point_type;
        using length_t = typename geometric_traits<point_type>::arithmetic_type;
        using area_t = decltype(std::declval<length_t>() * std::declval<length_t>());
        auto area = constants::zero<area_t>();
        for( typename PointSequence::const_iterator it( point_sequence_traits< PointSequence >::begin( polygon ) ),
             nextIt( point_sequence_traits< PointSequence >::begin( polygon ) + 1), end( point_sequence_traits< PointSequence >::end( polygon ) );
             nextIt != end; ++it, ++nextIt )
        {
            area += exterior_product_area( as_vector(*it), as_vector(*nextIt) );
        }

        area += exterior_product_area( as_vector( point_sequence_traits< PointSequence >::back( polygon ) ), as_vector( point_sequence_traits< PointSequence >::front( polygon ) ) );
        area *= 0.5;
        return area;
    }

    //! Function to calculate the area of a point sequence.
    template <typename PointSequence>
    inline typename result_of::get_area<PointSequence>::type get_area( const PointSequence& polygon,
                              typename boost::enable_if
                              <
                                boost::is_same
                                <
                                    typename geometric_traits
                                    <
                                        typename point_sequence_traits<PointSequence>::point_type
                                    >::dimension_type,
                                    dimension<2>
                                >
                              >::type* = 0 )
    {
        using std::abs;
        return abs(get_signed_area(polygon));
    }

    //! Function to calculate the min/max bounds of a point sequence.
    enum cartesian_bound
    {
        e_xmin = 0,
        e_xmax = 1,
        e_ymin = 2,
        e_ymax = 3,
        e_zmin = 4,
        e_zmax = 5
    };
    namespace detail {
        template<typename T>
        inline T get_highest(typename std::enable_if<!std::numeric_limits<T>::has_infinity>::type* = nullptr)
        {
            return (std::numeric_limits<T>::max)();
        }

        template <typename T>
        inline T get_highest(typename std::enable_if<std::numeric_limits<T>::has_infinity>::type* = nullptr)
        {
            return std::numeric_limits<T>::infinity();
        }
        template<typename T>
        inline T get_lowest(typename std::enable_if<!std::numeric_limits<T>::has_infinity>::type* = nullptr)
        {
            return std::numeric_limits<T>::lowest();
        }

        template <typename T>
        inline T get_lowest(typename std::enable_if<std::numeric_limits<T>::has_infinity>::type* = nullptr)
        {
            return constants::negative_infinity<T>();
        }
    }//! namespace detail;

    template <typename PointSequence, typename EnableIf=void>
    struct bounds_tuple
    {};
    
    template <typename PointSequence>
    struct bounds_tuple<PointSequence, typename std::enable_if<dimension_of<PointSequence>::value == 2>::type>
    {
        using point_type = typename point_sequence_traits<PointSequence>::point_type;
        using x_type = typename type_at<point_type, 0>::type;
        using y_type = typename type_at<point_type, 1>::type;
        using type = boost::tuple<x_type, y_type, x_type, y_type>;

        static type initial()
        {
            return type(detail::get_lowest<x_type>(), detail::get_highest<x_type>(), detail::get_lowest<y_type>(), detail::get_highest<y_type>());
        }
    };
    
    template <typename PointSequence>
    struct bounds_tuple<PointSequence, typename std::enable_if<dimension_of<PointSequence>::value == 3>::type>
    {
        using point_type = typename point_sequence_traits<PointSequence>::point_type;
        using x_type = typename type_at<point_type, 0>::type;
        using y_type = typename type_at<point_type, 1>::type;
        using z_type = typename type_at<point_type, 2>::type;
        using type = boost::tuple<x_type, y_type, z_type, x_type, y_type, z_type>;

        static type initial()
        {
            return type(detail::get_lowest<x_type>(), detail::get_highest<x_type>(), detail::get_lowest<y_type>(), detail::get_highest<y_type>(), detail::get_lowest<z_type>(), detail::get_highest<z_type>());
        }
    };

    template <typename PointSequence, typename NumberComparisonPolicy>
    inline typename bounds_tuple< PointSequence >::type
    get_bounds( const PointSequence& pointSequence,
                const NumberComparisonPolicy& compare,
                typename std::enable_if<dimension_of<PointSequence>::value == 2>::type* = nullptr )
    {
        auto bounds = bounds_tuple<PointSequence>::initial();

        for(const auto& p : pointSequence)
        {
            const auto& x = get<0>( p );
            if( compare.less_than( x, bounds.template get<e_xmin>() ) )
                bounds.template get<e_xmin>() = x;
            if( compare.greater_than( x, bounds.template get<e_xmax>() ) )
                bounds.template get<e_xmax>() = x;

            const auto& y = get<1>( p );
            if( compare.less_than( y, bounds.template get<e_ymin>() ) )
                bounds.template get<e_ymin>() = y;
            if( compare.greater_than( y, bounds.template get<e_ymax>() ) )
                bounds.template get<e_ymax>() = y;
        }

        return bounds;
    }
    
    template <typename PointSequence, typename NumberComparisonPolicy>
    inline typename bounds_tuple< PointSequence >::type
    get_bounds( const PointSequence& pointSequence,
                const NumberComparisonPolicy& compare,
                typename std::enable_if<dimension_of<PointSequence>::value == 3>::type* = nullptr )
    {
        auto bounds = bounds_tuple<PointSequence>::initial();

        for(const auto& p : pointSequence)
        {
            const auto& x = get<0>( p );
            if( compare.less_than( x, bounds.template get<e_xmin>() ) )
                bounds.template get<e_xmin>() = x;
            if( compare.greater_than( x, bounds.template get<e_xmax>() ) )
                bounds.template get<e_xmax>() = x;

            const auto& y = get<1>( p );
            if( compare.less_than( y, bounds.template get<e_ymin>() ) )
                bounds.template get<e_ymin>() = y;
            if( compare.greater_than( y, bounds.template get<e_ymax>() ) )
                bounds.template get<e_ymax>() = y;
            
            const auto& z = get<2>( p );
            if( compare.less_than( z, bounds.template get<e_zmin>() ) )
                bounds.template get<e_zmin>() = z;
            if( compare.greater_than( z, bounds.template get<e_zmax>() ) )
                bounds.template get<e_zmax>() = z;
        }

        return bounds;
    }

    template <typename Length>
    inline boost::tuple<Length, Length, Length, Length> update_bound(const boost::tuple<Length, Length, Length, Length>& b1, const boost::tuple<Length, Length, Length, Length>& b2)
    {
        auto minx = (std::min)(boost::get<e_xmin>(b1), boost::get<e_xmin>(b2));
        auto miny = (std::min)(boost::get<e_ymin>(b1), boost::get<e_ymin>(b2));
        auto maxx = (std::max)(boost::get<e_xmax>(b1), boost::get<e_xmax>(b2));
        auto maxy = (std::max)(boost::get<e_ymax>(b1), boost::get<e_ymax>(b2));
        return boost::make_tuple(minx, miny, maxx, maxy);
    }

    template <typename Polygon>
    inline std::size_t next_index( const Polygon& polygon, std::size_t index )
    {
        return (index + 1) < point_sequence_traits<Polygon>::size(polygon) ? index + 1 : 0;
    }

    template <typename Polygon>
    inline std::size_t prev_index( const Polygon& polygon, std::size_t index )
    {
        return (index > 0) ? index - 1 : (point_sequence_traits<Polygon>::size(polygon) - 1);
    }

    template <typename Polygon>
    inline bool is_point_concave(const Polygon& polygon, std::size_t index)
    {
        using point_t = typename point_sequence_traits<Polygon>::point_type;
        using length_t = typename geometric_traits<point_t>::arithmetic_type;
        using area_t = decltype(std::declval<length_t>() * std::declval<length_t>());
        const auto& point = point_sequence_traits<Polygon>::get_point( polygon, index );
        const auto& prevPoint = point_sequence_traits<Polygon>::get_point( polygon, prev_index( polygon, index ) );
        const auto& nextPoint = point_sequence_traits<Polygon>::get_point( polygon, next_index( polygon, index ) );
        return !(exterior_product_area( point - prevPoint, nextPoint - prevPoint ) > constants::zero<area_t>());
    }

    template <typename Segment, typename Polygon>
    inline std::vector< Segment > polygon_as_segment_range(const Polygon& p)
    {
        typedef point_sequence_traits< Polygon > access;
        std::vector< Segment > segments;
        std::size_t size = access::size(p);
        for (std::size_t i = 0, j = 1; i < size; ++i, j = (j + 1) % size) {
            Segment segment = construction_policy< Segment >::construct(access::get_point(p, i), access::get_point(p, j));
            segments.push_back(segment);
        }

        return std::move(segments);
    }

    template <typename Segment, typename Polyline>
    inline std::vector< Segment > polyline_as_segment_range(const Polyline& p)
    {
        typedef point_sequence_traits< Polyline > access;
        std::vector< Segment > segments;
        std::size_t size = access::size(p);
        for (std::size_t i = 0, j = 1; j < size; i = j++) {
            Segment segment = construction_policy< Segment >::construct(access::get_point(p, i), access::get_point(p, j));
            segments.push_back(segment);
        }

        return std::move(segments);
    }

    template <typename PointSequence1, typename PointSequence2, typename NumberComparisonPolicy>
    inline bool point_sequences_equal(const PointSequence1& p1, const PointSequence2& p2, const NumberComparisonPolicy& cmp)
    {
        typedef point_sequence_traits< PointSequence1 > access1;
        typedef point_sequence_traits< PointSequence2 > access2;
        std::size_t size1 = access1::size(p1);
        std::size_t size2 = access2::size(p2);
        if (size1 != size2)
            return false;
        for( std::size_t i = 0; i < size1; ++i )
        {
            if (!numeric_sequence_equals(access1::get_point(p1, i), access2::get_point(p2, i), cmp))
                return false;
        }

        return true;
    }

    template <typename Polygon1, typename Polygon2, typename NumberComparisonPolicy>
    inline bool polygons_equal(const Polygon1& p1, const Polygon2& p2, std::size_t offset, const NumberComparisonPolicy& cmp)
    {
        typedef point_sequence_traits< Polygon1 > access1;
        typedef point_sequence_traits< Polygon2 > access2;
        std::size_t size1 = access1::size(p1);
        std::size_t size2 = access2::size(p2);
        if (size1 != size2)
            return false;
        for (std::size_t i = 0; i < size1; ++i)
        {
            if (!numeric_sequence_equals(access1::get_point(p1, i), access2::get_point(p2, (i + offset) % size2), cmp))
                return false;
        }

        return true;
    }

    //! Given two polygons which have the same points and the same winding order, but a different start and endpoint in the loop, find the offset index of the second relative to the first.
    //! If the point is not found, returns std::numeric_limits<std::size_t>::max().
    template <typename Polygon1, typename Polygon2, typename NumberComparisonPolicy>
    inline std::size_t polygon_polygon_offset(const Polygon1& p1, const Polygon2& p2, const NumberComparisonPolicy& cmp)
    {
        typedef point_sequence_traits< Polygon1 > access1;
        typedef point_sequence_traits< Polygon2 > access2;
        std::size_t size = access2::size(p2);
        GEOMETRIX_ASSERT(size == access1::size(p1));
        for (std::size_t i = 0; i < size; ++i)
        {
            if (numeric_sequence_equals(access1::get_point(p1, 0), access2::get_point(p2, i), cmp))
                return i;
        }

        return (std::numeric_limits<std::size_t>::max)();
    }

    template <typename PointSequence>
    inline PointSequence reverse(const PointSequence& ps)
    {
        typedef point_sequence_traits<PointSequence> access;
        PointSequence poly(access::size(ps));
        std::reverse_copy(access::begin(ps), access::end(ps), access::begin(poly));
        return poly;
    }

}//namespace geometrix;

#endif //GEOMETRIX_POINT_SEQUENCE_UTILITIES_HPP
