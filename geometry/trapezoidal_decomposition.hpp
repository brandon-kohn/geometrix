//
//! Copyright © 2008
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_GEOMETRY_TRAPEZOIDAL_DECOMPOSITION_HPP
#define _BOOST_GEOMETRY_TRAPEZOIDAL_DECOMPOSITION_HPP
#pragma once

#include "segment_traits.hpp"
#include "point_sequence_utilities.hpp"
#include "utilities.hpp"
#include "random_generator.hpp"

#include <boost/foreach.hpp>
#include <set>
#include <limits>

namespace boost
{
namespace numeric
{
namespace geometry
{
    namespace detail
    {
        template <typename Polygon, typename Trapezoid, typename NumberComparisonPolicy>
        struct trapezoid_predicate
        {
            typedef typename Trapezoid::coordinate_type                   coordinate_type;
            typedef Trapezoid                                             trapezoid_type;
            typedef boost::shared_ptr< trapezoid_type >                   trapezoid_ptr;
            typedef typename point_sequence_traits< Polygon >::point_type point_type;
            
            trapezoid_predicate( const Polygon& polygon, const NumberComparisonPolicy& compare )
                : m_compare( compare )
                , m_polygon( polygon )
            {}

            //! workaround for having two operators with same name (even though sigs are different.. won't compile on vs 2008)
            template <typename T1, typename T2> struct comparer
            {
                BOOST_MPL_ASSERT_MSG( ( false ), TRAPEZOID_PREDICATE_MISMATCH_BETWEEN_COMPARE_ARG_TYPES, (T1,T2) );
            };

            template <>
            struct comparer< point_type, trapezoid_ptr >
            {
                template <typename Polygon, typename FloatingPointComparison>
                static inline bool compare( const point_type& lhs, const trapezoid_ptr& rhs, const Polygon& polygon, const FloatingPointComparison& compare )
                {
                    typedef cartesian_access_traits< point_type >                 point_access;
                    int rIndex = rhs->get_right();
                    if( rIndex < 0)
                    {
                        return true;
                    }

                    const point_type& prStart = polygon[ rIndex ];
                    const point_type& prEnd = polygon[ (rIndex +1) % polygon.size() ];
                    
                    orientation_type oType = get_orientation( prStart, prEnd, lhs, compare );
                    assert( oType != oriented_collinear );
                    if( oType == oriented_collinear )
                    {
                        return false;
                    }
                    else if( compare.less_than( point_access::get<1>( prStart ), point_access::get<1>( prEnd ) ) )
                    {
                        return ( oType == oriented_left );
                    }
                    else
                    {
                        return ( oType == oriented_right );
                    }
                }
            };
            template <>
            struct comparer< trapezoid_ptr, point_type >
            {
                template <typename Polygon, typename FloatingPointComparison>
                static inline bool compare( const trapezoid_ptr& lhs, const point_type& rhs, const Polygon& polygon, const FloatingPointComparison& compare )
                {
                    typedef cartesian_access_traits< point_type >                 point_access;
                    int rIndex = lhs->get_right();
                    if( rIndex < 0)
                    {
                        return false;
                    }

                    const point_type& prStart = polygon[ rIndex ];
                    const point_type& prEnd = polygon[ (rIndex + 1) % polygon.size() ];
                    
                    orientation_type oType = get_orientation( prStart, prEnd, rhs, compare );
                    if( oType == oriented_collinear )
                    {
                        return false;
                    }
                    else if( compare.less_than( point_access::get<1>( prStart ), point_access::get<1>( prEnd ) ) )
                    {
                        return ( oType == oriented_right );
                    }
                    else
                    {
                        return ( oType == oriented_left );
                    }        
                }
            };
            template <>
            struct comparer< trapezoid_ptr, trapezoid_ptr >
            {
                template <typename Polygon, typename FloatingPointComparison>
                static inline bool compare( const trapezoid_ptr& lhs, const trapezoid_ptr& rhs, const Polygon& polygon, const FloatingPointComparison& compare )
                {
                    //This is here only for the compiler (VS2008) which seems to have a debug on predicates.
                    //This is not called in 'release' compiled code.
                    return false;
                }
            };

            template <typename T1, typename T2>
            inline bool operator()( const T1& lhs, const T2& rhs ) const
            {                
                return comparer<T1,T2>::compare<Polygon, NumberComparisonPolicy>( lhs, rhs, m_polygon, m_compare );                              
            }

            const Polygon&                m_polygon;
            NumberComparisonPolicy m_compare;
        };

