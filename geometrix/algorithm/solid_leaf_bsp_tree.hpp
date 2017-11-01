//
//! Copyright © 2017
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_SOLID_LEAF_BSPTREE_HPP
#define GEOMETRIX_SOLID_LEAF_BSPTREE_HPP
#pragma once

#include <geometrix/utility/unique_ptr.hpp>
#include <geometrix/algorithm/classify_simplex_to_plane.hpp>
#include <geometrix/algorithm/intersection/segment_plane_intersection.hpp>
#include <geometrix/primitive/hyperplane_traits.hpp>
#include <geometrix/algorithm/point_in_solid_classification.hpp>

#include <boost/range/concepts.hpp>
#include <boost/range/algorithm_ext/iota.hpp>
#include <boost/dynamic_bitset.hpp>
#include <boost/mpl/inherit.hpp>
#include <boost/mpl/empty_base.hpp>

namespace geometrix {

	template <typename Length>
	struct solid_bsp_ray_tracing_result
	{
		using length_type = Length;

		typedef void (solid_bsp_ray_tracing_result::*bool_type)() const;

		solid_bsp_ray_tracing_result() = default;

		solid_bsp_ray_tracing_result(bool isIntersecting, const length_type& thit = length_type(), const std::uint32_t data = static_cast<std::uint32_t>(-1) )
			: result(isIntersecting)
			, thit(thit)
			, data(data)
		{}

		bool is_intersecting() const { return result; }
		length_type intersection_distance() const { return thit; }
				
		operator bool_type() const
		{
			return is_intersecting() ? &solid_bsp_ray_tracing_result::bool_type_mfn : 0;
		}

		bool operator == (bool b) const { return b == is_intersecting(); }

		void set_data(std::uint32_t d) { data = d; }
		std::uint32_t get_data() const { return data; }

	private:

		void bool_type_mfn() const {}
		
		bool result{ false };
		length_type thit;
		std::uint32_t data{ static_cast<std::uint32_t>(-1) };
	};


