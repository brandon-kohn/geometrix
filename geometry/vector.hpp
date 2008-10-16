//
//!  Copyright (c) 2008
//!  Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_GEOMETRY_VECTOR_HPP
#define _BOOST_GEOMETRY_VECTOR_HPP
#pragma once

#include "vector_traits.hpp"
#include "math_functions.hpp"

#include <boost/mpl/assert.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/utility.hpp>
#include <boost/operators.hpp>

#  include <boost/config.hpp>
#  include <boost/preprocessor/enum.hpp>
#  include <boost/preprocessor/enum_params.hpp>
#  include <boost/preprocessor/cat.hpp>
#  include <boost/preprocessor/repeat.hpp>
#  include <boost/preprocessor/repeat_from_to.hpp>
#  include <boost/preprocessor/arithmetic/add.hpp>
#  include <boost/preprocessor/inc.hpp>

#include <functional>

#define BOOST_GEOMETRY_VECTOR_COMMA ,
#define BOOST_GEOMETRY_VECTOR_SEMICOLON ;
#define BOOST_GEOMETRY_VECTOR_COLON :

/////////////////////////////////////////////////////////////////////////////
//
// FORWARD DECLARATION
//
/////////////////////////////////////////////////////////////////////////////

#define BOOST_GEOMETRY_VECTOR_MAX_DIMENSION 20

#define BOOST_GEOMETRY_VECTOR_INIT_DIMENSION( z, n, _ ) \
    get< ## n >() = BOOST_PP_CAT( a, n ) BOOST_GEOMETRY_VECTOR_SEMICOLON

#define BOOST_GEOMETRY_VECTOR_CTOR_ARGS( z, n, _ ) \
    const coordinate_type& BOOST_PP_CAT( a, n )

#define BOOST_GEOMETRY_VECTOR_CTOR( z, n, _ ) \
    explicit vector( BOOST_PP_ENUM( n, BOOST_GEOMETRY_VECTOR_CTOR_ARGS, nil) ) { BOOST_PP_REPEAT( n, BOOST_GEOMETRY_VECTOR_INIT_DIMENSION, nil ) }

namespace boost
{
namespace numeric
{
namespace geometry
{

//! Enum for common dimension names in cartesian coordinates.
enum cartesian_dimension
{
	e_x = 0,
	e_y = 1,
	e_z = 2
};

//! Enum for common dimension names in polar/spherical coordinates.
enum polar_dimension
{
	e_r = 0,
	e_theta = 1,
	e_phi = 2
};

/////////////////////////////////////////////////////////////////////////////
//
// CLASS vector
//! A template class for specifying vector types with a specialized coordinate
//! type and dimension. A constraint on vector dimensions is enforced to avoid
//! instantiating vectors with unsupported dimensionality.
//!
namespace detail
{
    struct VECTOR_DIMENSION_INDEX_OUT_OF_BOUNDS{};

    template <typename Coordinate, unsigned int N>
    struct vector_impl
    {
        typedef Coordinate        head;
        typedef vector_impl< Coordinate, N-1 > tail;

        head& get_head() const { return m_value; }
        tail& get_tail() const { return m_tail; }       

        mutable head m_value;
        mutable tail m_tail;
    };

    template <typename Coordinate>
    struct vector_impl<Coordinate, 0>
    {
        typedef Coordinate head;
        typedef VECTOR_DIMENSION_INDEX_OUT_OF_BOUNDS  tail;

        head& get_head() const { return m_value; }
        tail& get_tail() const { return m_tail; }

        mutable head m_value;
        mutable VECTOR_DIMENSION_INDEX_OUT_OF_BOUNDS m_tail;
    };

    template <unsigned int N>
    struct get_
    {
        template <typename Coordinate, typename VectorImpl>
        inline static Coordinate& get( VectorImpl& p )
        {
            return get_<N-1>::get<Coordinate>( p.get_tail() ); 
        }

        template <typename Coordinate, typename VectorImpl>
        inline static Coordinate get( const VectorImpl& p )
        {
            return get_<N-1>::get<Coordinate>( p.get_tail() ); 
        }
    };