        //! Helper struct for comparing items in collections.
        template <typename CoordinateType, typename Strip, typename NumberComparisonPolicy>
        struct strip_interval_predicate
        {
            typedef CoordinateType coordinate_type;
            typedef Strip          strip_type;

            strip_interval_predicate()                
            {}

            strip_interval_predicate(const NumberComparisonPolicy& compare )
                : m_compare( compare )
            {}

            //! workaround for having two operators with same name (even though sigs are different.. won't compile on vs 2008)
            template <typename T1, typename T2> struct comparer {};
            template <>
            struct comparer< coordinate_type, strip_type >
            {
                template <typename FloatingPointComparison>
                static inline bool compare( const coordinate_type& lhs, const strip_type& rhs, const FloatingPointComparison& compare )
                {
                    coordinate_type rhs_min = rhs.get_min();                
                    return ( compare.less_than( lhs, rhs_min ) );
                }
            };
            template <>
            struct comparer< strip_type, coordinate_type >
            {
                template <typename FloatingPointComparison>
                static inline bool compare( const strip_type& lhs, const coordinate_type& rhs, const FloatingPointComparison& compare )
                {
                     coordinate_type lhs_max = lhs.get_max();
                     return ( compare.less_than_or_equal( lhs_max, rhs ) ); 
                }
            };
            template <>
            struct comparer< strip_type, strip_type >
            {
                template <typename FloatingPointComparison>
                static inline bool compare( const strip_type& lhs, const strip_type& rhs, const FloatingPointComparison& compare )
                {
                     coordinate_type lhs_max = lhs.get_max();
                     coordinate_type rhs_min = rhs.get_min();
                     return ( compare.less_than_or_equal( lhs_max, rhs_min ) ); 
                }
            };

            template <typename T1, typename T2>
            inline bool operator()( const T1& lhs, const T2& rhs ) const
            {                
                return comparer<T1,T2>::compare<NumberComparisonPolicy>( lhs, rhs, m_compare );                              
            }
            
            NumberComparisonPolicy m_compare;

        };

        //! Positive if the interior of the polygon at the max edge is in the +x direction.
        //! Negative if the interior of the polygon at the min edge is in the -x direction.
        enum classification
        {
            e_negative = -1,
            e_positive = 1
        };
        template <typename Point, typename Iterator>
        class trapezoid
        {
        public:

            typedef Point                                                point_type;
            typedef typename point_traits< point_type >::coordinate_type coordinate_type;
            typedef cartesian_access_traits< point_type >                point_access;
            
            trapezoid( const coordinate_type& ymin, const coordinate_type& ymax, Iterator left, Iterator right, classification classify )
                : m_left( left )
                , m_right( right )                
                , m_classify( classify )
                , m_ymin( ymin )
                , m_ymax( ymax )
            {}

            inline Iterator               get_left() const { return m_left; }
            inline Iterator               get_right() const { return m_right; }
            inline Iterator               get_up() const { return m_up; }
            inline Iterator               get_down() const { return m_down; }
            inline void                   set_left( Iterator v ) { m_left = v; }
            inline void                   set_right( Iterator v ) { m_right = v; }
            inline void                   set_up( Iterator v ) { m_up = v; }
            inline void                   set_down( Iterator v ) { m_down = v; }
            inline classification         get_classification() const { return m_classify; }
            inline void                   set_classification( classification c ) { m_classify = c; }
            
            //! Access to explicit bounds
            inline const coordinate_type& get_ymin() const { return m_ymin; }
            inline const coordinate_type& get_ymax() const { return m_ymax; }
            inline void                   set_ymin( const coordinate_type& v ) { m_ymin = v; }
            inline void                   set_ymax( const coordinate_type& v ) { m_ymax = v; }

        private:
            
            Iterator        m_left;
            Iterator        m_right;
            Iterator        m_up;
            Iterator        m_down;
            coordinate_type m_ymin;
            coordinate_type m_ymax;
            classification  m_classify;

        };

        template <typename CoordinateType, typename Trapezoid>
        class trapezoid_strip
        {
        public:

            typedef CoordinateType                        coordinate_type;
            typedef Trapezoid                             trapezoid_type;
            typedef boost::shared_ptr<trapezoid_type>     trapezoid_ptr;
            typedef std::list< trapezoid_ptr >            trapezoid_list;
            typedef std::set< trapezoid_ptr >             trapezoid_store;
            typedef boost::shared_ptr< trapezoid_store >  trapezoid_store_ptr;

            trapezoid_strip( trapezoid_store_ptr& pTrapezoidStore, const coordinate_type& min_, const coordinate_type& max_ )
                : m_max( max_ )
                , m_min( min_ )
                , m_pTrapezoidStore( pTrapezoidStore )
            {}

