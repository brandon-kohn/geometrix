//
//! Copyright © 2008-2016
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_ALGORITHM_POINTSEQUENCE_POLYLINEOFFSET_HPP
#define GEOMETRIX_ALGORITHM_POINTSEQUENCE_POLYLINEOFFSET_HPP
#pragma once

#include <geometrix/primitive/point_sequence_traits.hpp>
#include <geometrix/utility/utilities.hpp>
#include <geometrix/primitive/point.hpp>
#include <geometrix/tensor/vector.hpp>
#include <geometrix/arithmetic/vector/perp.hpp>
#include <geometrix/algorithm/line_intersection.hpp>
#include <geometrix/algorithm/intersection/polyline_polyline_intersection.hpp>
#include <geometrix/algorithm/point_sequence/self_intersection.hpp>

#include <boost/optional.hpp>

#include <algorithm>

namespace geometrix {

	namespace detail {

		enum xpoint_type
		{
			tip = 0
		  , pfip = 1
		  , nfip = 2
		};

		inline bool is_fip(xpoint_type t) { return t > 0; }

		template <typename Dimensionless, typename NumberComparisonPolicy>
		inline xpoint_type classify_xpoint_type(const Dimensionless& s, const NumberComparisonPolicy& cmp)
		{
			if (cmp.less_than(s, constants::zero<Dimensionless>()) )
				return nfip;
			if( cmp.greater_than(s, constants::one<Dimensionless>()))
				return pfip;
			
			return tip;
		}

		template <orientation_type Orientation, typename Polyline, typename Length, typename NumberComparisonPolicy>
		inline Polyline untrimmed_offset(const Polyline& poly, const Length& offset, const NumberComparisonPolicy& cmp)
		{
			using access = point_sequence_traits<Polyline>;
			using point_type = typename point_sequence_traits<Polyline>::point_type;
			using vector_type = vector<Length, dimension_of<point_type>::value>;
			using dimensionless_type = typename geometric_traits<point_type>::dimensionless_type;

			std::size_t size = access::size(poly);
			GEOMETRIX_ASSERT(size > 1);

			auto A = access::get_point(poly, 0);
			auto B = access::get_point(poly, 1);
			vector_type vNormalAB = offset * oriented_normal<Orientation>(normalize(B - A));
			auto pOffsetA = construct<point_type>(A + vNormalAB);
			auto pOffsetB = construct<point_type>(B + vNormalAB);
			Polyline result = { pOffsetA };

			Polyline rdebug = { A };

			if (size > 2)
			{
				for (std::size_t d = 2; d < size; ++d)
				{
					auto& C = B;
					auto D = access::get_point(poly, d);

					vector_type vNormalCD = offset * oriented_normal<Orientation>(normalize(D - C));
					auto pOffsetC = construct<point_type>(C + vNormalCD);
					auto pOffsetD = construct<point_type>(D + vNormalCD);

					point_type xPoint;
					dimensionless_type s, t;
					auto itype = line_line_intersect(pOffsetA, pOffsetB, pOffsetC, pOffsetD, xPoint, s, t, cmp);
					if (itype == e_crossing)
					{
						auto abtype = classify_xpoint_type(s, cmp);
						auto cdtype = classify_xpoint_type(t, cmp);
						if (abtype == tip && cdtype == tip)
							result.push_back(xPoint);
						else if (is_fip(abtype) && is_fip(cdtype))
						{
							if (abtype == pfip)
								result.push_back(xPoint);
							else
							{
								result.push_back(pOffsetB);
								result.push_back(pOffsetC);
							}
						}
						//! If p0 is a TIP for s^i(or s^i+1) and an FIP for sˆi+1
						//! (or sˆi), let the ending point of
						//!	sˆi, the starting point of sˆi+1, construct two
						//!	points, respectively and add them to result	sequently;
						else if ((abtype == tip && cdtype != tip) || (abtype != tip && cdtype == tip))
						{
							result.push_back(pOffsetB);
							result.push_back(pOffsetC);
						}
					}
					else if (itype == e_overlapping)
					{
						result.push_back(pOffsetB);
					}

					rdebug.push_back(B);

					A = C;
					B = D;
					pOffsetA = pOffsetC;
					pOffsetB = pOffsetD;

					if (d + 1 == size)
						result.push_back(pOffsetD);
				}
			}
			else
			{
				result.push_back(pOffsetB);
			}

			return result;
		}

