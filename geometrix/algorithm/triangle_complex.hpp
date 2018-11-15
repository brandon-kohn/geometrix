//
//! Copyright © 2018
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include <geometrix/utility/utilities.hpp>
#include <geometrix/primitive/point_traits.hpp>
#include <boost/functional/hash.hpp>
#include <unordered_map>

namespace geometrix {

	template <typename Point, typename NumberComparisonPolicy>
	class triangle_complex
	{
		struct point_compare
		{
			point_compare(const NumberComparisonPolicy& cmp)
				: cmp(cmp)
			{}

			bool operator()(const Point& a, const Point& b) const
			{
				return lexicographically_less_than(a, b, cmp);
			}

			NumberComparisonPolicy cmp;
		};

		using vertex_handle = std::uint32_t;
		using invalid_handle = std::integral_constant<vertex_handle, static_cast<vertex_handle>(-1)>;
		using vertex_handle_map = std::map<Point, vertex_handle>;
		using vertex_point_map = std::vector<Point>;
		using edge_key = std::tuple<vertex_handle, vertex_handle>;
		using trig_key = std::tuple<vertex_handle, vertex_handle, vertex_handle>;

		struct half_edge
		{
			half_edge(vertex_handle o = invalid_handle::value, bool constrained = false)
				: opposite(o)
				, constrained(constrained)
			{}

			vertex_handle opposite;
			bool constrained;
		};

		using triangle_edge_opposite_vertex_map = std::unordered_map<edge_key, half_edge, boost::hash<edge_key>>;
		using triangle_map = std::unordered_map<trig_key, std::array<Point, 3>, boost::hash<trig_key>>;

	public:

		triangle_complex() = default;
		triangle_complex(const NumberComparisonPolicy& cmp)
			: m_cmp(cmp)
			, m_vertexHandleMap(point_compare(cmp))
		{}

		bool add_triangle(Point a, Point b, Point c)
		{
			sort(a, b, c);
			auto key = get_triangle_key(a, b, c);
			vertex_handle u, v, w;
			std::tie(u, v, w) = key;
			return add_triangle(u, v, w);
		}

		bool set_constraint(const Point& a, const Point& b, bool c)
		{
			auto u = get_vertex_handle(a);
			auto v = get_vertex_handle(b);
			auto it = m_edgeVertexMap.find(edge_key(u, v));
			if (it != m_edgeVertexMap.end()) {
				it->second.constrained = c;

				it = m_edgeVertexMap.find(edge_key(v, u));
				if (it != m_edgeVertexMap.end()) {
					it->second.constrained = c;
					return true;
				}
			}

			return false;
		}

		template <typename EncroachSegmentPredicate, typename TrianglePredicate>
		void refine(const stk::units::angle& minAngle, EncroachSegmentPredicate&& is_encroached, TrianglePredicate&& is_bad)
		{
			auto badSegs = get_encroaching_segments(std::forward<EncroachSegmentPredicate>(is_encroached));
			split_encroaching_segments(badSegs);

			std::deque<trig_key> badTrigs;

			for(auto item : m_triangleMap)
			{
				if (is_bad(item.first, item.second))
					badTrigs.emplace_back(item.first);
			}
		}

		/*
        bool delete_triangle(Point a, Point b, Point c)
        {
            sort(a,b,c);
            auto key = find_triangle_key(a, b, c);

            if(is_valid(key))
            {
                vertex_handle u, v, w;
                std::tie(u,v,w) = key;
                delete_triangle(u, v, w);
                return true;
            }

            return false;
        }
		*/

        void insert_vertex(const Point& p, Point a, Point b, Point c)
        {
            sort(a,b,c);
            auto key = find_triangle_key(a, b, c);

            GEOMETRIX_ASSERT(is_valid(key));
            GEOMETRIX_ASSERT(point_in_circumcircle(p, a, b, c, m_cmp) == point_circle_orientation::inside);

            vertex_handle u = get_vertex_handle(p), v, w, x;
            std::tie(v,w,x) = key;
            insert_vertex(u, v, w, x);
        }

    private:

