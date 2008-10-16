//
//!  Copyright (c) 2008
//!  Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_GEOMETRY_POLAR_REFERENCE_FRAME_HPP
#define _BOOST_GEOMETRY_POLAR_REFERENCE_FRAME_HPP
#pragma once

#include "affine_space_traits.hpp"

namespace boost
{
namespace numeric
{
namespace geometry
{  
    template <typename AffineSpace>
    class polar_reference_frame
    {
    public:

        typedef AffineSpace                                                       affine_space_type;
        typedef typename affine_space_traits< affine_space_type >::dimension_type dimension_type;
        typedef typename affine_space_traits< affine_space_type >::point_type     base_point_type;
        typedef typename affine_space_traits< affine_space_type >::vector_type    base_vector_type;
        typedef typename point_traits< base_point_type >::coordinate_type         coordinate_type;
        typedef polar_reference_frame< affine_space_type >                        reference_frame_type;
        typedef reference_frame_tag< base_point_type, reference_frame_type >      point_type;
        typedef reference_frame_tag< base_vector_type, reference_frame_type >     vector_type;
                
        polar_reference_frame( const base_point_type& origin )
            : m_origin( origin )
        {}

        polar_reference_frame( const point_type& origin )
            : m_origin( origin )
        {}

         //! Reference frame must define an origin.
        inline const point_type& get_origin() { return m_origin; }

        //! dimension_access needs to be specialized on point type up to point's dimension_type...
        //! there is no generic way to do this for a generic point type as it requires knowledge of the underlying type.
        //! Assume the general case needs to support random access via unsigned integer indices.
        template <typename Index>
        static inline coordinate_type get( const point_type& p ) 
        {
            BOOST_MPL_ASSERT_MSG
            (
                ( dimension_traits< Index >::value >= 0 || dimension_traits< Index >::value < dimension_type::value )
		        , INDEX_OUT_OF_BOUNDS
		        , (Index, Point)
             );
            
            return p.get<dimension_traits<Index>::value>();
        }

        //! point_type runtime index access.
        static inline coordinate_type get( const point_type& p, size_t index ) 
        {
            boost::function_requires< PointIndexedAccessConcept< point_type > >();
            std::assert( index >= 0 && index < dimension_type::value );
            return p[ index ];
        }

        //! base_point_type compile time access.
        template <typename Index>
        static inline coordinate_type get( const base_point_type& p ) 
        {
            BOOST_MPL_ASSERT_MSG
            (
                ( dimension_traits< Index >::value >= 0 || dimension_traits< Index >::value < dimension_type::value )
		        , INDEX_OUT_OF_BOUNDS
		        , (Index, Point)
             );
            
            return p.get<dimension_traits<Index>::value>();
        }

        //! base_point_type runtime index access.
        static inline coordinate_type get( const base_point_type& p, size_t index ) 
        {
            boost::function_requires< PointIndexedAccessConcept< point_type > >();
            std::assert( index >= 0 && index < dimension_type::value );
            return p[ index ];
        }

        //! vector_type compile time access
        template <typename Index>
        static inline coordinate_type get( const vector_type& p ) 
        {
            BOOST_MPL_ASSERT_MSG
            (
                ( dimension_traits< Index >::value >= 0 || dimension_traits< Index >::value < dimension_type::value )
		        , INDEX_OUT_OF_BOUNDS
		        , (Index, Point)
             );
            
            return p.get<dimension_traits<Index>::value>();
        }

        //! vector_type runtime index access.
        static inline coordinate_type get( const vector_type& p, size_t index ) 
        {
            boost::function_requires< PointIndexedAccessConcept< point_type > >();
            std::assert( index >= 0 && index < dimension_type::value );
            return p[ index ];
        }

        //! base vector compile time access.
        template <typename Index>
        static inline coordinate_type get( const base_vector_type& p ) 
        {
            BOOST_MPL_ASSERT_MSG
            (
                ( dimension_traits< Index >::value >= 0 || dimension_traits< Index >::value < dimension_type::value )
		        , INDEX_OUT_OF_BOUNDS
		        , (Index, Point)
             );
            
            return p.get<dimension_traits<Index>::value>();
        }
        
        //! base_vector_type runtime index access.
        static inline coordinate_type get( const base_vector_type& p, size_t index ) 
        {
            boost::function_requires< PointIndexedAccessConcept< point_type > >();
            std::assert( index >= 0 && index < dimension_type::value );
            return p[ index ];
        }

    private:

        const point_type m_origin;

    };

    //! Define the base traits of a frame of reference. 
    template <typename AffineSpace>    
    struct reference_frame_traits< polar_reference_frame<AffineSpace> >
    {
        //! Reference frame belongs to some affine space.
        typedef AffineSpace                                                    affine_space_type;
        typedef typename affine_space_traits< affine_space_type >::point_type  point_type;
        typedef typename affine_space_traits< affine_space_type >::vector_type vector_type;

        //! Reference frame must define an origin.
        inline static point_type get_origin( const polar_reference_frame<AffineSpace>& frame ) { return frame.get_origin(); }
    };

    //! Attempt to make a reference frame tag points and vectors in some way such that they are then known to be in that frame. How? Adaptor wrapper?


}}}//namespace boost::numeric::geometry;

#endif //_BOOST_GEOMETRY_POLAR_REFERENCE_FRAME_HPP