    // Given a vector of simplices, attempts to compute a good splitting plane
    /*
    inline Hyperplane PickSplittingPlane(std::vector<Simplex *> &simplices)
    {
        // Blend factor for optimizing for balance or splits (should be tweaked)
        const float K = 0.8f;
        // Variables for tracking best splitting plane seen so far
        Hyperplane bestPlane;
        float bestScore = FLT_MAX;
        // Try the plane of each polygon as a dividing plane
        for (int i = 0; i <simplices.size(); i++)
        {
            int numInFront = 0, numBehind = 0, numStraddling = 0;
            Hyperplane plane = GetPlaneFromPolygon(simplices[i]);
            // Test against all other simplices
            for (int j = 0; j <simplices.size(); j++)
            {
                // Ignore testing against self
                if (i == j) continue;
                // Keep standing count of the various smplx-plane relationships
                switch (classify_simplex_to_plane(simplices[j], plane))
                {
                case POLYGON_COPLANAR_WITH_PLANE:
                    // Coplanar simplices treated as being in front of plane
                case POLYGON_IN_FRONT_OF_PLANE:
                    numInFront++;
                    break;
                case POLYGON_BEHIND_PLANE:
                    numBehind++;
                    break;
                case POLYGON_STRADDLING_PLANE:
                    numStraddling++;
                    break;
                }
            }

            // Compute score as a weighted combination (based on K, with K in range
            // 0..1) between balance and splits (lower score is better)
            float score=K*numStraddling + (1.0f - K) * Abs(numInFront - numBehind);
            if (score < bestScore)
            {
                bestScore = score;
                bestPlane = plane;
            }
        }

        return bestPlane;
    }
    */
	namespace bsp_detail
	{
		//! Split a segment.
		template <typename Simplex, typename Hyperplane, typename NumberComparisonPolicy, typename std::enable_if<is_segment<Simplex>::value, int>::type = 0>
		inline void split_simplex(const Simplex &smplx, const Hyperplane& plane, Simplex& frontPoly, Simplex& backPoly, const NumberComparisonPolicy& cmp)
		{
			using simplex_t = typename std::decay<Simplex>::type;
			using point_t = typename geometric_traits<simplex_t>::point_type;
			using dimensionless_t = typename geometric_traits<point_t>::dimensionless_type;

			auto a = get_vertex(smplx, 0);
			auto b = get_vertex(smplx, 1);

			//! Create (and return) two new simplices from the two vertex lists
			auto aSide = classify_point_to_plane(a, plane, cmp);
			auto bSide = classify_point_to_plane(b, plane, cmp);
			if (aSide == plane_orientation::in_front_of_plane)
			{
				if (bSide == plane_orientation::in_back_of_plane)
				{
					//! Edge (a, b) straddles, output intersection point to both sides
					dimensionless_t t = constants::zero<dimensionless_t>();
					point_t xpoint;
					auto intersects = segment_plane_intersection(b, a, plane, t, xpoint);
					GEOMETRIX_ASSERT(intersects);
					GEOMETRIX_ASSERT(classify_point_to_plane(xpoint, plane, cmp) == plane_orientation::coplanar_with_plane);

					frontPoly = construct<Simplex>(a, xpoint);
					backPoly = construct<Simplex>(xpoint, b);
				} else
				{
					GEOMETRIX_ASSERT(false);//! This case should not happen.
					GEOMETRIX_ASSERT(bSide == plane_orientation::coplanar_with_plane);//! If both are on front.. why the split?
					frontPoly = construct<Simplex>(a, b);
				}
			} else if (aSide == plane_orientation::in_back_of_plane)
			{
				if (bSide == plane_orientation::in_front_of_plane)
				{
					dimensionless_t t = constants::zero<dimensionless_t>();
					point_t xpoint;
					auto intersects = segment_plane_intersection(b, a, plane, t, xpoint);
					GEOMETRIX_ASSERT(intersects);
					GEOMETRIX_ASSERT(classify_point_to_plane(xpoint, plane, cmp) == plane_orientation::coplanar_with_plane);

					backPoly = construct<Simplex>(a, xpoint);
					frontPoly = construct<Simplex>(xpoint, b);
				} else
				{
					GEOMETRIX_ASSERT(false);//! This case should not happen.	
					GEOMETRIX_ASSERT(bSide == plane_orientation::coplanar_with_plane);//! If both are on back.. should not split?
					backPoly = construct<Simplex>(a, b);
				}
			} else
			{
				GEOMETRIX_ASSERT(false);//! This case should not happen.
				//! A is on the plane.
				if (bSide == plane_orientation::in_back_of_plane)
					backPoly = construct<Simplex>(a, b);
				else
					frontPoly = construct<Simplex>(a, b);
			}
		}

