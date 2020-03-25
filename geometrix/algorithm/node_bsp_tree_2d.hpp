//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_BSPTREE2D_HPP
#define GEOMETRIX_BSPTREE2D_HPP
#pragma once

#include <geometrix/numeric/constants.hpp>
#include <geometrix/utility/utilities.hpp>
#include <geometrix/algorithm/line_intersection.hpp>
#include <geometrix/primitive/line.hpp>
#include <geometrix/utility/random_generator.hpp>
#include <geometrix/primitive/array_point_sequence.hpp>
#include <geometrix/primitive/axis_aligned_bounding_box.hpp>
#include <geometrix/algorithm/intersection/ray_aabb_intersection.hpp>
#include <geometrix/algorithm/point_location_classification.hpp>

#include <boost/range.hpp>
#include <boost/noncopyable.hpp>

#include <boost/smart_ptr/scoped_ptr.hpp>
#include <boost/tuple/tuple.hpp>

#include <algorithm>
#include <vector>
#include <stack>
#include <set>

namespace geometrix {

    namespace partition_policies
    {
        //! A concept to define the interface for a bsp partition strategy.
        template <typename PartitionSelector>
        struct PartitionGeometrySelectorConcept
        {
            void constraints()
            {
                PartitionSelector* pSelector = 0;

                //! The partition selector must return a geometry that defines a n-dimension partitioning geometry (line, plane etc).
                typedef typename PartitionSelector::output_geometry_type output_geometry_type;
                typedef typename PartitionSelector::input_geometry_type  input_geometry_type;

                input_geometry_type* first, last;
                std::pair< input_geometry_type*, input_geometry_type* > pRange( first, last );
                output_geometry_type g = (*pSelector)( pRange );
            }
        };

        //! A strategy to select the first geometry type as the partitioning geometry.
        template <typename Segment>
        struct first_segment_selector_policy
        {
            typedef Segment output_geometry_type;
            typedef Segment input_geometry_type;

            template <typename Range>
            output_geometry_type operator()( const Range& r ) const
            {
                return *boost::begin( r );
            }
        };

        //! A strategy to select the first geometry type as the partitioning geometry.
        template <typename Segment, typename RandomIntegerGenerator = random_integer_generator<> >
        struct random_segment_selector_policy
        {
            typedef Segment output_geometry_type;
            typedef Segment input_geometry_type;

            random_segment_selector_policy( const RandomIntegerGenerator& rnd = RandomIntegerGenerator() )
                : m_rnd( rnd )
            {}

            template <typename Range>
            output_geometry_type operator()( const Range& r ) const
            {
                std::vector<input_geometry_type> sV( boost::begin( r ), boost::end( r ) );
#ifdef STK_NO_CXX17_STD_SHUFFLE
                std::random_shuffle( sV.begin(), sV.end(), m_rnd );
#else
                std::shuffle( sV.begin(), sV.end(), m_rnd );
#endif
                return *sV.begin();
            }

            RandomIntegerGenerator m_rnd;
        };

        //! A strategy to select the first geometry type as the partitioning geometry.
        template <typename Segment, typename NumberComparisonPolicy>
        struct scored_segment_selector_policy
        {
            typedef Segment output_geometry_type;
            typedef Segment input_geometry_type;
            enum classification
            {
                e_crosses,
                e_positive,
                e_negative,
                e_coincident
            };

            scored_segment_selector_policy( const NumberComparisonPolicy& compare = NumberComparisonPolicy() )
                : m_compare( compare )
            {}