    template <>
    struct get_<0>
    {
        template <typename Coordinate, typename VectorImpl>
        inline static Coordinate& get( VectorImpl& p )
        {
            return p.get_head(); 
        }
         
        template <typename Coordinate, typename VectorImpl>
        inline static Coordinate get( const VectorImpl& p )
        {
            return p.get_head(); 
        }
    };

    template <typename Vector, unsigned int N>
    struct binary_operation_
    {
        typedef Vector vector_type;
        typedef typename vector_traits< Vector >::coordinate_type coordinate_type;
        typedef typename vector_traits< Vector >::dimenstion_type dimension_type;
        typedef point< coordinate_type, dimension_type::value >   point_type;

        template <typename Operation, typename VectorImpl>
        inline static void do_( VectorImpl& lhs, const vector_type& rhs )
        {
            lhs.get_head() = Operation()( lhs.get_head(), rhs.get<N>() );
            binary_operation_<vector_type, N-1>::do_<Operation>( lhs.get_tail(), rhs ); 
        }        
        
        template <typename Operation, typename VectorImpl>
        inline static void do_( VectorImpl& lhs, const point_type& rhs )
        {
            lhs.get_head() = Operation()( lhs.get_head(), rhs.get<N>() );
            binary_operation_<vector_type, N-1>::do_<Operation>( lhs.get_tail(), rhs ); 
        }      

        template <typename Operation, typename VectorImpl>
        inline static void do_( VectorImpl& lhs, const coordinate_type& rhs )
        {
            lhs.get_head() = Operation()( lhs.get_head(), rhs );
            binary_operation_<vector_type, N-1>::do_<Operation>( lhs.get_tail(), rhs ); 
        }    
    };

    template <typename Vector>
    struct binary_operation_<Vector, 0>
    {
        typedef Vector vector_type;
        typedef typename vector_traits< Vector >::coordinate_type coordinate_type;
        typedef typename vector_traits< Vector >::dimenstion_type dimension_type;
        typedef point< coordinate_type, dimension_type::value >   point_type;

        template <typename Operation, typename VectorImpl>
        inline static void do_( VectorImpl& lhs, const vector_type& rhs )
        {
            lhs.get_head() = Operation()( lhs.get_head(), rhs.get<0>() );            
        }

        template <typename Operation, typename VectorImpl>
        inline static void do_( VectorImpl& lhs, const point_type& rhs )
        {
            lhs.get_head() = Operation()( lhs.get_head(), rhs.get<0>() );            
        }

        template <typename Operation, typename VectorImpl>
        inline static void do_( VectorImpl& lhs, const coordinate_type& rhs )
        {2
            lhs.get_head() = Operation()( lhs.get_head(), rhs );            
        }
    };

    template <unsigned int N>
    struct unary_operation_
    {
        template <typename Operation, typename VectorImpl, typename Vector>
        inline static void do_( VectorImpl& lhs, const Vector& rhs )
        {
            lhs.get_head() = Operation()( rhs.get<N>() );
            unary_operation_<N-1>::do_<Operation>( lhs.get_tail(), rhs ); 
        }        

        template <typename Operation, typename VectorImpl, typename Vector>
        inline static void do_( VectorImpl& lhs )
        {
            lhs.get_head() = Operation()( lhs.get_head() );
            unary_operation_<N-1>::do_<Operation>( lhs.get_tail() ); 
        }      
    };

    template <>
    struct unary_operation_<0>
    {
        template <typename Operation, typename VectorImpl, typename Vector>
        inline static void do_( VectorImpl& lhs, const Vector& rhs )
        {
            lhs.get_head() = Operation()( rhs.get<0>() );            
        }

        template <typename Operation, typename VectorImpl, typename Vector>
        inline static void do_( VectorImpl& lhs )
        {
            lhs.get_head() = Operation()( lhs.get_head() );            
        }   
    };

    template <typename Coordinate>
    struct identity_right
	{
        template <typename T1, typename T2>
        Coordinate operator()( const T1& lhs, const T2& rhs ) const
		{            
            return rhs;
		}
	};

