//
//! Copyright © 2015
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
/////////////////////////////////////////////////////////////////////////////
#ifndef GEOMETRIX_GLU_MESH_FACTORY_HPP
#define GEOMETRIX_GLU_MESH_FACTORY_HPP
#pragma once

#include <geometrix/algorithm/mesh_2d.hpp>
#include <vector>

namespace geometrix
{	
	class glu_mesh_factory
	{
		struct vertex
		{
			vertex()
			{}

			template <typename Point>
			vertex( const Point& p )
			: x(geometrix::get<0>(p))
			, y(geometrix::get<0>(p))
			{}

			double x{0.};
			double y{0.};
			double z{0.};// coordinates
			double nx{0.};
			double ny{0.};
			double nz{1.};// normal vector
		};
		
		typedef std::vector<vertex> vertex_polygon;
	public:
		
		template <typename Polygon>
		static mesh_2d create( const Polygon& polygon, double precision = 1e-6 )
		{
			vertex_polygon vpoly;
			vpoly << polygon;
			return create(vpoly, precision);
		}

		template <typename Polygon>
		static mesh_2d create( const Polygon& outer, const std::vector<Polygon>& holes, double precision = 1e-6 )
		{
			vertex_polygon vpolyouter;
			vpolyouter << outer;
			std::vector<vertex_polygon> polygons;
			std::transform(holes.begin(), holes.end(), polygons.begin(), [](const Polygon& poly) -> vertex_polygon { vertex_polygon vhole; vhole << poly; return std::move(vhole); });
			polygons.push_back(vpolyouter);
			return create(polygons, precision);
		}
		
	private:
		
		template <typename Polygon>
		friend void operator << ( vertex_polygon& vertices, const Polygon& polygon )
		{
			for(const auto& p : polygon)
			{
				vertices.emplace_back(geometrix::get<0>(p), geometrix::get<1>(p));
			}
		}

		friend class glu_tesselator;
	
		glu_mesh_factory() = delete;
				
		static mesh_2d create( vertex_polygon&& polygon, double precision );
		static mesh_2d create( std::vector<vertex_polygon>&& polygons, double precision );
		
	};

}//namespace geometrix

#endif // GEOMETRIX_GLU_MESH_FACTORY_HPP