            template <typename Range>
            output_geometry_type operator()( const Range& r ) const
            {
                // Blend factor for optimizing for balance or splits (should be tweaked)
                const double K = 0.8;
                // Variables for tracking best splitting plane seen so far
                output_geometry_type bestPlane;
                double bestScore = (std::numeric_limits<double>::max)();
                // Try the plane of each polygon as a dividing plane

                typedef typename boost::range_iterator< const Range >::type const_iterator;
                const_iterator first( boost::begin( r ) ), last( boost::end( r ) );
                for( const_iterator i( first ); i != last; ++i )
                {
                    int numInFront = 0, numBehind = 0, numStraddling = 0;
                    output_geometry_type plane = *i;

                    // Test against all other polygons
                    for( const_iterator j ( first ); j != last; ++j )
                    {
                        // Ignore testing against self
                        if(i == j)
                            continue;
                        // Keep standing count of the various poly-plane relationships
                        switch( classify( *j, plane ) )
                        {
                        case e_coincident:
                            /* Coplanar polygons treated as being in front of plane */
                        case e_positive:
                            ++numInFront;
                            break;
                        case e_negative:
                            ++numBehind;
                            break;
                        case e_crosses:
                            ++numStraddling;
                            break;
                        }
                    }

                    // Compute score as a weighted combination (based on K, with K in range
                    // 0..1) between balance and splits (lower score is better)
                    double score = K * numStraddling + (1.0 - K) * std::abs(numInFront - numBehind);
                    if( score < bestScore )
                    {
                        bestScore = score;
                        bestPlane = plane;
                    }
                }

                return bestPlane;
            }

            classification  classify( const Segment& splittingLine, const Segment& edge ) const
            {
                typedef Segment                                              segment_type;
                typedef typename geometric_traits<segment_type>::point_type  point_type;

                orientation_type orientation_start = get_orientation( get_start( splittingLine ), get_end( splittingLine ), get_start( edge ), m_compare );
                orientation_type orientation_end = get_orientation( get_start( splittingLine ), get_end( splittingLine ), get_end( edge ), m_compare );

                if( (orientation_start == oriented_left && orientation_end == oriented_right ) ||
                    (orientation_start == oriented_right && orientation_end == oriented_left ) )
                {
                    //! TODO: Don't really need the intersection here.. so simplify this.
                    point_type xPoint;
                    intersection_type iType = line_segment_intersect( get_start( splittingLine ), get_end( splittingLine ), edge, xPoint, m_compare );
                    if( iType == e_crossing )
                    {
                        if( orientation_end == oriented_left )
                        {
                            if( numeric_sequence_equals( get_start( edge ), xPoint, m_compare ) )
                            {
                                return e_positive;
                            }
                            else if( numeric_sequence_equals( get_end( edge ), xPoint, m_compare ) )
                            {
                                return e_negative;
                            }
                        }
                        else
                        {
                            if( numeric_sequence_equals( get_start( edge ), xPoint, m_compare ) )
                            {
                                return e_negative;
                            }
                            else if( numeric_sequence_equals( get_end( edge ), xPoint, m_compare ) )
                            {
                                return e_positive;
                            }
                        }

                        return e_crosses;
                    }
                    else if( iType == e_overlapping )
                    {
                        return e_coincident;
                    }
                }

                if( orientation_start == oriented_left || orientation_end == oriented_left )
                    return e_positive;
                else if( orientation_start == oriented_right || orientation_end == oriented_right )
                    return e_negative;
                else
                    return e_coincident;
            }

            NumberComparisonPolicy m_compare;
        };
    }

    template <typename Segment>
    class node_bsp_tree_2d : boost::noncopyable
    {
        using point_type = typename point_type_of<Segment>::type;
        using length_type = typename arithmetic_type_of<Segment>::type;
        using line_type = typename result_of::make_line<Segment>::type;

    public:

        node_bsp_tree_2d()
            : m_bounds(construct<point_type>(constants::infinity<length_type>(), constants::infinity<length_type>()), construct<point_type>(constants::negative_infinity<length_type>(), constants::negative_infinity<length_type>()))
        {}

        template <typename Range, typename PartitionSelector, typename NumberComparisonPolicy>
        node_bsp_tree_2d( const Range& range, const PartitionSelector& selector, const NumberComparisonPolicy& compare );

        //! Destructor
        ~node_bsp_tree_2d() = default;

