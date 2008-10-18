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
#include "affine_space.hpp"

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
                
         //! Reference frame must define an origin.
        inline point_type get_origin() { return point_type(); }

        //! dimension_access needs to be specialized on point type up to point's dimension_type...
        //! there is no generic way to do this for a generic point type as it requires knowledge of the underlying type.
        //! Assume the general case needs to support random access via unsigned integer indices.
        template <unsigned int Index>
        static inline coordinate_type get( const point_type& p ) 
        {
            BOOST_MPL_ASSERT_MSG
            (
                ( dimension_traits< Index >::value >= 0 || dimension_traits< Index >::value < dimension_type::value )
		        , INDEX_OUT_OF_BOUNDS
		        , (Point)
             );
            
            return p.get<dimension_traits<Index>::value>();
        }

        //! point_type runtime index access.
        static inline coordinate_type get( const point_type& p, size_t index ) 
        {
            boost::function_requires< PointIndexedAccessConcept< point_type > >();
            BOOST_ASSERT( index >= 0 && index < dimension_type::value );
            return p[ index ];
        }

        //! base_point_type compile time access.
        template <unsigned int Index>
        static inline coordinate_type get( const base_point_type& p ) 
        {
            BOOST_MPL_ASSERT_MSG
            (
                ( dimension_traits< Index >::value >= 0 || dimension_traits< Index >::value < dimension_type::value )
		        , INDEX_OUT_OF_BOUNDS
		        , (Point)
             );
            
            return p.get<dimension_traits<Index>::value>();
        }

        //! base_point_type runtime index access.
        static inline coordinate_type get( const base_point_type& p, size_t index ) 
        {
            boost::function_requires< PointIndexedAccessConcept< point_type > >();
            BOOST_ASSERT( index >= 0 && index < dimension_type::value );
            return p[ index ];
        }

        //! vector_type compile time access
        template <unsigned int Index>
        static inline coordinate_type get( const vector_type& p ) 
        {
            BOOST_MPL_ASSERT_MSG
            (
                ( dimension_traits< Index >::value >= 0 || dimension_traits< Index >::value < dimension_type::value )
		        , INDEX_OUT_OF_BOUNDS
		        , (Point)
             );
            
            return p.get<dimension_traits<Index>::value>();
        }

        //! vector_type runtime index access.
        static inline coordinate_type get( const vector_type& p, size_t index ) 
        {
            boost::function_requires< PointIndexedAccessConcept< point_type > >();
            BOOST_ASSERT( index >= 0 && index < dimension_type::value );
            return p[ index ];
        }

        //! base vector compile time access.
        template <unsigned int Index>
        static inline coordinate_type get( const base_vector_type& p ) 
        {
            BOOST_MPL_ASSERT_MSG
            (
                ( dimension_traits< Index >::value >= 0 || dimension_traits< Index >::value < dimension_type::value )
		        , INDEX_OUT_OF_BOUNDS
		        , (Point)
             );
            
            return p.get<dimension_traits<Index>::value>();
        }
        
        //! base_vector_type runtime index access.
        static inline coordinate_type get( const base_vector_type& p, size_t index ) 
        {
            boost::function_requires< PointIndexedAccessConcept< point_type > >();
            BOOST_ASSERT( index >= 0 && index < dimension_type::value );
            return p[ index ];
        }

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

    typedef polar_reference_frame< affine_space_float_2d >               polar_reference_frame_float_2d;
    typedef polar_reference_frame< affine_space_float_3d >               polar_reference_frame_float_3d;
    typedef polar_reference_frame< affine_space_double_2d >              polar_reference_frame_double_2d;
    typedef polar_reference_frame< affine_space_double_3d >              polar_reference_frame_double_3d;
    typedef polar_reference_frame< affine_space_int_2d >                 polar_reference_frame_int_2d;
    typedef polar_reference_frame< affine_space_int_3d >                 polar_reference_frame_int_3d;
    typedef polar_reference_frame< affine_space_int64_2d >               polar_reference_frame_int64_2d;
    typedef polar_reference_frame< affine_space_int64_3d >               polar_reference_frame_int64_3d;

    typedef polar_reference_frame< affine_space_float_2d >::point_type   polar_point_float_2d;
    typedef polar_reference_frame< affine_space_float_3d >::point_type   polar_point_float_3d;
    typedef polar_reference_frame< affine_space_double_2d >::point_type  polar_point_double_2d;
    typedef polar_reference_frame< affine_space_double_3d >::point_type  polar_point_double_3d;
    typedef polar_reference_frame< affine_space_int_2d >::point_type     polar_point_int_2d;
    typedef polar_reference_frame< affine_space_int_3d >::point_type     polar_point_int_3d;
    typedef polar_reference_frame< affine_space_int64_2d >::point_type   polar_point_int64_2d;
    typedef polar_reference_frame< affine_space_int64_3d >::point_type   polar_point_int64_3d;

    typedef polar_reference_frame< affine_space_float_2d >::vector_type  polar_vector_float_2d;
    typedef polar_reference_frame< affine_space_float_3d >::vector_type  polar_vector_float_3d;
    typedef polar_reference_frame< affine_space_double_2d >::vector_type polar_vector_double_2d;
    typedef polar_reference_frame< affine_space_double_3d >::vector_type polar_vector_double_3d;
    typedef polar_reference_frame< affine_space_int_2d >::vector_type    polar_vector_int_2d;
    typedef polar_reference_frame< affine_space_int_3d >::vector_type    polar_vector_int_3d;
    typedef polar_reference_frame< affine_space_int64_2d >::vector_type  polar_vector_int64_2d;
    typedef polar_reference_frame< affine_space_int64_3d >::vector_type  polar_vector_int64_3d;

    template <>
    struct has_compile_time_access< polar_point_float_2d > : boost::true_type {};
    template <>
    struct has_run_time_access< polar_point_float_2d > : boost::true_type {};
    template <>
    struct has_compile_time_access< polar_point_float_3d > : boost::true_type {};
    template <>
    struct has_run_time_access< polar_point_float_3d > : boost::true_type {};

    template <>
    struct has_compile_time_access< polar_point_double_2d > : boost::true_type {};
    template <>
    struct has_run_time_access< polar_point_double_2d > : boost::true_type {};
    template <>
    struct has_compile_time_access< polar_point_double_3d > : boost::true_type {};
    template <>
    struct has_run_time_access< polar_point_double_3d > : boost::true_type {};

    template <>
    struct has_compile_time_access< polar_point_int_2d > : boost::true_type {};
    template <>
    struct has_run_time_access< polar_point_int_2d > : boost::true_type {};
    template <>
    struct has_compile_time_access< polar_point_int_3d > : boost::true_type {};
    template <>
    struct has_run_time_access< polar_point_int_3d > : boost::true_type {};

    template <>
    struct has_compile_time_access< polar_point_int64_2d > : boost::true_type {};
    template <>
    struct has_run_time_access< polar_point_int64_2d > : boost::true_type {};
    template <>
    struct has_compile_time_access< polar_point_int64_3d > : boost::true_type {};
    template <>
    struct has_run_time_access< polar_point_int64_3d > : boost::true_type {};

    template <>
    struct has_compile_time_access< polar_vector_float_2d > : boost::true_type {};
    template <>
    struct has_run_time_access< polar_vector_float_2d > : boost::true_type {};
    template <>
    struct has_compile_time_access< polar_vector_float_3d > : boost::true_type {};
    template <>
    struct has_run_time_access< polar_vector_float_3d > : boost::true_type {};

    template <>
    struct has_compile_time_access< polar_vector_double_2d > : boost::true_type {};
    template <>
    struct has_run_time_access< polar_vector_double_2d > : boost::true_type {};
    template <>
    struct has_compile_time_access< polar_vector_double_3d > : boost::true_type {};
    template <>
    struct has_run_time_access< polar_vector_double_3d > : boost::true_type {};

    template <>
    struct has_compile_time_access< polar_vector_int_2d > : boost::true_type {};
    template <>
    struct has_run_time_access< polar_vector_int_2d > : boost::true_type {};
    template <>
    struct has_compile_time_access< polar_vector_int_3d > : boost::true_type {};
    template <>
    struct has_run_time_access< polar_vector_int_3d > : boost::true_type {};

    template <>
    struct has_compile_time_access< polar_vector_int64_2d > : boost::true_type {};
    template <>
    struct has_run_time_access< polar_vector_int64_2d > : boost::true_type {};
    template <>
    struct has_compile_time_access< polar_vector_int64_3d > : boost::true_type {};
    template <>
    struct has_run_time_access< polar_vector_int64_3d > : boost::true_type {};

    BOOST_DEFINE_POINT_TRAITS( polar_point_float_2d );
    BOOST_DEFINE_POINT_TRAITS( polar_point_float_3d );
    BOOST_DEFINE_POINT_TRAITS( polar_point_double_2d );
    BOOST_DEFINE_POINT_TRAITS( polar_point_double_3d );

    BOOST_DEFINE_CARTESIAN_ACCESS_TRAITS( polar_point_float_2d );
    BOOST_DEFINE_CARTESIAN_ACCESS_TRAITS( polar_point_float_3d );
    BOOST_DEFINE_CARTESIAN_ACCESS_TRAITS( polar_point_double_2d );
    BOOST_DEFINE_CARTESIAN_ACCESS_TRAITS( polar_point_double_3d );

    BOOST_DEFINE_POLAR_ACCESS_TRAITS( polar_point_float_2d );
    BOOST_DEFINE_POLAR_ACCESS_TRAITS( polar_point_float_3d );
    BOOST_DEFINE_POLAR_ACCESS_TRAITS( polar_point_double_2d );
    BOOST_DEFINE_POLAR_ACCESS_TRAITS( polar_point_double_3d );

    BOOST_DEFINE_POINT_TRAITS( polar_point_int_2d );
    BOOST_DEFINE_POINT_TRAITS( polar_point_int_3d );
    BOOST_DEFINE_POINT_TRAITS( polar_point_int64_2d );
    BOOST_DEFINE_POINT_TRAITS( polar_point_int64_3d );

    BOOST_DEFINE_CARTESIAN_ACCESS_TRAITS( polar_point_int_2d );
    BOOST_DEFINE_CARTESIAN_ACCESS_TRAITS( polar_point_int_3d );
    BOOST_DEFINE_CARTESIAN_ACCESS_TRAITS( polar_point_int64_2d );
    BOOST_DEFINE_CARTESIAN_ACCESS_TRAITS( polar_point_int64_3d );

    BOOST_DEFINE_POLAR_ACCESS_TRAITS( polar_point_int_2d );
    BOOST_DEFINE_POLAR_ACCESS_TRAITS( polar_point_int_3d );
    BOOST_DEFINE_POLAR_ACCESS_TRAITS( polar_point_int64_2d );
    BOOST_DEFINE_POLAR_ACCESS_TRAITS( polar_point_int64_3d );

    BOOST_DEFINE_VECTOR_TRAITS( polar_vector_float_2d );
    BOOST_DEFINE_VECTOR_TRAITS( polar_vector_float_3d );
    BOOST_DEFINE_VECTOR_TRAITS( polar_vector_double_2d );
    BOOST_DEFINE_VECTOR_TRAITS( polar_vector_double_3d );

    BOOST_DEFINE_CARTESIAN_VECTOR_ACCESS_TRAITS( polar_vector_float_2d );
    BOOST_DEFINE_CARTESIAN_VECTOR_ACCESS_TRAITS( polar_vector_float_3d );
    BOOST_DEFINE_CARTESIAN_VECTOR_ACCESS_TRAITS( polar_vector_double_2d );
    BOOST_DEFINE_CARTESIAN_VECTOR_ACCESS_TRAITS( polar_vector_double_3d );

    BOOST_DEFINE_VECTOR_TRAITS( polar_vector_int_2d );
    BOOST_DEFINE_VECTOR_TRAITS( polar_vector_int_3d );
    BOOST_DEFINE_VECTOR_TRAITS( polar_vector_int64_2d );
    BOOST_DEFINE_VECTOR_TRAITS( polar_vector_int64_3d );

    BOOST_DEFINE_CARTESIAN_VECTOR_ACCESS_TRAITS( polar_vector_int_2d );
    BOOST_DEFINE_CARTESIAN_VECTOR_ACCESS_TRAITS( polar_vector_int_3d );
    BOOST_DEFINE_CARTESIAN_VECTOR_ACCESS_TRAITS( polar_vector_int64_2d );
    BOOST_DEFINE_CARTESIAN_VECTOR_ACCESS_TRAITS( polar_vector_int64_3d );


}}}//namespace boost::numeric::geometry;

#endif //_BOOST_GEOMETRY_POLAR_REFERENCE_FRAME_HPP