		template <typename EncroachSegmentPredicate>
		std::deque<edge_key> get_encroaching_segments(EncroachSegmentPredicate&& is_encroached) const
		{
			std::deque<edge_key> badSegs;

			for (auto item : m_edgeVertexMap) 
			{
				vertex_handle u, v;
				std::tie(u, v) = item.first;
				auto w = item.second.opposite;

				if (is_encroached(m_points[w], m_points[u], m_points[v]))
					badSegs.push_back(item.first);
			}

			return std::move(badSegs);
		}

		bool in_diametral_circle(Point const& p, Point const& o, Point const& d)
		{
			//! If the angle between OP and DP is obtuse, then P is inside the diametral circle of OD.
			//! two vectors have obtuse angle if dot product is negative.
			auto dp = dot_product(o - p, d - p);
			return dp < constants::zero<decltype(dp)>();
		}

		//bool in_diametral_lens(Angle const& theta, Point const& o, Point const& d, const Point& p)
		bool in_diametral_lens(Point const& p, Point const& o, Point const& d)
		{
			using std::cos;

			static const stk::units::angle theta = 0.523599 * units::si::radians;

			stk::vector2 op = o - p;
			stk::vector2 dp = d - p;
			auto dt = dot_product(op, dp);
			auto cos_theta = cos(theta);
			auto v2_cos_theta2_1 = 2.0 * cos_theta * cos_theta - 1.0;
			return (dt * dt) >= (v2_cos_theta2_1 * v2_cos_theta2_1 * magnitude_sqrd(op) * magnitude_sqrd(dp));
		}

        bool add_triangle(vertex_handle u, vertex_handle v, vertex_handle w)
        {
            auto key = trig_key(u,v,w);
            auto it = m_triangleMap.find(key);
            if(it == m_triangleMap.end())
            {
                m_edgeVertexMap[edge_key(u,v)] = w;
                m_edgeVertexMap[edge_key(v,w)] = u;
                m_edgeVertexMap[edge_key(w,u)] = v;

                std::array<Point,3> trig = {m_points[u],m_points[v],m_points[w]};
                m_triangleMap.emplace_hint(it, key, trig);
                return true;
            }

            return false;
        }

        void delete_triangle(vertex_handle u, vertex_handle v, vertex_handle w, bool& uvConstrained, bool& vwConstrained, bool& wuConstrained)
        {
            m_triangleMap.erase(trig_key(u,v,w));
            m_edgeVertexMap.erase(edge_key(u,v));
            m_edgeVertexMap.erase(edge_key(v,w));
            m_edgeVertexMap.erase(edge_key(w,u));
            m_triangleMap.erase(trig_key(u,v,w));

			auto it = m_edgeVertexMap.find(edge_key(u,v));
			if(it != m_edgeVertexMap.end())
			{ 
				uvConstrained = it->second.constrained;
				m_edgeVertexMap.erase(it);
			}

			it = m_edgeVertexMap.find(edge_key(v,w));
			if(it != m_edgeVertexMap.end())
			{ 
				vwConstrained = it->second.constrained;
				m_edgeVertexMap.erase(it);
			}

			it = m_edgeVertexMap.find(edge_key(w,u));
			if(it != m_edgeVertexMap.end())
			{ 
				wuConstrained = it->second.constrained;
				m_edgeVertexMap.erase(it);
			}
        }
        
        void insert_vertex(vertex_handle u, vertex_handle v, vertex_handle w, vertex_handle x)
        {
			bool vwConstrained = false;
			bool wxConstrained = false;
			bool xvConstrained = false;
            delete_triangle(v,w,x, vwConstrained, wxConstrained, xvConstrained);
            dig_cavity(u,v,w, vwConstrained);
            dig_cavity(u,w,x, wxConstrained);
            dig_cavity(u,x,v, xvConstrained);
        }

        half_edge adjacent(vertex_handle u, vertex_handle v) const
        {
            auto key = edge_key(u,v);
            auto it = m_edgeVertexMap.find(key);
            if(it != m_edgeVertexMap.end())
                return it->second;

            return half_edge(invalid_handle::value);
        }

        bool is_sorted(const Point& a, const Point& b, const Point& c) const
        {
            return lexicographically_less_than(a,b,m_cmp) && lexicographically_less_than(a,c,m_cmp) && is_ccw(a,b,c);
        }

