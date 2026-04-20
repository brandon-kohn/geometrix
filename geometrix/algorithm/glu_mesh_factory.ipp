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
#include <geometrix/primitive/point_sequence_traits.hpp>
#include <geometrix/primitive/point.hpp>
#include <boost/dynamic_bitset.hpp>
#include <type_traits>

#if WIN32
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#pragma comment( lib, "opengl32.lib" )
#pragma comment( lib, "glu32.lib" )
#endif

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
						
			template <typename CoordinateType>
			mesh_2d<CoordinateType> create_mesh(const glu_mesh_factory::vertex_polygon& polygon, bool fixTrigOrientation)
			{			
				gluTessBeginPolygon(m_pTesselator, reinterpret_cast<void*>(this));			
				m_vertices = polygon;
				std::size_t vertex_num = 0;
				std::size_t pSize = polygon.size();
				gluTessBeginContour(m_pTesselator);
				for (std::size_t i = 0; i < pSize ; ++i)
				{
					auto& vert = m_vertices[vertex_num];
					m_points.emplace_back(vert.x, vert.y);
					gluTessVertex(m_pTesselator, reinterpret_cast<double*>(&vert), reinterpret_cast<void*>(vertex_num++));
				}

				gluTessEndContour(m_pTesselator);
				gluTessEndPolygon(m_pTesselator);

				return create_mesh<CoordinateType>(fixTrigOrientation);
			}

			template <typename CoordinateType>
			mesh_2d<CoordinateType> create_mesh(const std::vector<glu_mesh_factory::vertex_polygon>& polygons, bool fixTrigOrientation)
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
						auto &vert = m_vertices[vertex_num];
						m_points.emplace_back(vert.x,vert.y);
						gluTessVertex(m_pTesselator, reinterpret_cast<double*>(&vert), reinterpret_cast<void*>(vertex_num++));
					});
					gluTessEndContour(m_pTesselator);
				}
				gluTessEndPolygon(m_pTesselator);
				return create_mesh<CoordinateType>(fixTrigOrientation);
			}
			
		private:
		
			template <typename CoordinateType>
			mesh_2d<CoordinateType> create_mesh(bool fixTrigOrientation)
			{
				return mesh_2d<CoordinateType>( m_points, m_indices, direct_comparison_policy{}, fixTrigOrientation );	
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
			std::vector<std::size_t> m_indices; //! The list of the triangle indices
			std::vector<geometrix::point<double,2>> m_points; //! A list of the points to be used in the partition
			std::vector<glu_mesh_factory::vertex> m_vertices;
		};
	}//! namespace detail;

	template <typename CoordinateType>
	inline mesh_2d<CoordinateType> glu_mesh_factory::create_impl( const vertex_polygon& polygon, double precision, bool fixTrigOrientation )
	{
		detail::glu_tesselator gt(precision);
		return gt.create_mesh<CoordinateType>(polygon, fixTrigOrientation);
	}
	
	template <typename CoordinateType>
	inline mesh_2d<CoordinateType> glu_mesh_factory::create_impl( const std::vector<vertex_polygon>& polygons, double precision, bool fixTrigOrientation )
	{	
		detail::glu_tesselator gt(precision);
		return gt.create_mesh<CoordinateType>(polygons, fixTrigOrientation);
	}

}//namespace geometrix

#endif // GEOMETRIX_GLU_MESH_FACTORY_IPP