            trapezoid_strip( trapezoid_store_ptr& pTrapezoidStore, const coordinate_type& min_, const coordinate_type& max_, trapezoid_type& trap )
                : m_max( max_ )
                , m_min( min_ )
                , m_pTrapezoidStore( pTrapezoidStore )
            {
                trapezoid_ptr pNewTrap( new trapezoid_type( trap ) );
                pNewTrap->set_ymax( max_ );
                pNewTrap->set_ymin( min_ );
                m_trapezoidSet.push_back( pNewTrap );
                pTrapezoidStore->insert( pNewTrap );
            }

            trapezoid_strip( trapezoid_store_ptr& pTrapezoidStore, const coordinate_type& min_, const coordinate_type& max_, const trapezoid_list& traps )
                : m_max( max_ )
                , m_min( min_ )
                , m_trapezoidSet( traps )
                , m_pTrapezoidStore( pTrapezoidStore )
            {}

            template <typename Polygon, typename CoordinateType, typename NumberComparisonPolicy>
            std::pair< typename trapezoid_list::iterator , typename trapezoid_list::iterator > insert( const Polygon& polygon, CoordinateType mid, int i, typename classification classify, const NumberComparisonPolicy& compare ) 
            {
                typedef typename point_sequence_traits< Polygon >::point_type point_type;
                typedef typename point_traits< point_type >::coordinate_type  coordinate_type;
                typedef cartesian_access_traits< point_type >                 point_access;
                
                size_t pSize = point_sequence_traits< Polygon >::size( polygon );
                const point_type& pStart = point_sequence_traits< Polygon >::get_point( polygon, i );
                const point_type& pEnd = point_sequence_traits< Polygon >::get_point( polygon, ( i + 1 ) % pSize );

                coordinate_type x;
                if( is_vertical( pStart, pEnd, compare ) )
                {
                    x = point_access::get<0>( pStart );
                }
                else if( is_horizontal( pStart, pEnd, compare ) )
                {
                    assert( false );
                    x = ( point_access::get<0>( pStart ) + point_access::get<0>( pEnd ) ) / coordinate_type( 2 );
                }
                else
                {
                    coordinate_type slope;//don't need it.
                    x = x_of_y( pStart, pEnd, mid, slope, compare );
                }

                point_type midPoint = construction_traits< point_type >::construct( x, mid );

                trapezoid_list::iterator iter = std::lower_bound( m_trapezoidSet.begin(), m_trapezoidSet.end(), midPoint, trapezoid_predicate<Polygon, trapezoid_type, NumberComparisonPolicy>( polygon, compare ) );
                if( iter != m_trapezoidSet.end() )
                {
                    trapezoid_ptr pNewTrap( new trapezoid_type( m_min, m_max, (*iter)->get_left(), i, classify ) );                    
                    
                    trapezoid_ptr pOldTrap( new trapezoid_type( **iter ) );
                    pOldTrap->set_left( i );
                    pOldTrap->set_classification( (classify == detail::e_negative) ? e_positive : e_negative );
                    pOldTrap->set_ymax( m_max );
                    pOldTrap->set_ymin( m_min );

                    ///Remove it from the store if there are only the two. (this one and the one in store)
                    trapezoid_store_ptr::element_type::iterator pIter = m_pTrapezoidStore->find( *iter );
                    if( pIter != m_pTrapezoidStore->end() )
                    {
                        m_pTrapezoidStore->erase( pIter );
                    }

                    *iter = pOldTrap;
                    m_trapezoidSet.insert( iter, pNewTrap );
                    
                    m_pTrapezoidStore->insert( pNewTrap );
                    m_pTrapezoidStore->insert( pOldTrap );

                    trapezoid_list::iterator right = iter;
                    trapezoid_list::iterator left = --iter;
                    return std::make_pair( left, right );
                }
                else
                {
                    assert( false );
                }

                return std::make_pair( iter, iter );

            }

            inline coordinate_type                       get_min() const { return m_min; }
            inline coordinate_type                       get_max() const { return m_max; }
            inline void                                  set_min( coordinate_type v ) { m_min = v; }
            inline void                                  set_max( coordinate_type v ) { m_max = v; }
            inline trapezoid_list&                       get_trapezoids() { return m_trapezoidSet; }

        private:

            coordinate_type     m_min;
            coordinate_type     m_max;
            trapezoid_list      m_trapezoidSet;          
            trapezoid_store_ptr m_pTrapezoidStore;

        };

