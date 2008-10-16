//
//!  Copyright (c) 2008
//!  Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_GEOMETRY_POINT_HPP
#define _BOOST_GEOMETRY_POINT_HPP
#pragma once

#include "point_traits.hpp"
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

#define BOOST_GEOMETRY_POINT_COMMA ,
#define BOOST_GEOMETRY_POINT_SEMICOLON ;
#define BOOST_GEOMETRY_POINT_COLON :

/////////////////////////////////////////////////////////////////////////////
//
// FORWARD DECLARATION
//
/////////////////////////////////////////////////////////////////////////////

#define BOOST_GEOMETRY_POINT_MAX_DIMENSION 20

#define BOOST_GEOMETRY_POINT_INIT_DIMENSION( z, n, _ ) \
    get< ## n >() = BOOST_PP_CAT( a, n ) BOOST_GEOMETRY_POINT_SEMICOLON

#define BOOST_GEOMETRY_POINT_CTOR_ARGS( z, n, _ ) \
    const coordinate_type& BOOST_PP_CAT( a, n )

#define BOOST_GEOMETRY_POINT_CTOR( z, n, _ ) \
    explicit point( BOOST_PP_ENUM( n, BOOST_GEOMETRY_POINT_CTOR_ARGS, nil) ) { BOOST_PP_REPEAT( n, BOOST_GEOMETRY_POINT_INIT_DIMENSION, nil ) }

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
// CLASS point
//! A template class for specifying point types with a specialized coordinate
//! type and dimension. A constraint on point dimensions is enforced to avoid
//! instantiating points with unsupported dimensionality.
//!
namespace detail
{
    struct POINT_DIMENSION_INDEX_OUT_OF_BOUNDS{};

    template <typename Coordinate, unsigned int N>
    struct point_impl
    {
        typedef Coordinate        head;
        typedef point_impl< Coordinate, N-1 > tail;

        head& get_head() const { return m_value; }
        tail& get_tail() const { return m_tail; }       

        mutable head m_value;
        mutable tail m_tail;
    };

    template <typename Coordinate>
    struct point_impl<Coordinate, 0>
    {
        typedef Coordinate head;
        typedef POINT_DIMENSION_INDEX_OUT_OF_BOUNDS  tail;

        head& get_head() const { return m_value; }
        tail& get_tail() const { return m_tail; }

        mutable head m_value;
        mutable POINT_DIMENSION_INDEX_OUT_OF_BOUNDS m_tail;
    };

    template <unsigned int N>
    struct get_
    {
        template <typename Coordinate, typename PointImpl>
        inline static Coordinate& get( PointImpl& p )
        {
            return get_<N-1>::get<Coordinate>( p.get_tail() ); 
        }

        template <typename Coordinate, typename PointImpl>
        inline static Coordinate get( const PointImpl& p )
        {
            return get_<N-1>::get<Coordinate>( p.get_tail() ); 
        }
    };

    template <>
    struct get_<0>
    {
        template <typename Coordinate, typename PointImpl>
        inline static Coordinate& get( PointImpl& p )
        {
            return p.get_head(); 
        }
         
        template <typename Coordinate, typename PointImpl>
        inline static Coordinate get( const PointImpl& p )
        {
            return p.get_head(); 
        }
    };

    template <typename Point, unsigned int N>
    struct binary_operation_
    {
        typedef Point point_type;
        typedef typename point_traits< Point >::coordinate_type coordinate_type;

        template <typename Operation, typename PointImpl>
        inline static void do_( PointImpl& lhs, const point_type& rhs )
        {
            lhs.get_head() = Operation()( lhs.get_head(), rhs.get<N>() );
            binary_operation_<point_type, N-1>::do_<Operation>( lhs.get_tail(), rhs ); 
        }        

        template <typename Operation, typename PointImpl>
        inline static void do_( PointImpl& lhs, const coordinate_type& rhs )
        {
            lhs.get_head() = Operation()( lhs.get_head(), rhs );
            binary_operation_<point_type, N-1>::do_<Operation>( lhs.get_tail(), rhs ); 
        }    
    };

    template <typename Point>
    struct binary_operation_<Point, 0>
    {
        typedef Point point_type;
        typedef typename point_traits< Point >::coordinate_type coordinate_type;

