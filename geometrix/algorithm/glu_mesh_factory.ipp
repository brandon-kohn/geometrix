//
//! Copyright © 2015
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
/////////////////////////////////////////////////////////////////////////////
#ifndef GEOMETRIX_GLU_MESH_FACTORY_IPP
#define GEOMETRIX_GLU_MESH_FACTORY_IPP
#pragma once

#include "glu_mesh_factory.hpp"
#include <geometrix/primitive/point_sequence.hpp>
#include <geometrix/primitive/point.hpp>

#include <GL/GLU.h>

typedef void (CALLBACK *GluTessCallbackType)();

namespace geometrix
{	
	namespace detail
	{	
		class glu_tesselator
		{
		public:

			glu_tesselator(double precision)
			: m_pTesselator(gluNewTess())
			{
				gluTessCallback(m_pTesselator, GLU_TESS_BEGIN_DATA, reinterpret_cast<GluTessCallbackType>(begin_callback));
				gluTessCallback(m_pTesselator, GLU_TESS_EDGE_FLAG_DATA, reinterpret_cast<GluTessCallbackType>(edge_flag_callback));
				gluTessCallback(m_pTesselator, GLU_TESS_VERTEX_DATA, reinterpret_cast<GluTessCallbackType>(vertex_callback));
				gluTessCallback(m_pTesselator, GLU_TESS_END_DATA, reinterpret_cast<GluTessCallbackType>(end_callback));
				gluTessCallback(m_pTesselator, GLU_TESS_COMBINE_DATA, reinterpret_cast<GluTessCallbackType>(combine_callback));
				gluTessCallback(m_pTesselator, GLU_TESS_ERROR_DATA, reinterpret_cast<GluTessCallbackType>(error_callback));
				gluTessNormal(m_pTesselator, 0, 0, 1);
				gluTessProperty(m_pTesselator, GLU_TESS_TOLERANCE, precision);			
			}
			
			~glu_tesselator()
			{
				gluDeleteTess(m_pTesselator);
			}
							
			mesh_2d create_mesh(const glu_mesh_factory::vertex_polygon& polygon)
			{			
				gluTessBeginPolygon(m_pTesselator, reinterpret_cast<void*>(this));			
				m_vertices = polygon;
				std::size_t vertex_num = 0;
				std::size_t pSize = geometrix::get_size(polygon);
				gluTessBeginContour(m_pTesselator);
				for (std::size_t i = 0; i < pSize ; ++i)
				{
					vertex& vert = m_vertices[vertex_num];
					m_points.emplace_back(vert.x, vert.y);
					gluTessVertex(m_pTesselator, reinterpret_cast<double*>(&vert), reinterpret_cast<void*>(vertex_num++));
				}

				gluTessEndContour(m_pTesselator);
				gluTessEndPolygon(m_pTesselator);

				return create_mesh();
			}

			mesh_2d glu_tesselator::create_mesh(const glu_mesh_factory::vertex_polygon& polygons)
			{
				for(const auto& polygon : polygons)
					for(const auto& p : polygon)
						m_vertices.push_back(p);
				
				std::size_t vertex_num = 0;

				gluTessBeginPolygon(m_pTesselator, reinterpret_cast<void*>(this));
				for(const auto& contour : polygons)
				{
					gluTessBeginContour(m_pTesselator);
					std::for_each(contour.begin(), contour.end(), [this, &vertex_num]()
					{
						vertex &vert = m_vertices[vertex_num];
						m_points.emplace_back(vert.x,vert.y);
						gluTessVertex(m_pTesselator, reinterpret_cast<double*>(&vert), reinterpret_cast<void*>(vertex_num++));
					});
					gluTessEndContour(m_pTesselator);
				}
				gluTessEndPolygon(m_pTesselator);
				return create_mesh();
			}
			
		private:
			
			mesh_2d create_mesh()
			{
				return mesh_2d(m_points,m_indices,false);	
			}
			
			//! GLU_TESS_BEGIN_DATA callback    
			static void CALLBACK begin_callback( GLenum type, glu_tesselator* caller )
			{
				
			}

			//! GLU_TESS_COMBINE_DATA callback
			static void CALLBACK combine_callback( GLdouble coords[3], std::size_t vertexData[4], GLfloat weight[4], std::size_t* outData, glu_tesselator* caller )
			{
				geometrix::point<double,2> p(coords[0],coords[1]);
				glu_mesh_factory::vertex vert(p);
				*outData = caller->m_vertices.size();
				caller->m_vertices.push_back(vert);
				caller->m_points.push_back(p);
			}

			//! GLU_TESS_EDGE_FLAG_DATA callback
			static void CALLBACK edge_flag_callback( GLboolean flag, glu_tesselator* caller )
			{
				caller->m_boundaryEdge = flag;
			}

			//! GLU_TESS_VERTEX_DATA callback
			static void CALLBACK vertex_callback( std::size_t vertexIndex, glu_tesselator* caller )
			{
				caller->m_indices.push_back(vertexIndex);
				caller->m_edgeFlags.push_back(caller->m_boundaryEdge);
			}

			//! GLU_TESS_END_DATA callback
			static void CALLBACK end_callback( glu_tesselator* caller )
			{
				
			}

			//! GLU_TESS_ERROR_DATA callback
			static void CALLBACK error_callback( GLenum errno, glu_tesselator* caller )
			{
				
			}
					
			GLUtesselator* m_pTesselator {nullptr};
			bool m_boundaryEdge {false}; //! Flag used to identify segments which lie on the boundary of the original polygon
			boost::dynamic_bitset<>	m_edgeFlags; //! A list of the edge flags marking the boundary of the tesselation	    
			std::vector<std::intptr_t> m_indices; //! The list of the triangle indices
			std::vector<geometrix::point<double,2>> m_points; //! A list of the points to be used in the partition
			std::vector<glu_mesh_factory::vertex> m_vertices;
		};
	}//! namespace detail;
	
	inline mesh_2d glu_mesh_factory::create( vertex_polygon&& polygon, double precision )
	{
		detail::glu_tesselator gt(precision);
		return gt.create_mesh(polygon);
	}
	
	inline mesh_2d glu_mesh_factory::create( std::vector<vertex_polygon>&& polygons, double precision )
	{	
		detail::glu_tesselator gt(precision);
		return gt.create_mesh(polygons);
	}

}//namespace geometrix

#endif // GEOMETRIX_GLU_MESH_FACTORY_IPP