        template <typename Polygon, typename Bounds, typename CoordinateType, typename TrapezoidStripSet, typename TrapezoidStore, typename NumberComparisonPolicy>
        void strip_insert( const Polygon& polygon, const Bounds& bounds, TrapezoidStripSet& stripSet, TrapezoidStore& pTrapStore, const CoordinateType& y0, const CoordinateType& y1, int i, classification classify, const NumberComparisonPolicy& compare )
        {
            typedef CoordinateType                                        coordinate_type;
            typedef typename TrapezoidStripSet::value_type                trapezoid_strip_type;
            typedef typename trapezoid_strip_type::trapezoid_type         trapezoid_type;            
            typedef typename trapezoid_strip_type::trapezoid_list         trapezoid_list;
            typedef typename point_sequence_traits< Polygon >::point_type point_type;
            typedef cartesian_access_traits< point_type >                 point_access;

            typename TrapezoidStripSet::iterator s0 = std::lower_bound( stripSet.begin(), stripSet.end(), y0, strip_interval_predicate<coordinate_type, trapezoid_strip_type, NumberComparisonPolicy>( compare ) );
            if( s0 != stripSet.end() )                
            {
                if( compare.greater_than( y0, s0->get_min() ) && compare.less_than( y0, s0->get_max() ) )
                {
                    trapezoid_strip< coordinate_type, trapezoid_type > newStrip( pTrapStore, s0->get_min(), y0, s0->get_trapezoids() );
                    s0->set_min( y0 );
                    stripSet.insert( s0, newStrip );                
                }    
            }
            else
            {
                assert( false );
            }

            typename TrapezoidStripSet::iterator s1 = std::lower_bound( stripSet.begin(), stripSet.end(), y1, strip_interval_predicate<coordinate_type, trapezoid_strip_type, NumberComparisonPolicy>( compare ) );
            if( s1 != stripSet.end() )                
            {
                if( compare.greater_than( y1, s1->get_min() ) && compare.less_than( y1, s1->get_max() ) )
                {
                    trapezoid_strip< coordinate_type, trapezoid_type > newStrip( pTrapStore, y1, s1->get_max(), s1->get_trapezoids() );
                    s1->set_max( y1 );
                    TrapezoidStripSet::iterator s2( s1 );
                    ++s2;
                    stripSet.insert( s2, newStrip );
                }
            }
            else
            {
                assert( false );
            }

            coordinate_type ymax = (std::max)( y0, y1 );
            coordinate_type ymin = (std::min)( y0, y1 );

            std::vector< std::pair< trapezoid_list::iterator, trapezoid_list::iterator > > trapIters;
            typedef trapezoid_strip< coordinate_type, trapezoid_type > TrapezoidStrip;
            TrapezoidStripSet::iterator L = s0;
            while( L != stripSet.end() )
            {
                coordinate_type s_ymin = L->get_min();
                coordinate_type s_ymax = L->get_max();

                bool edgeSpansStrip = ( compare.greater_than( ymax, s_ymin ) && compare.less_than( ymin, s_ymax ) );                
                if( edgeSpansStrip )
                {
                    coordinate_type mid = ( s_ymin + s_ymax ) / coordinate_type( 2 );
                    if( compare.greater_than( mid, ymax ) )
                    {
                        mid = ymax;
                    }
                    else if( compare.less_than( mid, ymin ) )
                    {
                        mid = ymin;
                    }

                    trapIters.push_back( L->insert( polygon, mid, i, classify, compare ) );                    
                }

                if( L == s1 )
                {
                    break;
                }
                ++L;
            }

            size_t pSize = point_sequence_traits< Polygon >::size( polygon );

            //! merge the traps
            if( !trapIters.empty() )
            {
                for( size_t i=0;i < trapIters.size() - 1; ++i )
                {
                    size_t n = i+1;

                    //! first are the traps left of the segment.
                    trapezoid_list::iterator current = trapIters[i].first;
                    trapezoid_list::iterator next = trapIters[n].first;

                    if( (*current)->get_left() != -1 && (*current)->get_right() != -1 && (*next)->get_left() != -1 && (*next)->get_right() != -1 )
                    {                   
                        if( (*current)->get_left() == (*next)->get_left() && (*current)->get_right() == (*next)->get_right() )
                        {
                            //merge 
                            coordinate_type nymax = (*next)->get_ymax();
                            coordinate_type nymin = (*next)->get_ymin();
                            coordinate_type cymin = (*current)->get_ymin();
                            coordinate_type cymax = (*current)->get_ymax();

                            coordinate_type ymax = (std::max)( nymax, cymax );
                            coordinate_type ymin = (std::min)( nymin, cymin );

                            coordinate_type mid = ( ymax + ymin ) * coordinate_type( 2 );

                            const point_type& plStart = point_sequence_traits< Polygon >::get_point( polygon, (*current)->get_left() );
                            const point_type& plEnd = point_sequence_traits< Polygon >::get_point( polygon, ( (*current)->get_left() + 1 ) % pSize );

                            coordinate_type x;
                            if( is_vertical( plStart, plEnd, compare ) )
                            {
                                x = point_access::get<0>( plStart );
                            }
                            else if( is_horizontal( plStart, plEnd, compare ) )
                            {
                                assert( false );
                                x = ( point_access::get<0>( plStart ) + point_access::get<0>( plEnd ) ) / coordinate_type( 2 );
                            }
                            else
                            {
                                x = x_of_y( plStart, plEnd, mid, compare );
                            }
//                            (*current)->set_xmin( x );

                            const point_type& prStart = point_sequence_traits< Polygon >::get_point( polygon, (*current)->get_right() );
                            const point_type& prEnd = point_sequence_traits< Polygon >::get_point( polygon, ( (*current)->get_right() + 1 ) % pSize );
                            
                            if( is_vertical( prStart, prEnd, compare ) )
                            {
                                x = point_access::get<0>( prStart );
                            }
                            else if( is_horizontal( prStart, prEnd, compare ) )
                            {
                                assert( false );
                                x = ( point_access::get<0>( prStart ) + point_access::get<0>( prEnd ) ) / coordinate_type( 2 );
                            }
                            else
                            {
                                x = x_of_y( prStart, prEnd, mid, compare );
                            }

                            (*current)->set_ymax( ymax );
                            (*current)->set_ymin( ymin );
                            pTrapStore->erase( *next );
                            *next = *current;            
                        }
                    }
                    
                    //! second are traps on the right of the segment.
                    current = trapIters[i].second;
                    next = trapIters[n].second;

                    if( (*current)->get_left() != -1 && (*current)->get_right() != -1 && (*next)->get_left() != -1 && (*next)->get_right() != -1 )
                    {
                        if( (*current)->get_left() == (*next)->get_left() && (*current)->get_right() == (*next)->get_right() )
                        {
                            //merge 
                            coordinate_type nymax = (*next)->get_ymax();
                            coordinate_type nymin = (*next)->get_ymin();
                            coordinate_type cymin = (*current)->get_ymin();
                            coordinate_type cymax = (*current)->get_ymax();

                            coordinate_type ymax = (std::max)( nymax, cymax );
                            coordinate_type ymin = (std::min)( nymin, cymin );

                            coordinate_type mid = ( ymax + ymin ) * coordinate_type( 2 );

                            const point_type& plStart = point_sequence_traits< Polygon >::get_point( polygon, (*current)->get_left() );
                            const point_type& plEnd = point_sequence_traits< Polygon >::get_point( polygon, ( (*current)->get_left() + 1 ) % pSize );

                            coordinate_type x;
                            if( is_vertical( plStart, plEnd, compare ) )
                            {
                                x = point_access::get<0>( plStart );
                            }
                            else if( is_horizontal( plStart, plEnd, compare ) )
                            {
                                assert( false );
                                x = ( point_access::get<0>( plStart ) + point_access::get<0>( plEnd ) ) / coordinate_type( 2 );
                            }
                            else
                            {
                                x = x_of_y( plStart, plEnd, mid, compare );
                            }

                            const point_type& prStart = point_sequence_traits< Polygon >::get_point( polygon, (*current)->get_right() );
                            const point_type& prEnd = point_sequence_traits< Polygon >::get_point( polygon, ( (*current)->get_right() + 1 ) % pSize );

                            if( is_vertical( prStart, prEnd, compare ) )
                            {
                                x = point_access::get<0>( prStart );
                            }
                            else if( is_horizontal( prStart, prEnd, compare ) )
                            {
                                assert( false );
                                x = ( point_access::get<0>( prStart ) + point_access::get<0>( prEnd ) ) / coordinate_type( 2 );
                            }
                            else
                            {
                                x = x_of_y( prStart, prEnd, mid, compare );
                            }
                            
                            (*current)->set_ymax( ymax );
                            (*current)->set_ymin( ymin );
                            pTrapStore->erase( *next );
                            *next = *current;                        
                        }
                    }
                }
            }

        }

    }//detail