    template <typename Coordinate>
    struct identity_left
	{
        template <typename T1, typename T2>
        Coordinate operator()( const T1& lhs, const T2& rhs ) const
		{            
            return lhs;
		}
	};
}
template <typename Coordinate, unsigned int Dimension>
class vector : boost::addable< vector<Coordinate, Dimension>                                    // vector + vector
             , boost::subtractable< vector<Coordinate, Dimension>                               // vector - vector
             , boost::dividable2< vector<Coordinate, Dimension>, Coordinate                     // vector / Coordinate
             , boost::multipliable2< vector<Coordinate, Dimension>, Coordinate                  // vector * Coordinate, Coordinate * vector
             , boost::addable2< vector<Coordinate, Dimension>, point<Coordinate, Dimension      // vector + point
             , boost::subtractable2< vector<Coordinate, Dimension>, point<Coordinate, Dimension // vector - point
             > > > > > >
{
public:

	typedef Coordinate                  coordinate_type;
	typedef dimension_traits<Dimension> dimension_type;

    //! define the constructors via the preprocessor.
    BOOST_PP_REPEAT_FROM_TO( 0, BOOST_GEOMETRY_VECTOR_MAX_DIMENSION, BOOST_GEOMETRY_VECTOR_CTOR, nil ) 

    template <unsigned int D>
    coordinate_type get() const
    {        
        BOOST_MPL_ASSERT_MSG
        (
           ( dimension_traits< D >::value >= 0 && dimension_traits< D >::value < dimension_type::value )
		   , VECTOR_GET_CALLED_WITH_INDEX_OUT_OF_BOUNDS
		   , ( dimension_traits< D > )
        ); 

        return detail::get_<D>::get<coordinate_type>( m_coordinates );
    }

    template <unsigned int D>
    coordinate_type& get() 
    {
        BOOST_MPL_ASSERT_MSG
        (
           ( dimension_traits< D >::value >= 0 && dimension_traits< D >::value < dimension_type::value )
		   , VECTOR_GET_CALLED_WITH_INDEX_OUT_OF_BOUNDS
		   , ( dimension_traits< D > )
        );

        return detail::get_<D>::get<coordinate_type>( m_coordinates );
    }

    //! Operator interface
    inline vector operator+= ( const vector& v ){ detail::binary_operation_< vector, dimension_type::value - 1 >::do_< std::plus< coordinate_type > >( m_coordinates, v ); return *this; }
    // vector operator+(vector, const vector&) automatically
    // generated by addable.

    inline vector operator-= ( const vector& v ){detail::binary_operation_< vector, dimension_type::value - 1 >::do_< std::minus< coordinate_type > >( m_coordinates, v ); return *this; }
    // vector operator-(vector, const vector&) automatically
    // generated by subtractable.

    //! Operator interface
    inline vector operator+= ( const point<Coordinate, Dimension>& p ){ detail::binary_operation_< vector, dimension_type::value - 1 >::do_< std::plus< coordinate_type > >( m_coordinates, p ); return *this; }
    // vector operator+(vector, const vector&) automatically
    // generated by addable.

    inline vector operator-= ( const point<Coordinate, Dimension>& p ){detail::binary_operation_< vector, dimension_type::value - 1 >::do_< std::minus< coordinate_type > >( m_coordinates, p ); return *this; }
    // vector operator-(vector, const vector&) automatically
    // generated by subtractable.

	inline vector operator*= ( const coordinate_type& v ) { detail::binary_operation_< vector, dimension_type::value - 1 >::do_< std::multiplies< coordinate_type > >( m_coordinates, v ); return *this; }
    // vector operator*(vector, const T&) and
    // vector operator*(const T&, vector) auto-generated
    // by multipliable.

	inline vector operator/= ( const coordinate_type& v ) {detail::binary_operation_< vector, dimension_type::value - 1 >::do_< std::divides< coordinate_type > >( m_coordinates, v ); return *this; }
    // vector operator/(vector, const T&) auto-generated
    // by dividable.

private:    

    detail::vector_impl< coordinate_type, dimension_type::value > m_coordinates;

};
                  
//! Concrete Vector Types for some of the more common coordinate types.
typedef vector<float, 2>       vector_float_2d;
typedef vector<float, 3>       vector_float_3d;
typedef vector<double, 2>      vector_double_2d;
typedef vector<double, 3>      vector_double_3d;

typedef vector<int, 2>         vector_int_2d;
typedef vector<int, 3>         vector_int_3d;
typedef vector<long long, 2>   vector_int64_2d;
typedef vector<long long, 3>   vector_int64_3d;

//! Specialize the coordinate accessors for cartesian coords.
#define BOOST_DEFINE_CARTESIAN_VECTOR_ACCESS_TRAITS( Vector )            \
template <>                                                              \
struct cartesian_access_traits< Vector >                                 \
{                                                                        \
    typedef Vector                                      vector_type;     \
    typedef vector_traits<vector_type>::coordinate_type coordinate_type; \
    typedef vector_traits<vector_type>::dimension_type  dimension_type;  \
                                                                         \
    template <unsigned int Index>                                        \
    static inline coordinate_type get( const vector_type& p ) { return p.get<Index>(); }\
                                                                                        \
    template <typename T>                                                               \
    static inline typename boost::enable_if< boost::is_same< typename vector_traits<T>::dimension_type, dimension_traits<2> >, vector_type >::type construct( const coordinate_type& x, const coordinate_type& y ) { return vector_type( x, y ); }\
    template <class T>                                                                  \
    static inline typename boost::disable_if< boost::is_same< typename vector_traits<T>::dimension_type, dimension_traits<2> >, vector_type >::type construct( const coordinate_type& x, const coordinate_type& y, const coordinate_type& z ) { return vector_type( x, y, z ); }\
};

template <unsigned int Index, typename Vector, typename Dimension>
struct polar_coordinate_get
{};

template<typename Vector>
struct polar_coordinate_get<0, Vector, dimension_traits<2> >
{
    //! Access the radius.
    static typename vector_traits<Vector>::coordinate_type get( const Vector& v )
    {
        typedef Vector                                          vector_type;                 
        typedef typename vector_traits<Vector>::coordinate_type coordinate_type;
        return math_functions< coordinate_type >::sqrt( v.get<e_x>() * v.get<e_x>() +
                                                        v.get<e_y>() * v.get<e_y>() );
    }

};

template<typename Vector>
struct polar_coordinate_get<0, Vector, dimension_traits<3> >
{
    //! Access the radius.
    static typename vector_traits<Vector>::coordinate_type get( const Vector& v )
    {
        typedef Vector                                          vector_type;                     
        typedef typename vector_traits<Vector>::coordinate_type coordinate_type;
        return math_functions< coordinate_type >::sqrt( v.get<e_x>() * v.get<e_x>() +
                                                        v.get<e_y>() * v.get<e_y>() +
                                                        v.get<e_z>() * v.get<e_z>() ); 
    }     
};

template<typename Vector, typename Dimension>
struct polar_coordinate_get<1, Vector, Dimension>
{
    typedef Vector                                               vector_type;                     
    typedef typename vector_traits<vector_type>::coordinate_type coordinate_type;                
    typedef Dimension                                            dimension_type; 