		//! Split polygon
		template <typename Simplex, typename Hyperplane, typename NumberComparisonPolicy, typename std::enable_if<!is_segment<Simplex>::value, int>::type = 0>
		inline void split_simplex(const Simplex &smplx, const Hyperplane& plane, Simplex& frontPoly, Simplex& backPoly, const NumberComparisonPolicy& cmp)
		{
			using simplex_t = typename std::decay<Simplex>::type;
			using point_t = typename geometric_traits<simplex_t>::point_type;
			using dimensionless_t = typename geometric_traits<point_t>::dimensionless_type;

			std::vector<point_t> frontVerts, backVerts;
			// Test all edges (a, b) starting with edge from last to first vertex
			auto numVerts = number_vertices(smplx);
			auto a = get_vertex(smplx, numVerts - 1);
			auto aSide = classify_point_to_plane(a, plane, cmp);
			// Loop over all edges given by vertex pair (n -1, n)
			for (auto n = 0; n < numVerts; ++n)
			{
				auto b = get_vertex(smplx, n);
				auto bSide = classify_point_to_plane(b, plane, cmp);
				if (bSide == plane_orientation::in_front_of_plane)
				{
					if (aSide == plane_orientation::in_back_of_plane)
					{
						// Edge (a, b) straddles, output intersection point to both sides
						dimensionless_t t = 0.0;
						point_t xpoint;
						auto intersects = segment_plane_intersection(b, a, plane, t, xpoint);
						GEOMETRIX_ASSERT(intersects);
						GEOMETRIX_ASSERT(classify_point_to_plane(xpoint, plane, cmp) == plane_orientation::coplanar_with_plane);
						frontVerts.push_back(xpoint);
						backVerts.push_back(xpoint);
					}
					// In all three cases, output b to the front side
					frontVerts.push_back(b);
				} else if (bSide == plane_orientation::in_back_of_plane)
				{
					if (aSide == plane_orientation::in_front_of_plane)
					{
						// Edge (a, b) straddles plane, output intersection point
						dimensionless_t t = 0.0;
						point_t xpoint;
						auto intersects = segment_plane_intersection(b, a, plane, t, xpoint);
						GEOMETRIX_ASSERT(intersects);
						GEOMETRIX_ASSERT(classify_point_to_plane(xpoint, plane, cmp) == plane_orientation::coplanar_with_plane);
						frontVerts.push_back(xpoint);
						backVerts.push_back(xpoint);
					} else if (aSide == plane_orientation::coplanar_with_plane)
					{
						// Output a when edge (a, b) goes from ‘on’ to ‘behind’ plane
						backVerts.push_back(a);
					}
					// In all three cases, output b to the back side
					backVerts.push_back(b);
				} else
				{
					// b is on the plane. In all three cases output b to the front side
					frontVerts.push_back(b);
					// In one case, also output b to back side
					if (aSide == plane_orientation::in_back_of_plane)
						backVerts.push_back(b);
				}
				// Keep b as the starting point of the next edge
				a = b;
				aSide = bSide;
			}

			// Create (and return) two new simplices from the two vertex lists
			frontPoly = construct<Simplex>(frontVerts);
			backPoly = construct<Simplex>(backVerts);
		}

		template <typename Point, typename Vector, typename Simplex, typename Length, typename NumberComparisonPolicy, typename std::enable_if<is_segment<Simplex>::value, int>::type = 0>
		inline bool ray_simplex_intersect(const Point& p, const Vector& v, const Simplex &smplx, Length& t, const NumberComparisonPolicy& cmp)
		{
			using length_t = typename arithmetic_type_of<Point>::type;
			geometrix::point<length_t, dimension_of<Point>::value> xPoints[2];
			return ray_segment_intersection(p, v, smplx, t, xPoints, cmp);
		}
	}//! namespace bsp_detail;

	struct identity_simplex_extractor
	{
		template <typename T>
		T& operator()(T&& item) const { return item; }

		template <typename T, typename U>
		T& make_split(T&& item, U&&) const { return item; }
	};

	struct null_node_visitor
	{
		template <typename TreePtr, typename SplitItem, typename Node>
		void operator()(TreePtr&&, SplitItem&&, Node&&) const {}
	};

    enum class node_orientation
    {
        front
      , back
      , root
    };
	
	//! TODO: Ray tracing doesn't work with front side solid convention yet.
//	template <typename Simplex>
//	struct front_solid_leaf_bsp_traits
//	{
//		static const node_orientation solid_side = node_orientation::front;
//	};
	
    template <typename Simplex>
    struct back_solid_leaf_bsp_traits
    {
        static const node_orientation solid_side = node_orientation::back;
    };

	//! From Real Time Collision Detection:
    template <typename Simplex, typename Traits = back_solid_leaf_bsp_traits<Simplex> >
    class solid_leaf_bsp_tree
    {
        using traits_type = Traits;
        using simplex_type = Simplex;
        using plane_type = typename result_of::make_hyperplane<simplex_type>::type;
        using plane_access = hyperplane_access_traits<plane_type>;
        using vector_type = typename plane_access::vector_type;
        using length_type = typename plane_access::arithmetic_type;
		using point_type = point<length_type, dimension_of<vector_type>::value>;
		using aabb_type = axis_aligned_bounding_box<point_type>;

        struct bsp_node
        {
            bsp_node(std::uint32_t plane, /*std::vector<simplex_type>&& simplices,*/ std::vector<std::uint32_t>&& indices, std::unique_ptr<bsp_node>&& f, std::unique_ptr<bsp_node>&& b)
            : front(std::forward<std::unique_ptr<bsp_node>>(f))
            , back(std::forward<std::unique_ptr<bsp_node>>(b))
            , plane(plane)
			//, simplices(std::forward<std::vector<simplex_type>>(simplices))
			, indices(std::forward<std::vector<std::uint32_t>>(indices))
            {}