        //! Method to detect if a point is inside, on the boundary our outside the shape represented by the partition.
        template <typename Point, typename NumberComparisonPolicy>
        point_location_classification               locate_point( const Point& point, const NumberComparisonPolicy& compare ) const;

        //! Method to to a 'painters algorithm' traversal of the BSP for a specified point.
        template <typename Point, typename Visitor, typename NumberComparisonPolicy>
        void                                        painters_traversal( const Point& point, Visitor&& visitor, const NumberComparisonPolicy& compare ) const;

        //! Method to get a partition of a line segment
        template <typename NumberComparisonPolicy>
        void                                        get_partition( const Segment& edge,
                                                                   std::vector< Segment >& positive,
                                                                   std::vector< Segment >& negative,
                                                                   std::vector< Segment >& coincidentSame,
                                                                   std::vector< Segment >& coincidentDiff,
                                                                   const NumberComparisonPolicy& compare ) const;

        //! Method to return a negated (edges reversed) copy of the bsp tree.
        boost::scoped_ptr< node_bsp_tree_2d< Segment > > negation() const;

        template <typename Range, typename PartitionSelector, typename NumberComparisonPolicy>
        void insert( const Range& range, const PartitionSelector& selector, const NumberComparisonPolicy& compare )
        {
            std::vector< Segment > posList, negList;

            typename boost::range_iterator< const Range >::type first( boost::begin( range ) ), last( boost::end( range ) );
            while( first != last )
            {
                using point_t = typename geometric_traits<Segment>::point_type;

                const Segment& segment = *first++;

                auto nbounds = get_bounds(std::array<point_t, 2>{ get_start(segment), get_end(segment) }, compare);
                m_bounds.expand(nbounds);

                Segment subNeg, subPos;
                classification type = classify( m_splittingSegment, segment, subPos, subNeg, compare );

                if( type == e_crosses )
                {
                    posList.push_back( subPos );
                    negList.push_back( subNeg );
                }
                else if( type == e_positive )
                {
                    posList.push_back( segment );
                }
                else if( type == e_negative )
                {
                    negList.push_back( segment );
                }
                else
                {
                    m_coincidentEdges.push_back( segment );
                }
            }

            if( !posList.empty() )
            {
                if( !m_positiveChild )
                    m_positiveChild.reset( new node_bsp_tree_2d( posList, selector, compare ) );
                else
                    m_positiveChild->insert( posList, selector, compare );
            }

            if( !negList.empty() )
            {
                if( !m_negativeChild )
                    m_negativeChild.reset( new node_bsp_tree_2d( negList, selector, compare ) );
                else
                    m_negativeChild->insert( negList, selector, compare );
            }
        }

        bool is_leaf() const { return m_negativeChild == nullptr && m_positiveChild == nullptr; }
        const line_type& get_cutting_plane() const { return m_cuttingPlane; }
/*
        template <typename Point, typename RayVector, typename NumberComparisonPolicy>
        bool intersection(const Point& o, const RayVector& v, const NumberComparisonPolicy& cmp) const
        {
            using area_t = decltype(std::declval<length_type>() * std::declval<length_type>());

            length_type minL, maxL;
            point_type q0, q1;
            auto iResult = ray_aabb_intersection(o, v, m_bounds, minL, q0, maxL, q1, cmp);

            using node_t = decltype(this);

            using elem_t = std::tuple<node_t, area_t, area_t>;
            std::stack<elem_t> stack = { elem_t{ this, minL, maxL } };

            node_t node;

            while (!stack.empty())
            {
                std::tie(node, minL, maxL) = stack.top();
                stack.pop();
                if (node == nullptr)
                    return false;
                if (node->is_leaf())
                {
                    return false;//?
                }

                auto dist = constants::zero<length_type>();
                auto l = node->get_cutting_plane();
                ray_line_intersection(o, v, l, dist, q0, cmp);
            }

            return false;
        }
*/
    private:

        enum classification
        {
            e_crosses,
            e_positive,
            e_negative,
            e_coincident
        };