		template <typename Polyline>
		struct point_closer_to_vertex_cmp
		{
			point_closer_to_vertex_cmp(const Polyline& p)
				: m_polyline(&p)
			{}

			template <typename Point>
			bool operator()(const std::tuple<std::size_t, Point>& lhs, const std::tuple<std::size_t, Point>& rhs) const
			{
				using access = point_sequence_traits<Polyline>;
				auto ilhs = std::get<0>(lhs);
				auto irhs = std::get<0>(rhs);

				if (ilhs != irhs)
					return ilhs < irhs;
				
				auto ipoint = access::get_point(*m_polyline, ilhs);
				return point_point_distance_sqrd(ipoint, std::get<1>(lhs)) < point_point_distance_sqrd(ipoint, std::get<1>(rhs));
			}

			const Polyline* m_polyline{ nullptr };
		};

		template <typename Polyline>
		using intersection_set = std::set<std::tuple<std::size_t, typename point_sequence_traits<Polyline>::point_type>, point_closer_to_vertex_cmp<Polyline>>;

		template <typename Polyline, typename NumberComparisonPolicy>
		inline intersection_set<Polyline> self_intersection_set(const Polyline& poly, const NumberComparisonPolicy& cmp)
		{
			intersection_set<Polyline> iset(poly);
			using point_t = typename point_sequence_traits<Polyline>::point_type;
			auto visitor = [&](std::size_t i, std::size_t j, intersection_type iType, const point_t& xp1, const point_t& xp2) 
			{
				GEOMETRIX_ASSERT(i != j);
				iset.insert(std::make_tuple(i, xp1));
				iset.insert(std::make_tuple(j, xp1));
				if (iType == e_overlapping)
				{
					iset.insert(std::make_tuple(i, xp2));
					iset.insert(std::make_tuple(j, xp2));
				}
			};
			polyline_self_intersection(poly, visitor, cmp);
			return iset;
		}


		template <typename Polyline, typename NumberComparisonPolicy>
		inline std::vector<Polyline> split(const intersection_set<Polyline>& iset, const Polyline& pline, const NumberComparisonPolicy& cmp)
		{
			using access = point_sequence_traits<Polyline>;
			std::vector<Polyline> results;
			std::size_t lastIndex = 0;
			typename access::point_type lastPoint;

			GEOMETRIX_ASSERT(!iset.empty());
			auto it = iset.begin();
			{
				Polyline pi{ access::get_point(pline, 0) };
				auto cIndex = std::get<0>(*it);
				auto& xPoint = std::get<1>(*it);
				for (std::size_t i = 1; i <= cIndex; ++i)
				{
					if (!numeric_sequence_equals(access::get_point(pline, i), access::back(pi), cmp))
						pi.push_back(access::get_point(pline, i));
				}

				if (!numeric_sequence_equals(xPoint, access::back(pi), cmp))
					pi.push_back(xPoint);

				lastIndex = cIndex;
				assign(lastPoint, xPoint);

				if (access::size(pi) > 1)
					results.push_back(std::move(pi));
			}

			for (++it; it != iset.end(); ++it)
			{
				auto& item = *it;

				Polyline pi{ lastPoint };

				auto cIndex = std::get<0>(item);

				if (cIndex != lastIndex)
				{
					for (auto i = lastIndex + 1; i <= cIndex; ++i)
					{
						if (!numeric_sequence_equals(access::get_point(pline, i), access::back(pi), cmp))
							pi.push_back(access::get_point(pline, i));
					}
				}

				auto& xPoint = std::get<1>(item);
				if (!numeric_sequence_equals(xPoint, access::back(pi), cmp))
					pi.push_back(xPoint);

				lastIndex = cIndex;
				assign(lastPoint, xPoint);

				if (access::size(pi) > 1)
					results.push_back(std::move(pi));
			}

			Polyline pi{ lastPoint };
			auto size = access::size(pline);
			for (auto i = lastIndex + 1; i < size; ++i)
			{
				if (!numeric_sequence_equals(access::get_point(pline, i), access::back(pi), cmp))
					pi.push_back(access::get_point(pline, i));
			}

			if (access::size(pi) > 1)
				results.push_back(std::move(pi));

			return results;
		}