			bsp_node(bool solid)
				: in_solid(!solid ? point_in_solid_classification::in_empty_space : point_in_solid_classification::in_solid)
			{

			}

			bsp_node(/*std::vector<simplex_type>&& simplices,*/ std::vector<std::uint32_t>&& indices, bool solid)
            : in_solid(!solid ? point_in_solid_classification::in_empty_space : point_in_solid_classification::in_solid)
			//, simplices(std::forward<std::vector<simplex_type>>(simplices))
			, indices(std::forward<std::vector<std::uint32_t>>(indices))
            {
			
			}

            bool is_leaf() const { return !front && !back; }
            bool is_solid() const { return in_solid == point_in_solid_classification::in_solid; }
			bool is_solid_leaf() const { return is_solid() && is_leaf(); }

			point_in_solid_classification in_solid_classification() const { return in_solid; }
			            
            std::unique_ptr<bsp_node>   front;
            std::unique_ptr<bsp_node>   back;
			std::uint32_t				plane{ static_cast<std::uint32_t>(-1) };
            point_in_solid_classification in_solid { point_in_solid_classification::in_empty_space };

			//std::vector<simplex_type> simplices;
			std::vector<std::uint32_t> indices;
        };
		
		static std::vector<std::uint32_t> make_index_range(std::vector<std::uint32_t>&& r)
		{
			boost::iota(r, 0);
			return std::move(r);
		}

    public:

        solid_leaf_bsp_tree() = default;

        template <typename Simplices, typename SimplexSelector, typename NumberComparisonPolicy, typename SimplexExtractor>
        solid_leaf_bsp_tree(const Simplices& pgons, const SimplexSelector& selector, const NumberComparisonPolicy& cmp, SimplexExtractor&& extract)            
        {
			for (const auto& pgon : pgons)
			{
				auto smplx = extract(pgon);
				m_simplices.emplace_back(smplx);
				m_planes.emplace_back(make_hyperplane(smplx));
			}

			m_root = build_root(pgons, boost::dynamic_bitset<>{ boost::size(pgons) }, make_index_range(std::vector<std::uint32_t>(boost::size(pgons))), selector, extract, cmp);
        }

		template <typename Simplices, typename SimplexSelector, typename NumberComparisonPolicy>
		solid_leaf_bsp_tree(const Simplices& pgons, const SimplexSelector& selector, const NumberComparisonPolicy& cmp)
			: solid_leaf_bsp_tree(pgons, selector, cmp, identity_simplex_extractor())
		{

		}

        solid_leaf_bsp_tree(solid_leaf_bsp_tree&& rhs)
            : m_simplices(std::move(rhs.m_simplices))
			, m_planes(std::move(rhs.m_planes))
			, m_root(std::move(rhs.m_root))
        {}

        solid_leaf_bsp_tree& operator =(solid_leaf_bsp_tree&& rhs)
        {
			m_simplices = std::move(rhs.m_simplices);
			m_planes = std::move(rhs.m_planes);
            m_root = std::move(rhs.m_root);
            return *this;
        }

        template <typename Point, typename NumberComparisonPolicy>
        point_in_solid_classification point_in_solid_space(const Point& p, const NumberComparisonPolicy& cmp) const
        {
            return point_in_solid_space(m_root, p, cmp);
        }

        template <typename Point, typename Vector, typename NumberComparisonPolicy>
		solid_bsp_ray_tracing_result<typename arithmetic_type_of<Point>::type> ray_intersection(const Point& p, const Vector& d, const NumberComparisonPolicy& cmp) const
        {
            return ray_intersection_impl(p, d, cmp);
        }

    private:

		template <typename Simplices, typename SimplexSelector, typename SimplexExtractor, typename NumberComparisonPolicy>
		BOOST_FORCEINLINE std::unique_ptr<bsp_node> build_root(const Simplices& simplices, boost::dynamic_bitset<>&& usedBits, std::vector<std::uint32_t>&& sIndices, const SimplexSelector& selector, const SimplexExtractor& extract, const NumberComparisonPolicy& cmp)
		{
			return build_tree<node_orientation::root>(simplices, std::forward<boost::dynamic_bitset<>>(usedBits), std::forward<std::vector<std::uint32_t>>(sIndices), selector, extract, cmp);
		}
		
        //! Constructs BSP tree from an input vector of simplices.
        template <node_orientation Side, typename Simplices, typename SimplexSelector, typename SimplexExtractor, typename NumberComparisonPolicy>
        std::unique_ptr<bsp_node> build_tree(const Simplices& simplices, boost::dynamic_bitset<>&& usedBits, std::vector<std::uint32_t>&& sIndices, const SimplexSelector& selector, const SimplexExtractor& extract, const NumberComparisonPolicy& cmp)
        {
            BOOST_CONCEPT_ASSERT((boost::RandomAccessRangeConcept<Simplices>));
			using item_t = typename boost::range_value<Simplices>::type;

            //! Return empty node if there are no simplices
            if (boost::empty(simplices))
                return boost::make_unique<bsp_node>(Side == traits_type::solid_side);

            // Get number of simplices in the input vector
            auto nSimplices = boost::size(simplices);

            //! Select best possible partitioning plane based on the input geometry
            auto selected = selector(simplices, usedBits);
			
            //! Test each polygon against the dividing plane, adding them
            //! to the front list, back list, or both, as appropriate
			if (selected == simplices.end())
			{
				//std::vector<simplex_type> sims;
				//using boost::adaptors::transformed;
				//boost::copy(simplices | transformed([&extract](const item_t& i) { return extract(i); }), std::back_inserter(sims));
				return boost::make_unique<bsp_node>(/*std::move(sims),*/ std::move(sIndices), Side == traits_type::solid_side);
			}

			auto sIndex = sIndices[std::distance(boost::begin(simplices), selected)];
            auto splitPlane = m_planes[sIndex];
			//std::vector<simplex_type> coplanar;
			std::vector<item_t> frontList, backList;
            boost::dynamic_bitset<> frontBits, backBits;
			std::vector<std::uint32_t> frontIndices, backIndices, coplanarIndices;
            std::size_t i = 0;
            for (const auto& item : simplices)
            {
				auto smplx = extract(item);
				auto add_to_front = [i, &frontList, &frontBits, &usedBits, &frontIndices, &sIndices](const item_t& item) -> void {
					frontList.push_back(item);
					frontBits.push_back(usedBits[i]);
					frontIndices.push_back(sIndices[i]);
				};
				auto add_to_back = [i, &backList, &backBits, &usedBits, &backIndices, &sIndices](const item_t& item) -> void {
					backList.push_back(item);
					backBits.push_back(usedBits[i]);
					backIndices.push_back(sIndices[i]);
				};
                switch (classify_simplex_to_plane(smplx, splitPlane, cmp))
                {
                case plane_orientation::coplanar_with_plane:
				{
					//! What’s done in this case depends on what type of tree is being
					//! built. For a node-storing tree, the polygon is stored inside
					//! the node at this level (along with all other simplices coplanar
					//! with the plane). Here, for a leaf-storing tree, coplanar simplices
					//! are sent to either side of the plane. In this case, to the front
					//! side, by falling through to the next case

					//! Try storing for debugging.
					//coplanar.emplace_back(smplx);
					coplanarIndices.push_back(sIndices[i]);

					//! Attempt to push according to the normal of the smplex.
					auto smplxPlane = m_planes[sIndices[i]];
					auto smplxNormal = plane_access::get_normal_vector(smplxPlane);
					auto planeNormal = plane_access::get_normal_vector(splitPlane);
					auto dp = dot_product(smplxNormal, planeNormal);
					GEOMETRIX_ASSERT(cmp.equals(dp, constants::one<decltype(dp)>()) || cmp.equals(dp, -constants::one<decltype(dp)>()));
					if (dp < constants::zero<decltype(dp)>())
					{
						add_to_back(item);
						break;
					}					
					BOOST_FALLTHROUGH;
				}
                case plane_orientation::in_front_of_plane:
					add_to_front(item);
                    break;
                case plane_orientation::in_back_of_plane:
					add_to_back(item);
                    break;
                case plane_orientation::straddling_plane:
                {
                    //! Split polygon to plane and send a part to each side of the plane
					simplex_type frontPart;
                    simplex_type backPart;
                    bsp_detail::split_simplex(smplx, splitPlane, frontPart, backPart, cmp);
                    add_to_front(extract.make_split(frontPart, item));
                    add_to_back(extract.make_split(backPart, item));
                    break;
                }
                };

                ++i;
            }

            //! Recursively build child subtrees and return new tree root combining them
			auto fNode = build_tree<node_orientation::front>(frontList, std::move(frontBits), std::move(frontIndices), selector, extract, cmp);
			auto bNode = build_tree<node_orientation::back>(backList, std::move(backBits), std::move(backIndices), selector, extract, cmp);
			return boost::make_unique<bsp_node>(sIndex, /*std::move(coplanar),*/ std::move(coplanarIndices), std::move(fNode), std::move(bNode));			
        }