        ///Method to classify the characterization of the splitting line and an edge (left,right or split).
        template <typename NumberComparisonPolicy>
        classification  classify( const Segment& splittingLine,
                                  const Segment& edge,
                                  Segment& subPos,
                                  Segment& subNeg,
                                  const NumberComparisonPolicy& compare ) const;

        //! Method to get a positive partition of a line segment
        template <typename NumberComparisonPolicy>
        void get_positive_partition( const Segment& edge,
                                     std::vector< Segment >& positive,
                                     std::vector< Segment >& negative,
                                     std::vector< Segment >& coincidentSame,
                                     std::vector< Segment >& coincidentDifferent,
                                     const NumberComparisonPolicy& compare ) const;

        //! Method to get a negative partition of a line segment
        template <typename NumberComparisonPolicy>
        void get_negative_partition( const Segment& edge,
                                     std::vector< Segment >& positive,
                                     std::vector< Segment >& negative,
                                     std::vector< Segment >& coincidentSame,
                                     std::vector< Segment >& coincidentDifferent,
                                     const NumberComparisonPolicy& compare) const;

        axis_aligned_bounding_box<point_type>        m_bounds;
        Segment                                      m_splittingSegment;
        line_type                                    m_cuttingPlane;
        std::vector<Segment>                         m_coincidentEdges;
        boost::scoped_ptr< node_bsp_tree_2d< Segment > >  m_positiveChild;
        boost::scoped_ptr< node_bsp_tree_2d< Segment > >  m_negativeChild;
    };

    template <typename Segment>
    template <typename Range, typename PartitionSelector, typename NumberComparisonPolicy>
    inline node_bsp_tree_2d< Segment >::node_bsp_tree_2d( const Range& range, const PartitionSelector& selector, const NumberComparisonPolicy& compare )
        : m_bounds(construct<point_type>(constants::infinity<length_type>(), constants::infinity<length_type>()), construct<point_type>(constants::negative_infinity<length_type>(), constants::negative_infinity<length_type>()))
        , m_splittingSegment(selector(range))
        , m_cuttingPlane(make_line(m_splittingSegment))
    {
        std::vector< Segment > posList, negList;

        typename boost::range_iterator< const Range >::type first( boost::begin( range ) ), last( boost::end( range ) );
        while( first != last )
        {
            using point_t = typename geometric_traits<Segment>::point_type;
            const Segment& segment = *first++;

            auto nbounds = get_bounds(std::array<point_t, 2>{ get_start(segment), get_end(segment) }, compare);
            m_bounds.expand(nbounds);

            Segment subNeg, subPos;
            classification type = classify( m_splittingSegment, segment, subPos, subNeg, compare );

            if( type == e_crosses )
            {
                posList.push_back( subPos );
                negList.push_back( subNeg );
            }
            else if( type == e_positive )
            {
                posList.push_back( segment );
            }
            else if( type == e_negative )
            {
                negList.push_back( segment );
            }
            else
            {
                m_coincidentEdges.push_back( segment );
            }
        }

        if( !posList.empty() )
            m_positiveChild.reset( new node_bsp_tree_2d( posList, selector, compare ) );

        if( !negList.empty() )
            m_negativeChild.reset( new node_bsp_tree_2d( negList, selector, compare ) );
    }