        template <typename Operation, typename PointImpl>
        inline static void do_( PointImpl& lhs, const point_type& rhs )
        {
            lhs.get_head() = Operation()( lhs.get_head(), rhs.get<0>() );            
        }

        template <typename Operation, typename PointImpl>
        inline static void do_( PointImpl& lhs, const coordinate_type& rhs )
        {
            lhs.get_head() = Operation()( lhs.get_head(), rhs );            
        }
    };

    template <unsigned int N>
    struct unary_operation_
    {
        template <typename Operation, typename PointImpl, typename Point>
        inline static void do_( PointImpl& lhs, const Point& rhs )
        {
            lhs.get_head() = Operation()( rhs.get<N>() );
            unary_operation_<N-1>::do_<Operation>( lhs.get_tail(), rhs ); 
        }        

        template <typename Operation, typename PointImpl, typename Point>
        inline static void do_( PointImpl& lhs )
        {
            lhs.get_head() = Operation()( lhs.get_head() );
            unary_operation_<N-1>::do_<Operation>( lhs.get_tail() ); 
        }      
    };

    template <>
    struct unary_operation_<0>
    {
        template <typename Operation, typename PointImpl, typename Point>
        inline static void do_( PointImpl& lhs, const Point& rhs )
        {
            lhs.get_head() = Operation()( rhs.get<0>() );            
        }