		vector_type get_normal_vector(const bsp_node* pNode) const 
		{ 
			GEOMETRIX_ASSERT(pNode->plane != -1); 
			return plane_access::get_normal_vector(m_planes[pNode->plane]); 
		}

		length_type get_distance_to_origin(const bsp_node* pNode) const 
		{
			GEOMETRIX_ASSERT(pNode->plane != -1);
			return plane_access::get_distance_to_origin(m_planes[pNode->plane]); 
		}

        template <typename Point, typename NumberComparisonPolicy>
        point_in_solid_classification point_in_solid_space(const std::unique_ptr<bsp_node>& startNode, const Point& p, const NumberComparisonPolicy& cmp) const
        {
            bsp_node* node = startNode.get();
			GEOMETRIX_ASSERT(node);
            while (!node->is_leaf())
            {
                //! Compute distance of point to dividing plane                
                auto dist = scalar_projection(as_vector(p), get_normal_vector(node)) - get_distance_to_origin(node);
                auto zero = constants::zero<typename std::decay<decltype(dist)>::type>();
                if (cmp.greater_than(dist, zero))
                {
                    // Point in front of plane, so traverse front of tree
                    node = node->front.get();
                }
                else if (cmp.less_than(dist, zero))
                {
                    //! Point behind of plane, so traverse back of tree
                    node = node->back.get();
                }
                else
                {
                    //! Point on dividing plane; must traverse both sides
                    auto front = point_in_solid_space(node->front, p, cmp);
                    auto back = point_in_solid_space(node->back, p, cmp);
                    //! If results agree, return that, else point is on boundary
                    return (front == back) ? front : point_in_solid_classification::on_boundary;
                }

				GEOMETRIX_ASSERT(node);
            }

            //! Now at a leaf, inside/outside status determined by solid flag

			return node->in_solid_classification();
        }

        template <typename Point, typename NumberComparisonPolicy>
        point_in_solid_classification point_in_solid_space_no_boundary(const std::unique_ptr<bsp_node>& startNode, const Point& p, const NumberComparisonPolicy& cmp) const
        {
            bsp_node* node = startNode.get();
			GEOMETRIX_ASSERT(node);
            while (!node->is_leaf())
            {
                //! Compute distance of point to dividing plane
                auto dist = scalar_projection(as_vector(p), get_normal_vector(node)) - get_distance_to_origin(node);
                //! Traverse front of tree when point in front of plane, else back of tree
                auto zero = constants::zero<typename std::decay<decltype(dist)>::type>();
                node = cmp.less_than_or_equal(dist, zero) ? node->back.get() : node->front.get();
				GEOMETRIX_ASSERT(node);
            }

            //! Now at a leaf, inside/outside status determined by solid flag
			return node->in_solid_classification();
        }