    template <typename Segment>
    template <typename NumberComparisonPolicy>
    inline typename node_bsp_tree_2d< Segment >::classification  node_bsp_tree_2d< Segment >::classify( const Segment& splittingLine,
                                                                                              const Segment& edge,
                                                                                              Segment& subPos,
                                                                                              Segment& subNeg,
                                                                                              const NumberComparisonPolicy& compare ) const
    {
        typedef Segment                                              segment_type;
        typedef typename geometric_traits<segment_type>::point_type  point_type;

        orientation_type orientation_start = get_orientation( get_start( splittingLine ), get_end( splittingLine ), get_start( edge ), compare );
        orientation_type orientation_end = get_orientation( get_start( splittingLine ), get_end( splittingLine ), get_end( edge ), compare );

        if( (orientation_start == oriented_left && orientation_end == oriented_right ) ||
            (orientation_start == oriented_right && orientation_end == oriented_left ) )
        {
            point_type xPoint;
            intersection_type iType = line_segment_intersect( get_start( splittingLine ), get_end( splittingLine ), edge, xPoint, compare );
            if( iType == e_crossing )
            {
                if( orientation_end == oriented_left )
                {
                    if( numeric_sequence_equals( get_start( edge ), xPoint, compare ) )
                    {
                        return e_positive;
                    }
                    else if( numeric_sequence_equals( get_end( edge ), xPoint, compare ) )
                    {
                        return e_negative;
                    }
                    else
                    {
                        subNeg = construct<Segment>( get_start( edge ), xPoint );
                        subPos = construct<Segment>( xPoint, get_end( edge ) );
                    }
                }
                else
                {
                    if( numeric_sequence_equals( get_start( edge ), xPoint, compare ) )
                    {
                        return e_negative;
                    }
                    else if( numeric_sequence_equals( get_end( edge ), xPoint, compare ) )
                    {
                        return e_positive;
                    }
                    else
                    {
                        subPos = construct<Segment>( get_start( edge ), xPoint );
                        subNeg = construct<Segment>( xPoint, get_end( edge ) );
                    }
                }

                return e_crosses;
            }
            else if( iType == e_overlapping )
            {
                return e_coincident;
            }
        }

        if( orientation_start == oriented_left || orientation_end == oriented_left )
            return e_positive;
        else if( orientation_start == oriented_right || orientation_end == oriented_right )
            return e_negative;
        else
            return e_coincident;
    }

    template <typename Segment>
    template <typename Point, typename NumberComparisonPolicy>
    inline point_location_classification node_bsp_tree_2d< Segment >::locate_point( const Point& point, const NumberComparisonPolicy& compare ) const
    {
        orientation_type orientation_point = get_orientation( get_start( m_splittingSegment ), get_end( m_splittingSegment ), point, compare );

        if( orientation_point == oriented_left )
        {
            if( m_positiveChild != 0 )
                return m_positiveChild->locate_point( point, compare );
            else
                return e_inside;
        }
        else if( orientation_point == oriented_right )
        {
            if( m_negativeChild != 0 )
                return m_negativeChild->locate_point( point, compare );
            else
                return e_outside;
        }
        else
        {
            for( const Segment& segment: m_coincidentEdges )
            {
                if( is_between( get_start( segment ), get_end( segment ), point, true, compare ) )
                    return e_boundary;
            }

            if( m_positiveChild != 0 )
            {
                return m_positiveChild->locate_point( point, compare );
            }
            else if( m_negativeChild != 0 )
            {
                return m_negativeChild->locate_point( point, compare );
            }
            else
            {
                ///This case should not happen.. but may due to numerical errors. The algorithms suggest noting this in debug mode but returning boundary in
                ///release (most likely due to round-off on a nearly collinear point.
                //! GEOMETRIX_ASSERT( false ); //! This case can happen if testing against a polyline.
                return e_boundary;
            }
        }
    }

    ///This concept is used to check if the edge properties conform to a numeric weight
    ///type which can be used in graph calculations.
    template <typename Visitor, typename VisitedType>
    struct VisitorConcept
    {
        BOOST_CONCEPT_USAGE(VisitorConcept)
        {
            Visitor* visitor = 0;
            VisitedType* visitee = 0;
            (*visitor)( *visitee ); ///needs to be able to call this.
        }
    };