        bool is_ccw(const Point& a, const Point& b, const Point& c) const
        {
            return (get_orientation(a, b, c, m_cmp) != oriented_right);
        }

        void sort(Point& a, Point& b, Point& c) const
        {
            using std::swap;

            //! A should always be the lexicographically lowest point. Then all points should order CCW.
            if(lexicographically_less_than(c, a, m_cmp))
                swap(a,c);

            if(lexicographically_less_than(b, a, m_cmp))
                swap(a,b);

            if(!is_ccw(a, b, c))
                swap(b,c);
        }

        bool is_degenerate(const Point& a, const Point& b, const Point& c) const
        {
            return (numeric_sequence_equals(a, b, m_cmp) ||
                    numeric_sequence_equals(b, c, m_cmp) ||
                    numeric_sequence_equals(c, a, m_cmp));
        }

        vertex_handle find_vertex_handle(const Point& p) const
        {
	        auto it = m_vertexHandleMap.lower_bound(p);
        	if( it != m_vertexHandleMap.end() && !m_vertexHandleMap.key_comp()(p, it->first) )
                return it->second;

            return invalid_handle::value;
        }

        vertex_handle get_vertex_handle(const Point& p) const
        {
	        auto it = m_vertexHandleMap.lower_bound(p);
        	if( it != m_vertexHandleMap.end() && !m_vertexHandleMap.key_comp()(p, it->first) )
                return it->second;

		    auto vDescriptor = get_descriptor();
		    m_vertexHandleMap.insert(it, std::make_pair(p, vDescriptor));
            if(vDescriptor >= m_points.size())
                m_points.emplace_back(p);
            else
                m_points[vDescriptor] = p;
            return vDescriptor;
        }

        vertex_handle get_descriptor() const
        {
            if(m_freeDescriptors.empty())
                return m_points.size();

            auto v = m_freeDescriptors.top();
            m_freeDescriptors.pop();
            return v;
        }

        trig_key get_triangle_key(const Point& a, const Point& b, const Point& c) const
        {
            GEOMETRIX_ASSERT(is_sorted(a,b,c));
            auto v0 = get_vertex_handle(a);
            auto v1 = get_vertex_handle(b);
            auto v2 = get_vertex_handle(c);
            return trig_key(v0, v1, v2);
        }

        trig_key find_triangle_key(const Point& a, const Point& b, const Point& c) const
        {
            GEOMETRIX_ASSERT(is_sorted(a,b,c));
            auto v0 = find_vertex_handle(a);
            auto v1 = find_vertex_handle(b);
            auto v2 = find_vertex_handle(c);
            return trig_key(v0, v1, v2);
        }

        bool is_valid(trig_key const& key) const
        {
            return std::get<0>(key) != invalid_handle::value &&
                std::get<1>(key) != invalid_handle::value &&
                std::get<2>(key) != invalid_handle::value;
        }

        void dig_cavity(vertex_handle u, vertex_handle v, vertex_handle w, bool vwConstrained)
        {
            auto x = adjacent(w,v);
            if(x.opposite != invalid_handle::value)
            {
                if(!x.constrained && point_in_circumcircle(m_points[v], m_points[w], m_points[x], u, m_cmp) == point_circle_orientation::inside)
                {
					bool wvConstrained = false, vxConstrained = false, xwConstrained = false;
                    delete_triangle(w,v,x, wvConstrained, vxConstrained, xwConstrained);
                    dig_cavity(u,v,x, vxConstrained);
                    dig_cavity(u,x,w, xwConstrained);
                }
				else 
				{
					add_triangle(u, v, w);
					auto it = m_edgeVertexMap.find(edge_key(v, w));
					GEOMETRIX_ASSERT(it != m_edgeVertexMap.end());
					if (it != m_edgeVertexMap.end())
						it->second.constrained = vwConstrained;
				}
            }
        }

        NumberComparisonPolicy            m_cmp;
        mutable vertex_handle_map         m_vertexHandleMap;
        mutable std::stack<vertex_handle> m_freeDescriptors;
        mutable vertex_point_map          m_points;
        triangle_edge_opposite_vertex_map m_edgeVertexMap;
        triangle_map                      m_triangleMap;

    };

}//! namespace geometrix;