        template <typename Operation, typename PointImpl, typename Point>
        inline static void do_( PointImpl& lhs )
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
class point : boost::addable< point<Coordinate, Dimension>                   // point + point
            , boost::subtractable< point<Coordinate, Dimension>              // point - point
            , boost::dividable2< point<Coordinate, Dimension>, Coordinate    // point / T
            , boost::multipliable2< point<Coordinate, Dimension>, Coordinate // point * T, T * point
            > > > >
{
public:	

	typedef Coordinate                  coordinate_type;
	typedef dimension_traits<Dimension> dimension_type;

    //! define the constructors via the preprocessor.
    BOOST_PP_REPEAT_FROM_TO( 0, BOOST_GEOMETRY_POINT_MAX_DIMENSION, BOOST_GEOMETRY_POINT_CTOR, nil ) 

    template <unsigned int D>
    coordinate_type get() const
    {        
        BOOST_MPL_ASSERT_MSG
        (
           ( dimension_traits< D >::value >= 0 && dimension_traits< D >::value < dimension_type::value )
		   , POINT_GET_CALLED_WITH_INDEX_OUT_OF_BOUNDS
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
		   , POINT_GET_CALLED_WITH_INDEX_OUT_OF_BOUNDS
		   , ( dimension_traits< D > )
        );

        return detail::get_<D>::get<coordinate_type>( m_coordinates );
    }

    //! Operator interface
    inline point operator+= ( const point& p ){ detail::binary_operation_< point, dimension_type::value - 1 >::do_< std::plus< coordinate_type > >( m_coordinates, p ); return *this; }
    // point operator+(point, const point&) automatically
    // generated by addable.

    inline point operator-= ( const point& p ){detail::binary_operation_< point, dimension_type::value - 1 >::do_< std::minus< coordinate_type > >( m_coordinates, p ); return *this; }
    // point operator-(point, const point&) automatically
    // generated by subtractable.

	inline point operator*= ( const coordinate_type& v ) { detail::binary_operation_< point, dimension_type::value - 1 >::do_< std::multiplies< coordinate_type > >( m_coordinates, v ); return *this; }
    // point operator*(point, const T&) and
    // point operator*(const T&, point) auto-generated
    // by multipliable.

	inline point operator/= ( const coordinate_type& v ) {detail::binary_operation_< point, dimension_type::value - 1 >::do_< std::divides< coordinate_type > >( m_coordinates, v ); return *this; }
    // point operator/(point, const T&) auto-generated
    // by dividable.

private:    

    detail::point_impl< coordinate_type, dimension_type::value > m_coordinates;
	
};

/*
//! 2D point specialization.
template <typename Coordinate>
class point<Coordinate, 2> : boost::addable< point<Coordinate,2>    // point + point
            , boost::subtractable< point<Coordinate,2>              // point - point
            , boost::dividable2< point<Coordinate,2>, Coordinate    // point / T
            , boost::multipliable2< point<Coordinate,2>, Coordinate // point * T, T * point
            > > > >
{
public:

	typedef Coordinate        coordinate_type;
	typedef dimension_traits<2>  dimension_type;

	//! Default Constructor
	explicit point( coordinate_type x = 0, coordinate_type y = 0 )
        : m_x( x ), 
          m_y( y )
	{}

	~point(){}

	//! Access the X coordinate
	inline coordinate_type get<e_x>() const { return m_x; }

	//! Access the Y coordinate
	inline coordinate_type get<e_y>() const { return m_y; }

	//! Mutate the X coordinate
	inline void            set_x( coordinate_type x ) { m_x = x; }

	//! Mutate the Y coordinate
	inline void            set_y( coordinate_type y ) { m_y = y; }

	//! dimensional index accessor
	inline coordinate_type get_coordinate( cartesian_dimension dimension ) const 
	{
		assert( dimension >= e_x && dimension <= e_y );
		if( dimension == e_x )
            return m_x;
        else
            return m_y;
	}

	//! dimensional index accessor
	inline coordinate_type operator []( cartesian_dimension dimension ) const { return get_coordinate( dimension ); }

	//! dimensional index accessor
	inline coordinate_type operator ()( cartesian_dimension dimension ) const { return get_coordinate( dimension ); }

	//! dimensional index mutator
	inline void            set_coordinate( cartesian_dimension dimension, coordinate_type value ) 
	{
		assert( dimension != e_z );
		if( dimension == e_x )
            m_x = value;
        else if( dimension == e_y )
            m_y = value;
	}

	//! Operator interface
	inline point operator+= ( const point& p ){ m_x += p.m_x; m_y += p.m_y; return *this; }
    // point operator+(point, const point&) automatically
    // generated by addable.

    inline point operator-= ( const point& p ){ m_x -= p.m_x; m_y -= p.m_y; return *this; }
    // point operator-(point, const point&) automatically
    // generated by subtractable.

	inline point operator*= ( const coordinate_type& v ) { m_x *= v; m_y *= v; return *this; }
    // point operator*(point, const T&) and
    // point operator*(const T&, point) auto-generated
    // by multipliable.

	inline point operator/= ( const coordinate_type& v ) { m_x /= v; m_y /= v; return *this; }
    // point operator/(point, const T&) auto-generated
    // by dividable.

private:

	coordinate_type m_x;
    coordinate_type m_y;

};

//! 3D point specialization.
template <typename Coordinate>
class point<Coordinate, 3> : boost::addable< point<Coordinate,3>    // point + point
            , boost::subtractable< point<Coordinate,3>              // point - point
            , boost::dividable2< point<Coordinate,3>, Coordinate    // point / T
            , boost::multipliable2< point<Coordinate,3>, Coordinate // point * T, T * point
            > > > >
{
public:

	typedef Coordinate           coordinate_type;
	typedef dimension_traits<3>  dimension_type;

	//! Default Constructor
	explicit point( coordinate_type x = 0, coordinate_type y = 0, coordinate_type z = 0 )
    : m_coordinates( dimension_type::value )
	{		
		m_coordinates( 0 ) = x;
		m_coordinates( 1 ) = y;
		m_coordinates( 2 ) = z;
	}

	~point(){}

	//! Access the X coordinate
	inline coordinate_type get<e_x>() const { return m_coordinates(0); }

	//! Access the Y coordinate
	inline coordinate_type get<e_y>() const { return m_coordinates(1); }

	//! Access the Z coordinate (if enabled)
	inline coordinate_type get<e_z>() const { return m_coordinates(2); }
	
	//! Mutate the X coordinate
	inline void            set_x( coordinate_type x ) { m_coordinates( 0 ) = x; }

	//! Mutate the Y coordinate
	inline void            set_y( coordinate_type y ) { m_coordinates( 1 ) = y; }

	//! Mutate the Z coordinate
	inline void            set_z( coordinate_type z ) { m_coordinates( 2 ) = z; }

	//! dimensional index accessor
	inline coordinate_type get_coordinate( cartesian_dimension dimension ) const { return m_coordinates( dimension ); }

	//! dimensional index accessor
	inline coordinate_type operator []( cartesian_dimension dimension ) const { return m_coordinates( dimension ); }

	//! dimensional index accessor
	inline coordinate_type operator ()( cartesian_dimension dimension ) const { return m_coordinates( dimension ); }

	//! dimensional index mutator
	inline void            set_coordinate( cartesian_dimension dimension, coordinate_type value ) { m_coordinates( dimension ) = value; }

    //! Operator interface
	inline point operator+= ( const point& p ){ ( m_coordinates += p.m_coordinates ); return *this; }
    // point operator+(point, const point&) automatically
    // generated by addable.

    inline point operator-= ( const point& p ){ ( m_coordinates -= p.m_coordinates ); return *this; }
    // point operator-(point, const point&) automatically
    // generated by subtractable.

	inline point operator*= ( const coordinate_type& v ) { ( m_coordinates *= v ); return *this; }
    // point operator*(point, const T&) and
    // point operator*(const T&, point) auto-generated
    // by multipliable.

	inline point operator/= ( const coordinate_type& v ) { ( m_coordinates /= v ); return *this; }
    // point operator/(point, const T&) auto-generated
    // by dividable.

private:

	boost::numeric::ublas::vector<coordinate_type> m_coordinates;

};
*/
                  
//! Concrete Point Types for some of the more common coordinate types.
typedef point<float, 2>       point_float_2d;
typedef point<float, 3>       point_float_3d;
typedef point<double, 2>      point_double_2d;
typedef point<double, 3>      point_double_3d;

typedef point<int, 2>         point_int_2d;
typedef point<int, 3>         point_int_3d;
typedef point<long long, 2>   point_int64_2d;
typedef point<long long, 3>   point_int64_3d;

//! Specialize the coordinate accessors for cartesian coords.
#define BOOST_DEFINE_CARTESIAN_ACCESS_TRAITS( Point )                                 \
template <>                                                                           \
struct cartesian_access_traits< Point >                                               \
{                                                                                     \
    typedef Point                                     point_type;                     \
    typedef point_traits<point_type>::coordinate_type coordinate_type;                \
    typedef point_traits<point_type>::dimension_type  dimension_type;                 \
    \
    template <unsigned int Index>                                                     \
    static inline coordinate_type get( const point_type& p ) { return p.get<Index>(); } \
    \
    template <typename T>                                                             \
    static inline typename boost::enable_if< boost::is_same< typename point_traits<T>::dimension_type, dimension_traits<2> >, point_type >::type construct( const coordinate_type& x, const coordinate_type& y ) { return point_type( x, y ); } \
    template <class T>                                                                \
    static inline typename boost::disable_if< boost::is_same< typename point_traits<T>::dimension_type, dimension_traits<2> >, point_type >::type construct( const coordinate_type& x, const coordinate_type& y, const coordinate_type& z ) { return point_type( x, y, z ); } \
};

template <unsigned int Index, typename Point, typename Dimension>
struct polar_coordinate_get
{};

template<typename Point>
struct polar_coordinate_get<0, Point, dimension_traits<2> >
{
    //! Access the radius.
    static typename point_traits<Point>::coordinate_type get( const Point& p )
    {
        typedef Point                                         point_type;                 
        typedef typename point_traits<Point>::coordinate_type coordinate_type;
        return math_functions< coordinate_type >::sqrt( p.get<e_x>() * p.get<e_x>() +
                                                        p.get<e_y>() * p.get<e_y>() );
    }

};

template<typename Point>
struct polar_coordinate_get<0, Point, dimension_traits<3> >
{
    //! Access the radius.
    static typename point_traits<Point>::coordinate_type get( const Point& p )
    {
        typedef Point                                         point_type;                     
        typedef typename point_traits<Point>::coordinate_type coordinate_type;
        return math_functions< coordinate_type >::sqrt( p.get<e_x>() * p.get<e_x>() +
                                                        p.get<e_y>() * p.get<e_y>() +
                                                        p.get<e_z>() * p.get<e_z>() ); 
    }     
};

template<typename Point, typename Dimension>
struct polar_coordinate_get<1, Point, Dimension>
{
    typedef Point                                              point_type;                     
    typedef typename point_traits<point_type>::coordinate_type coordinate_type;                
    typedef Dimension                                          dimension_type; 