    template <typename Segment>
    template <typename Point, typename Visitor, typename NumberComparisonPolicy>
    inline void node_bsp_tree_2d< Segment >::painters_traversal( const Point& point, Visitor&& visitor, const NumberComparisonPolicy& compare ) const
    {
        BOOST_CONCEPT_ASSERT((VisitorConcept<Visitor,Segment>));
        typedef Segment segment_type;

        if( m_negativeChild == 0 && m_positiveChild == 0 )
        {
            for( const segment_type& segment: m_coincidentEdges )
            {
                visitor( segment );
            }
        }
        else
        {
            orientation_type orientation_point = get_orientation( get_start(m_splittingSegment), get_end(m_splittingSegment), point, compare );

            if( orientation_point == oriented_left )
            {
                if( m_negativeChild != 0 )
                    m_negativeChild->painters_traversal( point, visitor, compare );

                for( const segment_type& segment: m_coincidentEdges )
                {
                    visitor( segment );
                }

                if( m_positiveChild != 0 )
                    m_positiveChild->painters_traversal( point, visitor, compare );
            }
            else if( orientation_point == oriented_right )
            {
                if( m_positiveChild != 0 )
                    m_positiveChild->painters_traversal( point, visitor, compare );

                for( const segment_type& segment: m_coincidentEdges )
                {
                    visitor( segment );
                }

                if( m_negativeChild != 0 )
                    m_negativeChild->painters_traversal( point, visitor, compare );
            }
            else
            {
                if( m_positiveChild != 0 )
                    m_positiveChild->painters_traversal( point, visitor, compare );
                if( m_negativeChild != 0 )
                    m_negativeChild->painters_traversal( point, visitor, compare );
            }
        }
    }

    template <typename Segment>
    inline boost::scoped_ptr< node_bsp_tree_2d< Segment > > node_bsp_tree_2d< Segment >::negation() const
    {
        boost::scoped_ptr< node_bsp_tree_2d< Segment > > pNegatedTree( new node_bsp_tree_2d< Segment >() );

        for( const Segment& edge: m_coincidentEdges )
        {
            Segment reversedEdge = construct( get_end( edge ), get_start( edge ) );
            pNegatedTree.m_coincidentEdges.push_back( reversedEdge );
        }

        if( m_positiveChild )
            pNegatedTree.m_negativeChild = m_positiveChild->negation();

        if( m_negativeChild )
            pNegatedTree.m_positiveChild = m_negativeChild->negation();

        return pNegatedTree;
    }

