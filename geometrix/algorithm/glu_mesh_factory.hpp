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
	public:
		struct vertex
		{
			vertex()
			{}

			template <typename Point>
			vertex( const Point& p )
			: x(get(get<0>(p)))
			, y(get(get<1>(p)))
			{}

			double x{0.};
			double y{0.};
			double z{0.};// coordinates
			double nx{0.};
			double ny{0.};
			double nz{1.};// normal vector
		};
		
		typedef std::vector<vertex> vertex_polygon;
		
		template <typename CoordinateType, typename Polygon>
		static mesh_2d<CoordinateType> create( const Polygon& polygon, double precision = 1e-10, bool fixTrigOrientation = true )
		{
			vertex_polygon vpoly;
			vpoly.reserve( polygon.size() );
			insert( vpoly, polygon);
			return create_impl<CoordinateType>(vpoly, precision, fixTrigOrientation);
		}

		template <typename CoordinateType, typename Polygon>
		static mesh_2d<CoordinateType> create( const Polygon& outer, const std::vector<Polygon>& holes, double precision = 1e-10, bool fixTrigOrientation = true )
		{
			vertex_polygon vpolyouter;
			insert(vpolyouter, outer);
			std::vector<vertex_polygon> polygons;
			std::transform(holes.begin(), holes.end(), polygons.begin(), [](const Polygon& poly) -> vertex_polygon { vertex_polygon vhole; insert(vhole, poly); return vhole; });
			polygons.push_back(vpolyouter);
			return create_impl<CoordinateType>(polygons, precision, fixTrigOrientation);
		}
		
	private:
		
		template <typename Polygon>
		static void insert( vertex_polygon& vertices, const Polygon& polygon )
		{
			for(auto p : polygon)
			{
				vertices.emplace_back( p );
			}
		}

		friend class glu_tesselator;
	
		glu_mesh_factory() = delete;
			
		template <typename CoordinateType>
		static mesh_2d<CoordinateType> create_impl( const vertex_polygon& polygon, double precision, bool fixTrigOrientation );
		template <typename CoordinateType>
		static mesh_2d<CoordinateType> create_impl( const std::vector<vertex_polygon>& polygons, double precision, bool fixTrigOrientation );
		
	};

}//namespace geometrix

#endif // GEOMETRIX_GLU_MESH_FACTORY_HPP
