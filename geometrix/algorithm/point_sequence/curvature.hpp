//
//! Copyright © 2008-2016
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_ALGORITHM_POINTSEQUENCE_CURVATURE_HPP
#define GEOMETRIX_ALGORITHM_POINTSEQUENCE_CURVATURE_HPP
#pragma once

#include <geometrix/primitive/point_sequence_traits.hpp>
#include <geometrix/utility/utilities.hpp>
#include <geometrix/primitive/point.hpp>
#include <geometrix/tensor/vector.hpp>
#include <geometrix/algorithm/euclidean_distance.hpp>
#include <algorithm>
#include <tuple>
#include <boost/limits.hpp>

namespace geometrix {

    namespace result_of {
        template <typename Polyline>
        struct polyline_signed_curvature_at_index
        {
            private:
            using access = point_sequence_traits<Polyline>;
            using point_t = typename point_sequence_traits<Polyline>::point_type;
            using length_t = typename geometric_traits<point_t>::arithmetic_type;
        public:
            using type = decltype(1.0 / std::declval<length_t>());
        };

		template <typename Polyline>
		struct polyline_total_curvature
		{
		private:
			using access = point_sequence_traits<Polyline>;
			using point_t = typename point_sequence_traits<Polyline>::point_type;
			using length_t = typename geometric_traits<point_t>::arithmetic_type;
		public:
			using type = decltype(std::declval<length_t>() / std::declval<length_t>());
		};

		template <typename Polyline>
		using polyline_total_absolute_curvature = polyline_total_curvature<Polyline>;

		template <typename Polyline>
		using polyline_total_curvature_over_index = polyline_total_curvature<Polyline>;
    }//! namespace result_of;
    
    template <typename Polyline>
    inline typename result_of::polyline_signed_curvature_at_index<Polyline>::type polyline_signed_curvature_at_index(const Polyline& poly, std::size_t j)
    {
        typedef point_sequence_traits<Polyline> access;
        typedef typename point_sequence_traits<Polyline>::point_type point_type;
        typedef typename geometric_traits<point_type>::arithmetic_type length_t;
		using vector_t = geometrix::vector<length_t, dimension_of<point_type>::value>;
        using curvature_t = decltype(1.0 / std::declval<length_t>());
        using std::sqrt;
        
        if( j > 0 && (j+1) < access::size(poly) )
        {
            std::size_t i = j - 1;
            std::size_t k = j + 1;
            auto dij = point_point_distance_sqrd(access::get_point(poly, j), access::get_point(poly, i));
            auto djk = point_point_distance_sqrd(access::get_point(poly, j), access::get_point(poly, k));
            auto dik = point_point_distance_sqrd(access::get_point(poly, i), access::get_point(poly, k));
                
            vector_t v1 = (access::get_point(poly, j) - access::get_point(poly, i));
            vector_t v2 = (access::get_point(poly, k) - access::get_point(poly, j));
                
            curvature_t kappa = 2.0 * exterior_product_area(v1, v2) / sqrt(dij * djk * dik);
            return kappa;
        }
        else 
            return constants::zero<curvature_t>();
    }

	//! Get the estimated total curvature integrated over indices j-1, j, j+1.
	template <typename Polyline>
	inline typename result_of::polyline_total_curvature_over_index<Polyline>::type polyline_total_curvature_over_index(const Polyline& poly, std::size_t j)
	{
		typedef point_sequence_traits<Polyline> access;
		typedef typename point_sequence_traits<Polyline>::point_type point_type;
		typedef typename geometric_traits<point_type>::arithmetic_type length_t;
		using vector_t = geometrix::vector<length_t, dimension_of<point_type>::value>;
		using curvature_t = decltype(1.0 / std::declval<length_t>());
		using std::sqrt;
		using std::abs;

		if (j > 0 && (j + 1) < access::size(poly))
		{
			std::size_t i = j - 1;
			std::size_t k = j + 1;
			auto dij = point_point_distance_sqrd(access::get_point(poly, j), access::get_point(poly, i));
			auto djk = point_point_distance_sqrd(access::get_point(poly, j), access::get_point(poly, k));
			auto dik = point_point_distance_sqrd(access::get_point(poly, i), access::get_point(poly, k));

			vector_t v1 = (access::get_point(poly, j) - access::get_point(poly, i));
			vector_t v2 = (access::get_point(poly, k) - access::get_point(poly, j));

			curvature_t kappa = 2.0 * abs(exterior_product_area(v1, v2)) / sqrt(dij * djk * dik);
			return (0.5 * (sqrt(dij) + sqrt(djk)) * kappa);
		}
		else
			return constants::zero<decltype(std::declval<length_t>() * std::declval<curvature_t>())>();
	}

