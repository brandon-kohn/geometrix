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

#include <geometrix/primitive/axis_aligned_bounding_box.hpp>
#include <geometrix/utility/unique_ptr.hpp>
#include <geometrix/algorithm/classify_simplex_to_plane.hpp>
#include <geometrix/algorithm/intersection/segment_plane_intersection.hpp>
#include <geometrix/algorithm/distance/point_segment_distance.hpp>
#include <geometrix/primitive/hyperplane_traits.hpp>
#include <geometrix/algorithm/intersection/ray_segment_intersection.hpp>
#include <geometrix/algorithm/point_in_solid_classification.hpp>

#include <boost/range/concepts.hpp>
#include <boost/range/algorithm_ext/iota.hpp>
#include <boost/dynamic_bitset.hpp>
#include <boost/mpl/inherit.hpp>
#include <boost/mpl/empty_base.hpp>
#include <stack>
#include <type_traits>

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
                        auto t = constants::zero<dimensionless_t>();
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
                        auto t = constants::zero<dimensionless_t>();
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

        template <typename Length>
        struct square_type
        {
            using type = decltype(std::declval<Length>() * std::declval<Length>());
        };

        template <typename Point, typename Simplex, typename NumberComparisonPolicy, typename std::enable_if<is_segment<Simplex>::value, int>::type = 0>
        inline typename square_type<typename arithmetic_type_of<Point>::type>::type point_simplex_distance_squared(const Point& p, const Simplex &smplx, const NumberComparisonPolicy&)
        {
            return  point_segment_distance_sqrd(p, smplx);
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
//  template <typename Simplex>
//  struct front_solid_leaf_bsp_traits
//  {
//      static const node_orientation solid_side = node_orientation::front;
//  };

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
        using dimensionless_type = typename dimensionless_type_of<typename plane_access::vector_type>::type;
        using vector_type = vector<dimensionless_type, dimension_of<typename plane_access::vector_type>::value>;
        using length_type = typename plane_access::arithmetic_type;
        using point_type = point<length_type, dimension_of<vector_type>::value>;
        using aabb_type = axis_aligned_bounding_box<point_type>;
        using index_type = std::uint32_t;
        using undefined_index = std::integral_constant<index_type, static_cast<index_type>(-1)>;
        using index_vector = std::vector<index_type>;

        static index_vector make_index_range(index_vector&& r)
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

            m_root = build_root(pgons, boost::dynamic_bitset<>(boost::size(pgons)), make_index_range(index_vector(boost::size(pgons))), selector, extract, cmp);
        }

        template <typename Simplices, typename SimplexSelector, typename NumberComparisonPolicy>
        solid_leaf_bsp_tree(const Simplices& pgons, const SimplexSelector& selector, const NumberComparisonPolicy& cmp)
            : solid_leaf_bsp_tree(pgons, selector, cmp, identity_simplex_extractor())
        {

        }

        solid_leaf_bsp_tree(solid_leaf_bsp_tree&& rhs)
            : m_front(std::move(rhs.m_front))
            , m_back(std::move(rhs.m_back))
            , m_node_planes(std::move(rhs.m_node_planes))
            , m_in_solid(std::move(rhs.m_in_solid))
            , m_indices(std::move(rhs.m_indices))
            , m_simplices(std::move(rhs.m_simplices))
            , m_planes(std::move(rhs.m_planes))
            , m_root(rhs.m_root)
        {}

        solid_leaf_bsp_tree& operator =(solid_leaf_bsp_tree&& rhs)
        {
            m_front = std::move(rhs.m_front);
            m_back = std::move(rhs.m_back);
            m_node_planes = std::move(rhs.m_node_planes);
            m_in_solid = std::move(rhs.m_in_solid);
            m_indices = std::move(rhs.m_indices);
            m_simplices = std::move(rhs.m_simplices);
            m_planes = std::move(rhs.m_planes);
            m_root = rhs.m_root;
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
            return ray_intersection_impl(p, d, false, cmp);
        }

        template <typename Point, typename Vector, typename NumberComparisonPolicy>
        solid_bsp_ray_tracing_result<typename arithmetic_type_of<Point>::type> ray_intersection_only_border(const Point& p, const Vector& d, const NumberComparisonPolicy& cmp) const
        {
            return ray_intersection_impl(p, d, true, cmp);
        }

        template <typename Point, typename NumberComparisonPolicy>
        typename bsp_detail::square_type<typename arithmetic_type_of<Point>::type>::type get_min_distance_sqrd_to_solid(const Point& p, std::size_t& simplexIndex, const NumberComparisonPolicy& cmp) const
        {
            return get_min_distance_sqrd_to_solid_impl(m_root, p, simplexIndex, cmp);
        }

        template <typename Point, typename NumberComparisonPolicy>
        typename arithmetic_type_of<Point>::type get_min_distance_to_solid(const Point& p, std::size_t& simplexIndex, const NumberComparisonPolicy& cmp) const
        {
            using std::sqrt;
            return sqrt(get_min_distance_sqrd_to_solid_impl(m_root, p, simplexIndex, cmp));
        }

    private:

        index_type create_leaf(bool isSolid)
        {
            index_type id = m_front.size();
            m_front.push_back(undefined_index::value);
            m_back.push_back(undefined_index::value);
            m_node_planes.push_back(undefined_index::value);
            m_in_solid.push_back(isSolid ? point_in_solid_classification::in_solid : point_in_solid_classification::in_empty_space);
            m_indices.resize(m_indices.size() + 1);
            return id;
        }

        index_type create_leaf(index_vector&& sIndices, bool isSolid)
        {
            index_type id = m_front.size();
            m_front.push_back(undefined_index::value);
            m_back.push_back(undefined_index::value);
            m_node_planes.push_back(undefined_index::value);
            m_in_solid.push_back(isSolid ? point_in_solid_classification::in_solid : point_in_solid_classification::in_empty_space);
            m_indices.emplace_back(std::forward<index_vector>(sIndices));
            return id;
        }

        index_type create_node(index_type sIndex, index_vector&& sIndices, index_type f, index_type b)
        {
            index_type id = m_front.size();
            m_front.push_back(f);
            m_back.push_back(b);
            m_node_planes.push_back(sIndex);
            m_in_solid.push_back(point_in_solid_classification::in_empty_space);
            m_indices.emplace_back(std::forward<index_vector>(sIndices));
            return id;
        }

        template <typename Simplices, typename SimplexSelector, typename SimplexExtractor, typename NumberComparisonPolicy>
        BOOST_FORCEINLINE index_type build_root(const Simplices& simplices, boost::dynamic_bitset<>&& usedBits, index_vector&& sIndices, const SimplexSelector& selector, const SimplexExtractor& extract, const NumberComparisonPolicy& cmp)
        {
            return build_tree<node_orientation::root>(simplices, std::forward<boost::dynamic_bitset<>>(usedBits), std::forward<index_vector>(sIndices), selector, extract, cmp);
        }

        //! Constructs BSP tree from an input vector of simplices.
        template <node_orientation Side, typename Simplices, typename SimplexSelector, typename SimplexExtractor, typename NumberComparisonPolicy>
        index_type build_tree(const Simplices& simplices, boost::dynamic_bitset<>&& usedBits, index_vector&& sIndices, const SimplexSelector& selector, const SimplexExtractor& extract, const NumberComparisonPolicy& cmp)
        {
            BOOST_CONCEPT_ASSERT((boost::RandomAccessRangeConcept<Simplices>));
            using item_t = typename boost::range_value<Simplices>::type;

            //! Return empty node if there are no simplices
            if (boost::empty(simplices))
                return create_leaf(Side == traits_type::solid_side);

            // Get number of simplices in the input vector
            auto nSimplices = boost::size(simplices);

            //! Select best possible partitioning plane based on the input geometry
            auto selected = selector(simplices, usedBits);

            //! Test each polygon against the dividing plane, adding them
            //! to the front list, back list, or both, as appropriate
            if (selected == simplices.end())
            {
                return create_leaf(std::move(sIndices), Side == traits_type::solid_side);
            }

            auto sIndex = sIndices[std::distance(boost::begin(simplices), selected)];
            auto splitPlane = m_planes[sIndex];
            std::vector<item_t> frontList, backList;
            boost::dynamic_bitset<> frontBits, backBits;
            index_vector frontIndices, backIndices, coplanarIndices;
            std::size_t i = 0;
            auto add_to_front = [&i, &frontList, &frontBits, &usedBits, &frontIndices, &sIndices](const item_t& item) -> void {
                frontList.push_back(item);
                frontBits.push_back(usedBits[i]);
                frontIndices.push_back(sIndices[i]);
            };
            auto add_to_back = [&i, &backList, &backBits, &usedBits, &backIndices, &sIndices](const item_t& item) -> void {
                backList.push_back(item);
                backBits.push_back(usedBits[i]);
                backIndices.push_back(sIndices[i]);
            };
            for (const auto& item : simplices)
            {
                auto smplx = extract(item);

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

            return create_node(sIndex, /*std::move(coplanar),*/ std::move(coplanarIndices), fNode, bNode);
        }

        vector_type get_normal_vector(index_type node) const
        {
            GEOMETRIX_ASSERT(m_node_planes[node] != -1);
            return plane_access::get_normal_vector(m_planes[m_node_planes[node]]);
        }

        length_type get_distance_to_origin(index_type node) const
        {
            GEOMETRIX_ASSERT(m_node_planes[node] != -1);
            return plane_access::get_distance_to_origin(m_planes[m_node_planes[node]]);
        }

        template <typename Point, typename NumberComparisonPolicy>
        point_in_solid_classification point_in_solid_space(index_type node, const Point& p, const NumberComparisonPolicy& cmp) const
        {
            while (!is_leaf(node))
            {
                //! Compute distance of point to dividing plane
                auto dist = scalar_projection(as_vector(p), get_normal_vector(node)) - get_distance_to_origin(node);
                auto zero = constants::zero<typename std::decay<decltype(dist)>::type>();
                if (cmp.greater_than(dist, zero))
                {
                    // Point in front of plane, so traverse front of tree
                    node = m_front[node];
                }
                else if (cmp.less_than(dist, zero))
                {
                    //! Point behind of plane, so traverse back of tree
                    node = m_back[node];
                }
                else
                {
                    //! Point on dividing plane; must traverse both sides
                    auto front = point_in_solid_space(m_front[node], p, cmp);
                    auto back = point_in_solid_space(m_back[node], p, cmp);
                    //! If results agree, return that, else point is on boundary
                    return (front == back) ? front : point_in_solid_classification::on_boundary;
                }

                GEOMETRIX_ASSERT(node != -1);
            }

            //! Now at a leaf, inside/outside status determined by solid flag

            return in_solid_classification(node);
        }

        template <typename Point, typename NumberComparisonPolicy>
        typename bsp_detail::square_type<typename arithmetic_type_of<Point>::type>::type get_min_distance_sqrd_to_solid_impl(index_type node, const Point& p, std::size_t& closestSimplex, const NumberComparisonPolicy& cmp) const
        {
            using length_t = typename arithmetic_type_of<Point>::type;
            using area_t = decltype(std::declval<length_t>()*std::declval<length_t>());
            auto minDist2 = constants::infinity<area_t>();
            auto calc_orientation = [&cmp](length_t dist) -> plane_orientation
            {
                const auto zero = constants::zero<length_t>();
                if (cmp.greater_than(dist, zero))
                    return plane_orientation::in_front_of_plane;
                else if (cmp.less_than(dist, zero))
                    return plane_orientation::in_back_of_plane;
                //! Point on dividing plane; must traverse both sides
                return plane_orientation::straddling_plane;
            };

            std::stack<index_type> nodeStack;
            nodeStack.push(m_root);
            while (!nodeStack.empty())
            {
                auto node = nodeStack.top();
                nodeStack.pop();

                for(auto idx : m_indices[node])
                {
                    auto d2 = bsp_detail::point_simplex_distance_squared(p, m_simplices[idx], cmp);
                    if(d2 < minDist2)
                    {
                        closestSimplex = idx;
                        minDist2 = d2;
                    }
                }

                if(!is_leaf(node))
                {
                    //! Compute distance of point to dividing plane
                    auto dist = scalar_projection(as_vector(p), get_normal_vector(node)) - get_distance_to_origin(node);
                    auto dist2 = dist * dist;
                    auto orientation = calc_orientation(dist);

                    //! back
                    if (dist2 < minDist2 || orientation != plane_orientation::in_front_of_plane )
                        nodeStack.push(m_back[node]);

                    //! front
                    if (dist2 < minDist2 || orientation != plane_orientation::in_back_of_plane )
                        nodeStack.push(m_front[node]);
                }
            }

            return minDist2;
        }

        template <typename Point, typename NumberComparisonPolicy>
        point_in_solid_classification point_in_solid_space_no_boundary(index_type node, const Point& p, const NumberComparisonPolicy& cmp) const
        {
            while (!is_leaf(node))
            {
                //! Compute distance of point to dividing plane
                auto dist = scalar_projection(as_vector(p), get_normal_vector(node)) - get_distance_to_origin(node);
                //! Traverse front of tree when point in front of plane, else back of tree
                auto zero = constants::zero<typename std::decay<decltype(dist)>::type>();
                node = cmp.less_than_or_equal(dist, zero) ? m_back[node] : m_front[node];
            }

            //! Now at a leaf, inside/outside status determined by solid flag
            return in_solid_classification(node);
        }

        template <typename Point, typename Vector, typename Indices, typename NumberComparisonPolicy>
        index_type find_closest_hit(const Point& p, const Vector& d, const Indices& sIndices, const NumberComparisonPolicy& cmp) const
        {
            auto minDist = (std::numeric_limits<length_type>::max)();
            length_type t;
            index_type minIndex = undefined_index::value;
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
        solid_bsp_ray_tracing_result<typename arithmetic_type_of<Point>::type> ray_intersection_impl(const Point& p, const Vector& d, bool onlyBorder, const NumberComparisonPolicy& cmp) const
        {
            using length_t = typename arithmetic_type_of<Point>::type;
            auto tmax = std::numeric_limits<length_t>::max();
            auto tmin = constants::zero<length_t>();

            using elem_t = std::tuple<index_type, length_t, std::set<index_type>/*, std::vector<simplex_type>*/>;
            std::stack<elem_t> nodeStack;
            auto node = m_root;
            std::set<index_type> maxIndices, minIndices;
            //std::vector<simplex_type> maxS, minS;
            while (1)
            {
                if (!is_leaf(node))
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
                                auto farNode = (1 ^ nearIndex) ? m_back[node] : m_front[node];
                                nodeStack.push(std::make_tuple(farNode, tmax, std::move(maxIndices)/*, std::move(maxS)*/));

                                maxIndices.insert(m_indices[node].begin(), m_indices[node].end());
                                //using boost::adaptors::transformed;
                                //boost::copy(node->indices | transformed([this](index_type i) { return m_simplices[i]; }), std::back_inserter(maxS));
                                tmax = t;
                            }
                            else
                                nearIndex = 1 ^ nearIndex;//! 0 <= t < tmin, visit far side
                        }
                    }
                    node = nearIndex ? m_back[node] : m_front[node];
                }
                else
                {
                    //! Now at a leaf. If it is solid, there’s a hit at time tmin, so exit
                    if (is_solid(node))
                    {
                        //! Look at geometry in sIndices and find the closest hit.
						if (!minIndices.empty() || !onlyBorder)
						{
							auto sIndex = find_closest_hit(p, d, minIndices, cmp);
							return solid_bsp_ray_tracing_result<length_t>(true, tmin, sIndex);
						}

						auto sIndex = find_closest_hit(p, d, maxIndices, cmp);
						return solid_bsp_ray_tracing_result<length_t>(true, tmax, sIndex);
                    }

                    //! Exit if no more subtrees to visit, else pop off a node and continue
                    if (nodeStack.empty())
                        break;
                    tmin = tmax;
                    minIndices = std::move(maxIndices);
                    //minS = maxS;
                    std::tie(node, tmax, maxIndices/*, maxS*/) = nodeStack.top();
                    nodeStack.pop();
                }

                GEOMETRIX_ASSERT(node != undefined_index::value);
            }

            //! No hit
            return solid_bsp_ray_tracing_result<length_t>(false);
        }

        bool is_leaf(index_type n) const { return m_front[n] == -1 && m_back[n] == -1; }
        bool is_solid(index_type n) const { return m_in_solid[n] == point_in_solid_classification::in_solid; }
        bool is_solid_leaf(index_type n) const { return is_solid(n) && is_leaf(n); }
        point_in_solid_classification in_solid_classification(index_type n) const { return m_in_solid[n]; }

        index_vector m_front;
        index_vector m_back;
        index_vector m_node_planes;
        std::vector<point_in_solid_classification> m_in_solid;
        std::vector<index_vector> m_indices;
        std::vector<simplex_type> m_simplices;
        std::vector<plane_type>   m_planes;
        index_type                m_root{ undefined_index::value };
    };

}//namespace geometrix;

#endif //GEOMETRIX_SOLID_LEAF_BSPTREE_HPP