		template <orientation_type Orientation, typename Polyline, typename Length, typename NumberComparisonPolicy>
		inline std::vector<Polyline> liu_polyline_offset_impl(const Polyline& pline, const Length& offset, const NumberComparisonPolicy& cmp)
		{
			std::vector<Polyline> Array, tmpArray1, tmpArray2;

			auto pline1 = untrimmed_offset<Orientation>(pline, offset, cmp);
			intersection_set<Polyline> iset = self_intersection_set(pline1, cmp);

			if (!iset.empty())
			{
				auto splits = split(iset, pline, cmp);

				using point_type = typename point_sequence_traits<Polyline>::point_type;

				for (auto& pi : splits)
				{
					bool reject = false;
					std::size_t size = point_sequence_traits<Polyline>::size(pline);
					auto visitor = [&pline, &reject, size](intersection_type /*iType*/, std::size_t /*i1*/, std::size_t /*j1*/, std::size_t i2, std::size_t /*j2*/, const point_type& /*x1*/, const point_type& /*x2*/)
					{
						return reject = (i2 > 0 && (i2 + 1) < size);
					};

					if (!polyline_polyline_intersect(pi, pline, visitor, cmp))
						tmpArray1.push_back(std::move(pi));
					else if (!reject)
						tmpArray2.push_back(std::move(pi));
				}
			}
			else 
				tmpArray1.push_back(std::move(pline1));

			return Array;
		}

		template <orientation_type Orientation, typename Polyline, typename Length, typename NumberComparisonPolicy>
		inline Polyline polyline_offset_impl(const Polyline& poly, const Length& offset, const NumberComparisonPolicy& cmp)
		{
			using access = point_sequence_traits<Polyline>;
			using point_type = typename point_sequence_traits<Polyline>::point_type;
			using vector_type = vector<Length, dimension_of<point_type>::value>;

			std::size_t size = access::size(poly);
			GEOMETRIX_ASSERT(size > 1);

			auto A = access::get_point(poly, 0);
			auto B = access::get_point(poly, 1);
			vector_type vNormalAB = offset * oriented_normal<Orientation>(normalize(B - A));
			auto pOffsetA = construct<point_type>(A + vNormalAB);
			auto pOffsetB = construct<point_type>(B + vNormalAB);
			Polyline result = { pOffsetA };
			if (size > 2)
			{
				for (std::size_t d = 2; d < size; ++d)
				{
					auto& C = B;
					auto D = access::get_point(poly, d);

					vector_type vNormalCD = offset * oriented_normal<Orientation>(normalize(D - C));
					auto pOffsetC = construct<point_type>(C + vNormalCD);
					auto pOffsetD = construct<point_type>(D + vNormalCD);

					point_type xPoint;
					line_line_intersect(pOffsetA, pOffsetB, pOffsetC, pOffsetD, xPoint, cmp);
					result.push_back(xPoint);

					A = C;
					B = D;
					pOffsetA = pOffsetC;
					pOffsetB = pOffsetD;

					if (d + 1 == size)
						result.push_back(pOffsetD);
				}
			}
			else
			{
				result.push_back(pOffsetB);
			}

			return result;
		}
	}//! namespace detail;

    template <typename Polyline, typename Length, typename NumberComparisonPolicy>
	inline Polyline polyline_offset(const Polyline& poly, orientation_type side, const Length& offset, const NumberComparisonPolicy& cmp)
	{
		if( side == oriented_left )
		    return detail::polyline_offset_impl<oriented_left>(poly, offset, cmp);
		else
			return detail::polyline_offset_impl<oriented_right>(poly, offset, cmp);
    }

	template <typename Polyline, typename Length, typename NumberComparisonPolicy>
	inline Polyline polyline_offset2(const Polyline& poly, orientation_type side, const Length& offset, const NumberComparisonPolicy& cmp)
	{
		if (side == oriented_left)
			return detail::untrimmed_offset<oriented_left>(poly, offset, cmp);
		else
			return detail::untrimmed_offset<oriented_right>(poly, offset, cmp);
	}

}//namespace geometrix;

#endif //! GEOMETRIX_ALGORITHM_POINTSEQUENCE_POLYLINEOFFSET_HPP