    //! Function to decompose a polygon into trapezoids.
    template <typename Polygon, typename NumberComparisonPolicy>
    boost::shared_ptr< std::vector< std::vector< typename point_sequence_traits< Polygon >::point_type > > >
    trapezoidal_decomposition_polygon( const Polygon& polygon, const NumberComparisonPolicy& compare )
    {
        using namespace boost::numeric::geometry::detail;

        typedef typename point_sequence_traits< Polygon >::point_type point_type;
        typedef typename point_traits< point_type >::coordinate_type  coordinate_type;
        typedef cartesian_access_traits< point_type >                 point_access;
        typedef trapezoid< point_type, int >                          trapezoid_type;
        typedef boost::shared_ptr< trapezoid_type >                   trapezoid_ptr;
        typedef trapezoid_strip< coordinate_type, trapezoid_type >    trapezoid_strip_type;        
        typedef std::set< trapezoid_ptr >                             trapezoid_store;
        typedef boost::shared_ptr< trapezoid_store >                  trapezoid_store_ptr;

        trapezoid_store_ptr pTrapStore( new trapezoid_store() );

        ///!randomly shuffle the polygon edges
        size_t N = point_sequence_traits< Polygon >::size( polygon );
        std::vector< size_t > indices( N );
        std::generate( indices.begin(), indices.end(), sequence_generator< size_t >( size_t( 0 ) ) );
        std::random_shuffle( indices.begin(), indices.end(), random_integer_generator< boost::mt19937 >() );

        //Get the bound extrema of the polygon vertices.
        typename bounds_tuple< Polygon >::type bounds = get_bounds( polygon, compare );

        //Add infinity buffer.
        bounds.get<e_xmin>() -= coordinate_type( 1 );
        bounds.get<e_ymin>() -= coordinate_type( 1 );
        bounds.get<e_xmax>() += coordinate_type( 1 );
        bounds.get<e_ymax>() += coordinate_type( 1 );

        //We now have 'inifity' defined in terms of max bounds for the polygon. (bounds +/- 1).

        typedef std::list< trapezoid_strip_type > Strips;
        Strips pStrip;

        trapezoid_type trap( bounds.get<e_ymin>(), bounds.get<e_ymax>(),-1, -1, detail::e_negative );
        
        trapezoid_strip< coordinate_type, trapezoid_type > newStrip( pTrapStore, bounds.get<e_ymin>(), bounds.get<e_ymax>(), trap );
        pStrip.push_back( newStrip ); 

        for( size_t index = 0; index < N; ++index )
        {
            size_t i0 = indices[index];
            size_t i1 = (i0 + 1) % N;
            const point_type& pointI0 = point_sequence_traits< Polygon >::get_point( polygon, i0 );
            const point_type& pointI1 = point_sequence_traits< Polygon >::get_point( polygon, i1 );
            if( compare.less_than( point_access::get<1>( pointI0 ), point_access::get<1>( pointI1 ) ) )
            {
                strip_insert( polygon, bounds, pStrip, pTrapStore, point_access::get<1>( pointI0 ), point_access::get<1>( pointI1 ), i0, detail::e_negative, compare );
            }
            else if( compare.less_than( point_access::get<1>( pointI1 ), point_access::get<1>( pointI0 ) ) )
            {
                strip_insert( polygon, bounds, pStrip, pTrapStore, point_access::get<1>( pointI1 ), point_access::get<1>( pointI0 ), i0, detail::e_positive, compare );
            }
            //else ignore horizontals
        }

        boost::shared_ptr< std::vector< std::vector< point_type > > > pTtrapezoids( new std::vector< std::vector< point_type > >() );

        BOOST_FOREACH( trapezoid_ptr& pTrap, *pTrapStore )
        {                        
            int left = pTrap->get_left();
            int right = pTrap->get_right();

            if( left >= 0 && right >= 0 )
            {
                point_type lStart = point_sequence_traits< Polygon >::get_point( polygon, left );
                point_type lEnd   = point_sequence_traits< Polygon >::get_point( polygon, (left+1) % N );

                point_type rStart = point_sequence_traits< Polygon >::get_point( polygon, right );
                point_type rEnd   = point_sequence_traits< Polygon >::get_point( polygon, (right+1) % N );

                coordinate_type ymin = pTrap->get_ymin();
                coordinate_type ymax = pTrap->get_ymax();

                coordinate_type lsy = point_access::get<1>( lStart );
                coordinate_type ley = point_access::get<1>( lEnd );
                coordinate_type lymin = (std::min)( lsy, ley );
                coordinate_type lymax = (std::max)( lsy, ley );

                coordinate_type ll_x;
                coordinate_type ul_x;
                if( is_vertical( lStart, lEnd, compare ) )
                {
                    ll_x = point_access::get<0>( lStart );
                    ul_x = ll_x;
                }
                else
                {
                    ll_x = x_of_y( lStart, lEnd, (std::max)(ymin, lymin), compare );
                    ul_x = x_of_y( lStart, lEnd, (std::min)(ymax, lymax), compare );
                }

                coordinate_type rsy = point_access::get<1>( rStart );
                coordinate_type rey = point_access::get<1>( rEnd );
                coordinate_type rymin = (std::min)( rsy, rey );
                coordinate_type rymax = (std::max)( rsy, rey );

                coordinate_type lr_x;
                coordinate_type ur_x;
                if( is_vertical( rStart, rEnd, compare ) )
                {
                    lr_x = point_access::get<0>( rStart );
                    ur_x = lr_x;
                }
                else
                {
                    lr_x = x_of_y( rStart, rEnd, (std::max)(ymin,rymin), compare );
                    ur_x = x_of_y( rStart, rEnd, (std::min)(ymax,rymax), compare );
                }

                point_type ll_point = construction_traits< point_type >::construct( ll_x, (std::max)(ymin,lymin) );
                point_type ul_point = construction_traits< point_type >::construct( ul_x, (std::min)(ymax,lymax) );
                point_type lr_point = construction_traits< point_type >::construct( lr_x, (std::max)(ymin,rymin) );
                point_type ur_point = construction_traits< point_type >::construct( ur_x, (std::min)(ymax,rymax) );

                std::vector< point_type > trapezoid_;
                trapezoid_.push_back( ll_point );
                trapezoid_.push_back( lr_point );
                trapezoid_.push_back( ur_point );
                trapezoid_.push_back( ul_point );
                trapezoid_.push_back( ll_point );

                point_type centroid = get_centroid( trapezoid_, compare );
                if( get_orientation( lStart, lEnd, centroid, compare ) == oriented_left && get_orientation( rStart, rEnd, centroid, compare ) == oriented_left )
                    pTtrapezoids->push_back( trapezoid_ );
            }

        }

        return pTtrapezoids;
    }