		template <typename Point, typename Vector, typename Indices, typename NumberComparisonPolicy>
		std::uint32_t find_closest_hit(const Point& p, const Vector& d, const Indices& sIndices, const NumberComparisonPolicy& cmp) const
		{
			auto minDist = (std::numeric_limits<length_type>::max)();
			length_type t;
			std::uint32_t minIndex = static_cast<std::uint32_t>(-1);
			for (auto i : sIndices)
			{
				if (bsp_detail::ray_simplex_intersect(p, d, m_simplices[i], t, cmp) && t < minDist)
				{
					minDist = t;
					minIndex = i;
				}
			}

			return minIndex;
		}

        //! Intersect ray/segment R(t)=p+t*d, tmin <= t <= tmax, against bsp tree
        //! ’node’, returning distance along the ray thit of first intersection with a solid leaf, if any
        template <typename Point, typename Vector, typename NumberComparisonPolicy>
		solid_bsp_ray_tracing_result<typename arithmetic_type_of<Point>::type> ray_intersection_impl(const Point& p, const Vector& d, const NumberComparisonPolicy& cmp) const
        {
			using length_t = typename arithmetic_type_of<Point>::type;
			auto tmax = std::numeric_limits<length_t>::max();
			auto tmin = constants::zero<length_t>();

#ifdef _DEBUG
			auto ray = segment<Point>{ p, p + 20.0 * d };
#endif

            using elem_t = std::tuple<bsp_node*, length_t, std::set<std::uint32_t>/*, std::vector<simplex_type>*/>;
            std::stack<elem_t> nodeStack;
            auto node = m_root.get();
			std::set<std::uint32_t> maxIndices, minIndices;
			//std::vector<simplex_type> maxS, minS;
            GEOMETRIX_ASSERT(node);
            while (1)
            {
                if (!node->is_leaf())
                {
                    auto denom = scalar_projection(d, get_normal_vector(node));
					//! Compute distance from dividing plane to p.
                    auto dist = get_distance_to_origin(node) - scalar_projection(as_vector(p), get_normal_vector(node));
                    auto nearIndex = dist > constants::zero<length_t>();
                    //! If denom is zero, ray runs parallel to plane. In this case,
                    //! just fall through to visit the near side (the one p lies on)
                    if (denom != constants::zero<decltype(denom)>())
                    {
                        auto t = dist / denom;
                        if (constants::zero<length_t>() <= t && t <= tmax)
                        {
                            if (t >= tmin)
                            {
                                //! Straddling, push far side onto stack,then visit near side					
								auto farNode = (1 ^ nearIndex) ? node->back.get() : node->front.get();
                                nodeStack.push(std::make_tuple(farNode, tmax, std::move(maxIndices)/*, std::move(maxS)*/));
								
								maxIndices.insert(node->indices.begin(), node->indices.end());
								//using boost::adaptors::transformed;
								//boost::copy(node->indices | transformed([this](std::uint32_t i) { return m_simplices[i]; }), std::back_inserter(maxS));
                                tmax = t;
                            }
                            else
                                nearIndex = 1 ^ nearIndex;//! 0 <= t < tmin, visit far side
                        }
                    }
                    node = nearIndex ? node->back.get() : node->front.get();
                }
                else
                {
                    //! Now at a leaf. If it is solid, there’s a hit at time tmin, so exit
                    if (node->is_solid())
                    {
						//! Look at geometry in sIndices and find the closest hit.
						auto sIndex = find_closest_hit(p, d, minIndices, cmp);
						return solid_bsp_ray_tracing_result<length_t>(true, tmin, sIndex);
                    }

					//! Exit if no more subtrees to visit, else pop off a node and continue
                    if (nodeStack.empty())
                        break;
                    tmin = tmax;
					minIndices = maxIndices;
					//minS = maxS;
					std::tie(node, tmax, maxIndices/*, maxS*/) = nodeStack.top();
                    nodeStack.pop();
                }

				GEOMETRIX_ASSERT(node);
            }
			
			//! No hit
			return solid_bsp_ray_tracing_result<length_t>(false);
        }

		std::vector<simplex_type> m_simplices;
		std::vector<plane_type>   m_planes;
        std::unique_ptr<bsp_node> m_root;

    };

}//namespace geometrix;

#endif //GEOMETRIX_SOLID_LEAF_BSPTREE_HPP