    //! Method to get a partition of a line segment
    template <typename Segment>
    template <typename NumberComparisonPolicy>
    inline void node_bsp_tree_2d< Segment >::get_partition( const Segment& edge,
                                                       std::vector< Segment >& positive,
                                                       std::vector< Segment >& negative,
                                                       std::vector< Segment >& coincidentSame,
                                                       std::vector< Segment >& coincidentDifferent,
                                                       const NumberComparisonPolicy& compare ) const
    {
        typedef Segment                                                segment_type;
        typedef typename geometric_traits<segment_type>::point_type    point_type;
        typedef typename geometric_traits<point_type>::arithmetic_type arithmetic_type;

        Segment subNeg, subPos;
        classification type = classify( m_splittingSegment, edge, subPos, subNeg );
        if( type == e_crosses )
        {
            get_positive_partition( subPos, positive, negative, coincidentSame, coincidentDifferent, compare );
            get_negative_partition( subNeg, positive, negative, coincidentSame, coincidentDifferent, compare );
        }
        else if( type == e_positive )
        {
            get_positive_partition( edge, positive, negative, coincidentSame, coincidentDifferent, compare );
        }
        else if( type == e_negative )
        {
            get_negative_partition( edge, positive, negative, coincidentSame, coincidentDifferent, compare );
        }
        else
        {
            typedef std::set< Segment, lexicographical_segment_compare< NumberComparisonPolicy > > UniqueSegmentSet;
            lexicographical_segment_compare< NumberComparisonPolicy > lexSegCompare( compare );
            UniqueSegmentSet overlappingSegments( lexSegCompare );
            typedef std::set< Segment, segment_interval_compare< NumberComparisonPolicy > > IntervalSegmentSet;
            segment_interval_compare< NumberComparisonPolicy > intervalSegCompare( compare );
            IntervalSegmentSet nonoverlappingSegments( intervalSegCompare );

            overlappingSegments.insert( edge );
            const point_type& edgeStart = get_start( edge );
            const point_type& edgeEnd  = get_end( edge );
            bool noneOverlaps = true;
            for( const Segment& edgeC: m_coincidentEdges )
            {
                std::vector< Segment > overlapping;
                for( const Segment edgeA: overlappingSegments )
                {
                    point_type xPoints[2];
                    intersection_type iType = segment_segment_intersection( edgeC, edgeA, xPoints, compare );
                    if( iType == e_overlapping )
                    {
                        noneOverlaps = false;
                        Segment AB = construct<Segment>( xPoints[0], xPoints[1] );
                        overlapping.push_back( AB );
                    }
                }
                overlappingSegments.insert( overlapping.begin(), overlapping.end() );
            }

            if( noneOverlaps )
            {
                //clear edge from the overlappingSegments set.
                overlappingSegments.clear();
            }

            for( const Segment& edgeA: overlappingSegments )
            {
               //! Instead of using arctan2 and thus complicating user defined types... I am going to attempt using a comparison of slopes
               //! Compare the delta x and y to see sign difference.
               arithmetic_type splitDeltaY = get<1>( get_start( m_splittingSegment ) ) - get<1>( get_end( m_splittingSegment ) );
               arithmetic_type edgeADeltaY = get<1>( get_start( edgeA ) ) - get<1>( get_end( edgeA ) );

               arithmetic_type splitDeltaX = get<0>( get_start( m_splittingSegment ) ) - get<0>( get_end( m_splittingSegment ) );
               arithmetic_type edgeADeltaX = get<0>( get_start( edgeA ) ) - get<0>( get_end( edgeA ) );

               //Check if opposite directions
               if( compare.less_than( splitDeltaY * edgeADeltaY, arithmetic_type( 0 ) ) || compare.less_than( splitDeltaX * edgeADeltaX, arithmetic_type( 0 ) ) )
                   coincidentDifferent.push_back( edgeA );
               else
                   coincidentSame.push_back( edgeA );
            }

            //! Construct segments on edge which do not overlap with any in m_coincident
            nonoverlappingSegments.insert( edge );
            for( const Segment& AB: overlappingSegments )
            {
                if( nonoverlappingSegments.empty() )
                    break;

                collinear_segment_difference( nonoverlappingSegments, AB, compare );
            }

            for( const Segment s: nonoverlappingSegments )
            {
                get_positive_partition( s, positive, negative, coincidentSame, coincidentDifferent, compare );
                get_negative_partition( s, positive, negative, coincidentSame, coincidentDifferent, compare );
            }
        }
    }

    //! Method to get a partition of a line segment
    template <typename Segment>
    template <typename NumberComparisonPolicy>
    inline void node_bsp_tree_2d< Segment >::get_positive_partition( const Segment& edge,
                                                                std::vector< Segment >& positive,
                                                                std::vector< Segment >& negative,
                                                                std::vector< Segment >& coincidentSame,
                                                                std::vector< Segment >& coincidentDifferent,
                                                                const NumberComparisonPolicy& compare ) const
    {
        if( m_positiveChild )
            m_positiveChild->get_partition( edge, positive, negative, coincidentSame, coincidentDifferent, compare );
        else
            positive.push_back( edge );
    }

    //! Method to get a partition of a line segment
    template <typename Segment>
    template <typename NumberComparisonPolicy>
    inline void node_bsp_tree_2d< Segment >::get_negative_partition( const Segment& edge,
                                                                std::vector< Segment >& positive,
                                                                std::vector< Segment >& negative,
                                                                std::vector< Segment >& coincidentSame,
                                                                std::vector< Segment >& coincidentDifferent,
                                                                const NumberComparisonPolicy& compare ) const
    {
        if( m_negativeChild )
            m_negativeChild->get_partition( edge, positive, negative, coincidentSame, coincidentDifferent, compare );
        else
            negative.push_back( edge );
    }

}//namespace geometrix;

#endif //GEOMETRIX_BSPTREE2D_HPP