    //! Function to decompose a polygon into trapezoids.
    template <typename Polygon, typename NumberComparisonPolicy>
    boost::shared_ptr< std::vector< std::vector< typename point_sequence_traits< Polygon >::point_type > > >
    trapezoidal_decomposition_polygon_with_holes( const std::vector<Polygon>& polygons, const NumberComparisonPolicy& compare )
    {
        using namespace boost::numeric::geometry::detail;

        typedef typename point_sequence_traits< Polygon >::point_type point_type;
        typedef typename point_traits< point_type >::coordinate_type  coordinate_type;
        typedef cartesian_access_traits< point_type >                 point_access;
        typedef trapezoid< point_type, int >                          trapezoid_type;
        typedef boost::shared_ptr< trapezoid_type >                   trapezoid_ptr;
        typedef trapezoid_strip< coordinate_type, trapezoid_type >    trapezoid_strip_type;
        typedef std::set< trapezoid_ptr >                             trapezoid_store;
        typedef boost::shared_ptr< trapezoid_store >                  trapezoid_store_ptr;

        trapezoid_store_ptr pTrapStore( new trapezoid_store() );

        //Get the bound extrema of the polygon vertices.
        typename bounds_tuple< Polygon >::type bounds = get_bounds( polygons[0], compare );

        //Add infinity buffer.
        bounds.get<e_xmin>() -= coordinate_type( 1 );
        bounds.get<e_ymin>() -= coordinate_type( 1 );
        bounds.get<e_xmax>() += coordinate_type( 1 );
        bounds.get<e_ymax>() += coordinate_type( 1 );

        std::vector< size_t > indices;
        std::vector< point_type > points;        
        BOOST_FOREACH( const Polygon& poly, polygons )
        {
            size_t polySize = point_sequence_traits< Polygon >::size( poly );
            for( size_t i=0; i < polySize; ++i )
            {
                if( i + 1 < polySize )//Don't add the index for the last so as not to connect holes together.
                    indices.push_back( points.size() );
                points.push_back( point_sequence_traits< Polygon >::get_point( poly, i ) );                
            }

            //! add the front to close
            if( !equals( points.back(), point_sequence_traits< Polygon >::get_point( poly, 0 ), compare ) )
            {
                //! unless the polygon wasn't properly closed.
                indices.push_back( points.size()-1 );
                points.push_back( point_sequence_traits< Polygon >::get_point( poly, 0 ) );                
            }            
        }

        ///!randomly shuffle the polygon edges        
        std::random_shuffle( indices.begin(), indices.end(), random_integer_generator< boost::mt19937 >() );

        //We now have 'inifity' defined in terms of max bounds for the polygon. (bounds +/- 1).

        typedef std::list< trapezoid_strip_type > Strips;
        Strips pStrip;

        trapezoid_type trap( bounds.get<e_ymin>(), bounds.get<e_ymax>(), -1, -1, e_negative );
        
        trapezoid_strip< coordinate_type, trapezoid_type > newStrip( pTrapStore, bounds.get<e_ymin>(), bounds.get<e_ymax>(), trap );
        pStrip.push_back( newStrip ); 

        size_t N = indices.size();
        for( size_t index = 0; index < N; ++index )
        {
            size_t i0 = indices[index];
            size_t i1 = i0 + 1;
            if( compare.less_than( point_access::get<1>( points[ i0 ] ), point_access::get<1>( points[ i1 ] ) ) )
            {
                strip_insert( points, bounds, pStrip, pTrapStore, point_access::get<1>( points[ i0 ] ), point_access::get<1>( points[ i1 ] ), i0, detail::e_negative, compare );
            }
            else if( compare.less_than( point_access::get<1>( points[ i1 ] ), point_access::get<1>( points[ i0 ] ) ) )
            {
                strip_insert( points, bounds, pStrip, pTrapStore, point_access::get<1>( points[ i1 ] ), point_access::get<1>( points[ i0 ] ), i0, detail::e_positive, compare );
            }
            //else ignore horizontals
        }

        boost::shared_ptr< std::vector< std::vector< point_type > > > pTtrapezoids( new std::vector< std::vector< point_type > >() );

        ///First and last traps are always out of boundary of polygon. (top and bottom too)
        BOOST_FOREACH( trapezoid_ptr& pTrap, *pTrapStore )
        {                        
            int left = pTrap->get_left();
            int right = pTrap->get_right();

            if( left >= 0 && right >= 0 )
            {
                point_type lStart = points[ left ];
                point_type lEnd   = points[ (left+1) ];

                point_type rStart = points[ right ];
                point_type rEnd   = points[ (right+1) ];

                coordinate_type ymin = pTrap->get_ymin();
                coordinate_type ymax = pTrap->get_ymax();

                coordinate_type lsy = point_access::get<1>( lStart );
                coordinate_type ley = point_access::get<1>( lEnd );
                coordinate_type lymin = (std::min)( lsy, ley );
                coordinate_type lymax = (std::max)( lsy, ley );

                coordinate_type ll_x;
                coordinate_type ul_x;
                if( is_vertical( lStart, lEnd, compare ) )
                {
                    ll_x = point_access::get<0>( lStart );
                    ul_x = ll_x;
                }
                else
                {
                    ll_x = x_of_y( lStart, lEnd, (std::max)(ymin, lymin), compare );
                    ul_x = x_of_y( lStart, lEnd, (std::min)(ymax, lymax), compare );
                }

                coordinate_type rsy = point_access::get<1>( rStart );
                coordinate_type rey = point_access::get<1>( rEnd );
                coordinate_type rymin = (std::min)( rsy, rey );
                coordinate_type rymax = (std::max)( rsy, rey );

                coordinate_type lr_x;
                coordinate_type ur_x;
                if( is_vertical( rStart, rEnd, compare ) )
                {
                    lr_x = point_access::get<0>( rStart );
                    ur_x = lr_x;
                }
                else
                {
                    lr_x = x_of_y( rStart, rEnd, (std::max)(ymin,rymin), compare );
                    ur_x = x_of_y( rStart, rEnd, (std::min)(ymax,rymax), compare );
                }

                point_type ll_point = point_access::construct< point_type >( ll_x, (std::max)(ymin,lymin) );
                point_type ul_point = point_access::construct< point_type >( ul_x, (std::min)(ymax,lymax) );
                point_type lr_point = point_access::construct< point_type >( lr_x, (std::max)(ymin,rymin) );
                point_type ur_point = point_access::construct< point_type >( ur_x, (std::min)(ymax,rymax) );

                std::vector< point_type > trapezoid_;
                trapezoid_.push_back( ll_point );
                trapezoid_.push_back( lr_point );
                trapezoid_.push_back( ur_point );
                trapezoid_.push_back( ul_point );
                trapezoid_.push_back( ll_point );

                point_type centroid = get_centroid( trapezoid_, compare );
                if( get_orientation( lStart, lEnd, centroid, compare ) == oriented_left && get_orientation( rStart, rEnd, centroid, compare ) == oriented_left )
                    pTtrapezoids->push_back( trapezoid_ );
            }

        }

        return pTtrapezoids;
    }

}}}//namespace boost::numeric::geometry;

#endif //_BOOST_GEOMETRY_TRAPEZOIDAL_DECOMPOSITION_HPP