    template <typename Polyline>
    inline typename result_of::polyline_total_curvature<Polyline>::type polyline_total_curvature( const Polyline& poly )
    {
        using std::sqrt;
        
        typedef point_sequence_traits<Polyline> access;
        typedef typename point_sequence_traits<Polyline>::point_type point_type;
        typedef typename geometric_traits<point_type>::arithmetic_type length_t;
		using vector_t = geometrix::vector<length_t, dimension_of<point_type>::value>;
        using curvature_t = decltype(1.0 / std::declval<length_t>());
		using dimensionless_t = typename result_of::polyline_total_curvature<Polyline>::type;
		auto sum = constants::zero<dimensionless_t>();
        std::size_t size = access::size( poly );
        
        if( size > 2 )
        {
            std::size_t i = 0, j = 1;
            auto dij = point_point_distance_sqrd(access::get_point(poly, j), access::get_point(poly, i));
            vector_t v1 = (access::get_point(poly, j) - access::get_point(poly, i));
            for( std::size_t k = 2; k < size; ++i, ++j, ++k )
            {
                auto djk = point_point_distance_sqrd(access::get_point(poly, j), access::get_point(poly, k));
                auto dik = point_point_distance_sqrd(access::get_point(poly, i), access::get_point(poly, k));
                vector_t v2 = (access::get_point(poly, k) - access::get_point(poly, j));
                
                curvature_t curvature = 2.0 * exterior_product_area(v1, v2) / sqrt(dij * djk * dik);
				sum += (0.5 * (sqrt(dij) + sqrt(djk)) * curvature);

                dij = djk;
                assign(v1, v2);
            }
        }
        return sum;
    }

	template <typename Polyline>
	inline typename result_of::polyline_total_curvature<Polyline>::type polyline_total_absolute_curvature(const Polyline& poly)
	{
		using std::sqrt;
		using std::abs;

		typedef point_sequence_traits<Polyline> access;
		typedef typename point_sequence_traits<Polyline>::point_type point_type;
		typedef typename geometric_traits<point_type>::arithmetic_type length_t;
		using vector_t = geometrix::vector<length_t, dimension_of<point_type>::value>;
		using curvature_t = decltype(1.0 / std::declval<length_t>());
		using dimensionless_t = typename result_of::polyline_total_absolute_curvature<Polyline>::type;
		auto sum = constants::zero<dimensionless_t>();
		std::size_t size = access::size(poly);

		if (size > 2)
		{
			std::size_t i = 0, j = 1;
			auto dij = point_point_distance_sqrd(access::get_point(poly, j), access::get_point(poly, i));
			vector_t v1 = (access::get_point(poly, j) - access::get_point(poly, i));
			for (std::size_t k = 2; k < size; ++i, ++j, ++k)
			{
				auto djk = point_point_distance_sqrd(access::get_point(poly, j), access::get_point(poly, k));
				auto dik = point_point_distance_sqrd(access::get_point(poly, i), access::get_point(poly, k));
				vector_t v2 = (access::get_point(poly, k) - access::get_point(poly, j));

				curvature_t curvature = 2.0 * abs(exterior_product_area(v1, v2)) / sqrt(dij * djk * dik);
				sum += (0.5 * (sqrt(dij) + sqrt(djk)) * curvature);

				dij = djk;
				assign(v1, v2);
			}
		}
		return sum;
	}

	template <typename PointSequence>
	inline std::vector<std::size_t> inflection_points(const PointSequence& poly)
	{
		typedef point_sequence_traits<PointSequence> access;
		std::vector<std::size_t> results;

		int cSign = sign(polyline_signed_curvature_at_index(poly, 0));
		auto size = access::size(poly);
		for (std::size_t i = 1; i < size; ++i)
		{
			auto k = polyline_signed_curvature_at_index(poly, i);
			int nSign = sign(k);
			if (nSign != cSign)
			{
				results.push_back(i);
				cSign = nSign;
			}
		}

		return results;
	}

	template <typename Polyline>
	inline std::vector<Polyline> split_polyline_by_inflection_points(const Polyline& poly)
	{
		std::vector<Polyline> results;
		using access = point_sequence_traits<Polyline>;
		Polyline current{ access::get_point(poly,0) };
		int cSign = sign(polyline_signed_curvature_at_index(poly, 0));
		auto size = access::size(poly);
		for (std::size_t i = 1; i < size; ++i) 
		{
			auto k = polyline_signed_curvature_at_index(poly, i);
			int nSign = sign(k);
			current.push_back(access::get_point(poly, i));
			if (nSign != cSign) 
			{
				if (access::size(current) > 1)
				{
					results.push_back(std::move(current));
					current.push_back(access::get_point(poly, i));
				}
				cSign = nSign;
			}
		}

		if (access::size(current) > 1)
			results.push_back(std::move(current));
		
		return results;
	}

	template <typename Polyline, typename AggregateCurvature>
	inline std::vector<Polyline> split_by_total_curvature(const Polyline& poly, const AggregateCurvature& ksplit)
	{
		std::vector<Polyline> results;
		using access = point_sequence_traits<Polyline>;
		auto ktot = constants::zero<AggregateCurvature>();

		Polyline current{ access::get_point(poly, 0) };
		auto size = access::size(poly);
		for (std::size_t i = 1; i < size; ++i)
		{
			using std::abs;
			
			auto k = polyline_total_curvature_over_index(poly, i);
			ktot += k;
			current.push_back(access::get_point(poly, i));
			if (ktot > ksplit)
			{
				if (access::size(current) > 1)
				{
					results.push_back(std::move(current));
					current.push_back(access::get_point(poly, i));
				}
				ktot = constants::zero<AggregateCurvature>();
			}
		}

		if (access::size(current) > 1)
			results.push_back(std::move(current));

		return results;
	}
    
}//! namespace geometrix

#endif//GEOMETRIX_ALGORITHM_POINTSEQUENCE_CURVATURE_HPP