    //! Access theta
    static inline coordinate_type get( const vector_type& v ) { return math_functions< coordinate_type >::atan( v.get<1>() / v.get<0>() ); }  \
};

template<typename Vector>
struct polar_coordinate_get<2, Vector, dimension_traits<3> >
{
    typedef Vector                                               vector_type;                     
    typedef typename vector_traits<vector_type>::coordinate_type coordinate_type;                
    typedef typename vector_traits<vector_type>::dimension_type  dimension_type; 

    //! Access phi
    static inline coordinate_type get( const Vector& v )
    {
        return math_functions< coordinate_type >::atan( math_functions< coordinate_type >::sqrt( v.get<e_x>()*v.get<e_x>() + v.get<e_y>() * v.get<e_y>() ) / v.get<e_z>() ); 
    } 
};

//! Specialize the coordinate accessors for polar coords.
#define BOOST_DEFINE_POLAR_VECTOR_ACCESS_TRAITS( Vector )               \
template <>                                                             \
struct polar_access_traits< Vector >                                    \
{                                                                       \
    typedef Vector                                      vector_type;    \
    typedef vector_traits<vector_type>::coordinate_type coordinate_type;\
    typedef vector_traits<vector_type>::dimension_type  dimension_type; \
                                                                        \
    template <unsigned int Index>                                       \
    static inline coordinate_type get( const vector_type& p ) { return polar_coordinate_get<Index, Vector, dimension_type>::get( p ); }\
    template <typename T>                                               \
    static inline typename boost::enable_if< boost::is_same< typename vector_traits<T>::dimension_type, dimension_traits<2> >, vector_type >::type construct( const coordinate_type& r, const coordinate_type& t ) { return vector_type( r*math_functions< coordinate_type >::cos(t), r*math_functions< coordinate_type >::sin(t) ); }\
    template <typename T>                                               \
    static inline typename boost::disable_if< boost::is_same< typename vector_traits<T>::dimension_type, dimension_traits<2> >, vector_type >::type construct( const coordinate_type& r, const coordinate_type& t, const coordinate_type& phi ) { return vector_type( r*math_functions< coordinate_type >::cos(t)*math_functions< coordinate_type >::sin(phi), r*math_functions< coordinate_type >::sin(t)*math_functions< coordinate_type >::sin(phi), r*math_functions< coordinate_type >::cos(phi) ); }\
};

BOOST_DEFINE_VECTOR_TRAITS( vector_float_2d );
BOOST_DEFINE_VECTOR_TRAITS( vector_float_3d );
BOOST_DEFINE_VECTOR_TRAITS( vector_double_2d );
BOOST_DEFINE_VECTOR_TRAITS( vector_double_3d );

BOOST_DEFINE_CARTESIAN_VECTOR_ACCESS_TRAITS( vector_float_2d );
BOOST_DEFINE_CARTESIAN_VECTOR_ACCESS_TRAITS( vector_float_3d );
BOOST_DEFINE_CARTESIAN_VECTOR_ACCESS_TRAITS( vector_double_2d );
BOOST_DEFINE_CARTESIAN_VECTOR_ACCESS_TRAITS( vector_double_3d );

BOOST_DEFINE_POLAR_VECTOR_ACCESS_TRAITS( vector_float_2d );
BOOST_DEFINE_POLAR_VECTOR_ACCESS_TRAITS( vector_float_3d );
BOOST_DEFINE_POLAR_VECTOR_ACCESS_TRAITS( vector_double_2d );
BOOST_DEFINE_POLAR_VECTOR_ACCESS_TRAITS( vector_double_3d );

BOOST_DEFINE_VECTOR_TRAITS( vector_int_2d );
BOOST_DEFINE_VECTOR_TRAITS( vector_int_3d );
BOOST_DEFINE_VECTOR_TRAITS( vector_int64_2d );
BOOST_DEFINE_VECTOR_TRAITS( vector_int64_3d );

BOOST_DEFINE_CARTESIAN_VECTOR_ACCESS_TRAITS( vector_int_2d );
BOOST_DEFINE_CARTESIAN_VECTOR_ACCESS_TRAITS( vector_int_3d );
BOOST_DEFINE_CARTESIAN_VECTOR_ACCESS_TRAITS( vector_int64_2d );
BOOST_DEFINE_CARTESIAN_VECTOR_ACCESS_TRAITS( vector_int64_3d );

BOOST_DEFINE_POLAR_VECTOR_ACCESS_TRAITS( vector_int_2d );
BOOST_DEFINE_POLAR_VECTOR_ACCESS_TRAITS( vector_int_3d );
BOOST_DEFINE_POLAR_VECTOR_ACCESS_TRAITS( vector_int64_2d );
BOOST_DEFINE_POLAR_VECTOR_ACCESS_TRAITS( vector_int64_3d );

}}}//namespace boost::numeric::geometry;

#endif //_BOOST_GEOMETRY_VECTOR_HPP