    //! Access theta
    static inline coordinate_type get( const point_type& p ) { return math_functions< coordinate_type >::atan( p.get<1>() / p.get<0>() ); }  \
};

template<typename Point>
struct polar_coordinate_get<2, Point, dimension_traits<3> >
{
    typedef Point                                              point_type;                     
    typedef typename point_traits<point_type>::coordinate_type coordinate_type;                
    typedef typename point_traits<point_type>::dimension_type  dimension_type; 

    //! Access phi
    static inline coordinate_type get( const Point& p )
    {
        return math_functions< coordinate_type >::atan( math_functions< coordinate_type >::sqrt( p.get<e_x>()*p.get<e_x>() + p.get<e_y>() * p.get<e_y>() ) / p.get<e_z>() ); 
    } 
};

//! Specialize the coordinate accessors for polar coords.
#define BOOST_DEFINE_POLAR_ACCESS_TRAITS( Point )                                     \
template <>                                                                           \
struct polar_access_traits< Point >                                                   \
{                                                                                     \
    typedef Point                                     point_type;                     \
    typedef point_traits<point_type>::coordinate_type coordinate_type;                \
    typedef point_traits<point_type>::dimension_type  dimension_type;                 \
                                                                                      \
    template <unsigned int Index>                                                     \
    static inline coordinate_type get( const point_type& p ) { return polar_coordinate_get<Index, Point, dimension_type>::get( p ); } \
    template <typename T>                                                             \
    static inline typename boost::enable_if< boost::is_same< typename point_traits<T>::dimension_type, dimension_traits<2> >, point_type >::type construct( const coordinate_type& r, const coordinate_type& t ) { return point_type( r*math_functions< coordinate_type >::cos(t), r*math_functions< coordinate_type >::sin(t) ); }                                                                                        \
    template <typename T>                                                             \
    static inline typename boost::disable_if< boost::is_same< typename point_traits<T>::dimension_type, dimension_traits<2> >, point_type >::type construct( const coordinate_type& r, const coordinate_type& t, const coordinate_type& phi ) { return point_type( r*math_functions< coordinate_type >::cos(t)*math_functions< coordinate_type >::sin(phi), r*math_functions< coordinate_type >::sin(t)*math_functions< coordinate_type >::sin(phi), r*math_functions< coordinate_type >::cos(phi) ); } \
};

BOOST_DEFINE_POINT_TRAITS( point_float_2d );
BOOST_DEFINE_POINT_TRAITS( point_float_3d );
BOOST_DEFINE_POINT_TRAITS( point_double_2d );
BOOST_DEFINE_POINT_TRAITS( point_double_3d );

BOOST_DEFINE_CARTESIAN_ACCESS_TRAITS( point_float_2d );
BOOST_DEFINE_CARTESIAN_ACCESS_TRAITS( point_float_3d );
BOOST_DEFINE_CARTESIAN_ACCESS_TRAITS( point_double_2d );
BOOST_DEFINE_CARTESIAN_ACCESS_TRAITS( point_double_3d );

BOOST_DEFINE_POLAR_ACCESS_TRAITS( point_float_2d );
BOOST_DEFINE_POLAR_ACCESS_TRAITS( point_float_3d );
BOOST_DEFINE_POLAR_ACCESS_TRAITS( point_double_2d );
BOOST_DEFINE_POLAR_ACCESS_TRAITS( point_double_3d );

BOOST_DEFINE_POINT_TRAITS( point_int_2d );
BOOST_DEFINE_POINT_TRAITS( point_int_3d );
BOOST_DEFINE_POINT_TRAITS( point_int64_2d );
BOOST_DEFINE_POINT_TRAITS( point_int64_3d );

BOOST_DEFINE_CARTESIAN_ACCESS_TRAITS( point_int_2d );
BOOST_DEFINE_CARTESIAN_ACCESS_TRAITS( point_int_3d );
BOOST_DEFINE_CARTESIAN_ACCESS_TRAITS( point_int64_2d );
BOOST_DEFINE_CARTESIAN_ACCESS_TRAITS( point_int64_3d );

BOOST_DEFINE_POLAR_ACCESS_TRAITS( point_int_2d );
BOOST_DEFINE_POLAR_ACCESS_TRAITS( point_int_3d );
BOOST_DEFINE_POLAR_ACCESS_TRAITS( point_int64_2d );
BOOST_DEFINE_POLAR_ACCESS_TRAITS( point_int64_3d );

}}}//namespace boost::numeric::geometry;

#endif //_BOOST_GEOMETRY_POINT_HPP
