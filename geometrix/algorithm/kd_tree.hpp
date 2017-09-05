//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_KD_TREE_HPP
#define GEOMETRIX_KD_TREE_HPP
#pragma once

#include <boost/smart_ptr.hpp>
#include <geometrix/primitive/axis_aligned_bounding_box.hpp>
#include <geometrix/primitive/point_sequence_utilities.hpp>

namespace geometrix {
    //! \brief A data structure used to store a set of points in N-dimensional space with search query functionality.

    //! A kd_tree may be used to perform queries on points within an N-dimensional orthogonal bound. The run-time complexity
    //! of queries is \f$O(\sqrt{n}*k)\f$ where \f$n\f$ is the total number of points in the tree and \f$k\f$ is the number of points found
    //! in the query.
    //! Example usage:
    //! \code
    //! using namespace geometrix;
    //!
    //! typedef point_double_3D point_3D;
    //! std::vector< point_3D > points;
    //! random_real_generator< boost::mt19937 > rnd(10.0);
    //! fraction_tolerance_comparison_policy<double> compare(1e-10);
    //!
    //! for( std::size_t i=0;i < 1000; ++i )
    //! {
    //!     double x = rnd();
    //!     double y = rnd();
    //!     double z = rnd();
    //!     points.insert( point_3D( x, y, z ) );
    //! }
    //!
    //! kd_tree< point_3D > tree( points, compare, median_partitioning_strategy() );
    //!
    //! //! Specify a volume (box) with diagonal vector from 0,0,0, to 5,5,5 for the search range.
    //! axis_aligned_bounding_box< point_3D > range( point_3D( 0.0, 0.0, 0.0 ), point_3D( 5.0, 5.0, 5.0 ) );
    //!
    //! //! Visit all the points inside the volume.
    //! point_visitor visitor;
    //! tree.search( range, visitor, compare );
    //! \endcode
    template <typename NumericSequence>
    class kd_tree
    {
    public:

        typedef NumericSequence                                             sequence_type;
        typedef typename geometric_traits< sequence_type >::dimension_type  dimension_type;
        typedef typename geometric_traits< sequence_type >::arithmetic_type numeric_type;

        template <typename PointSequence, typename NumberComparisonPolicy, typename PartitionStrategy>
        kd_tree( const PointSequence& pSequence, const NumberComparisonPolicy& compare, const PartitionStrategy& partitionStrategy, typename boost::enable_if< is_point_sequence< PointSequence > >::type* =0 )
            : m_region( make_aabb<NumericSequence>(pSequence) )
        {
            build( pSequence, compare, partitionStrategy );
        }

        //! Traverse the tree on a range and visit all leaves in the specified range.
        template <typename NumericSequence, typename Visitor, typename NumberComparisonPolicy>
        void search( const axis_aligned_bounding_box<NumericSequence>& range, Visitor&& visitor, const NumberComparisonPolicy& compare ) const
        {
            if( m_pLeaf )
                visitor( *m_pLeaf );
            else
                search<0>( range, visitor, compare );
        }

    private:

        kd_tree( const axis_aligned_bounding_box< sequence_type >& region )
            : m_region( region )
        {}

        template <typename NumericSequence, std::size_t Dimension, std::size_t D>
        friend struct kd_tree_builder;

        template <typename NumericSequence, std::size_t Dimension, std::size_t D>
        struct kd_tree_builder
        {
            template <typename NumericSequence>
            friend class kd_tree;

            template <typename NumberComparisonPolicy, typename PartitionStrategy>
            static void build_tree( boost::shared_ptr< kd_tree<NumericSequence> >& pTree, std::vector< NumericSequence >& pSequence, const NumberComparisonPolicy& compare, const PartitionStrategy& partitionStrategy )
            {
                std::size_t pSize = pSequence.size();
                if( pSize == 1 )
                {
                    pTree->m_pLeaf.reset( new sequence_type( pSequence[0] ) );
                }
                else
                {
                    std::size_t medianIndex = partitionStrategy.template partition<Dimension>( pSequence, compare );
                    pTree->m_median = get<Dimension>( pSequence[ medianIndex ] );

                    //! Split to the left tree those that are on left or collinear of line... and to the right those on the right.
                    std::vector< sequence_type > left( pSequence.begin(), pSequence.begin() + medianIndex );
                    std::vector< sequence_type > right( pSequence.begin() + medianIndex, pSequence.end() );

                    if( !left.empty() )
                    {
                        auto upperBound = construct<sequence_type>( pTree->m_region.get_upper_bound() );
                        set<Dimension>(upperBound, pTree->m_median);
                        pTree->m_pLeftChild.reset( new kd_tree<NumericSequence>( axis_aligned_bounding_box< sequence_type >( pTree->m_region.get_lower_bound(), upperBound ) ) );
                        kd_tree_builder<NumericSequence, (Dimension+1)%D, D>::build_tree( pTree->m_pLeftChild, left, compare, partitionStrategy );
                    }
                    if( !right.empty() )
                    {
                        auto lowerBound = construct<sequence_type>( pTree->m_region.get_lower_bound() );
                        set<Dimension>(lowerBound, pTree->m_median);
                        pTree->m_pRightChild.reset( new kd_tree<NumericSequence>( axis_aligned_bounding_box< sequence_type >( lowerBound, pTree->m_region.get_upper_bound() ) ) );
                        kd_tree_builder<NumericSequence, (Dimension+1)%D, D>::build_tree( pTree->m_pRightChild, right, compare, partitionStrategy );
                    }
                }
            }
        };

        template <typename PointSequence, typename NumberComparisonPolicy, typename PartitionStrategy>
        void build( const PointSequence& pSequence, const NumberComparisonPolicy& compare, const PartitionStrategy& partitionStrategy )
        {
            std::size_t pSize = point_sequence_traits< PointSequence >::size( pSequence );
            if( pSize == 1 )
            {
                m_pLeaf.reset( new sequence_type( point_sequence_traits< PointSequence >::get_point( pSequence, 0 ) ) );
            }
            else
            {
                std::vector< sequence_type > sortedSequence( point_sequence_traits< PointSequence >::begin( pSequence ), point_sequence_traits< PointSequence >::end( pSequence ) );

                std::size_t medianIndex = partitionStrategy.template partition<0>( sortedSequence, compare );
                m_median = get<0>( sortedSequence[ medianIndex ] );

                //! Split to the left tree those that are on left or collinear of line... and to the right those on the right.
                std::vector< sequence_type > left( sortedSequence.begin(), sortedSequence.begin() + medianIndex );
                std::vector< sequence_type > right( sortedSequence.begin() + medianIndex, sortedSequence.end() );

                if( !left.empty() )
                {
                    auto upperBound = construct<sequence_type>( m_region.get_upper_bound() );
                    set<0>(upperBound, m_median);
                    m_pLeftChild.reset( new kd_tree<NumericSequence>( axis_aligned_bounding_box< sequence_type >( m_region.get_lower_bound(), upperBound ) ) );
                    kd_tree_builder<NumericSequence, 1, dimension_type::value>::build_tree( m_pLeftChild, left, compare, partitionStrategy );
                }
                if( !right.empty() )
                {
                    auto lowerBound = construct<sequence_type>( m_region.get_lower_bound() );
                    set<0>(lowerBound, m_median);
                    m_pRightChild.reset(new kd_tree<NumericSequence>( axis_aligned_bounding_box< sequence_type >( lowerBound, m_region.get_upper_bound() ) ) );
                    kd_tree_builder<NumericSequence, 1, dimension_type::value>::build_tree( m_pRightChild, right, compare, partitionStrategy );
                }
            }
        }

        template <std::size_t Dimension, typename Visitor, typename NumberComparisonPolicy>
        void search( const axis_aligned_bounding_box<sequence_type>& range, Visitor&& visitor, const NumberComparisonPolicy& compare ) const
        {
            if( m_pLeaf )
            {
                visitor( *m_pLeaf );
                return;
            }

            //! Search the left child.
            if( m_pLeftChild )
            {
                if( range.contains( m_pLeftChild->m_region, compare ) )
                    m_pLeftChild->traverse_subtrees( visitor );
                else if( range.intersects( m_pLeftChild->m_region, compare ) )
                    m_pLeftChild->search<(Dimension+1)%dimension_type::value>( range, visitor, compare );
            }

            //! Search the right child.
            if( m_pRightChild )
            {
                if( range.contains( m_pRightChild->m_region, compare ) )
                    m_pRightChild->traverse_subtrees( visitor );
                else if( m_pRightChild && range.intersects( m_pRightChild->m_region, compare ) )
                    m_pRightChild->search<(Dimension+1)%dimension_type::value>( range, visitor, compare );
            }
        }

        template <typename Visitor>
        void traverse_subtrees( Visitor& v ) const
        {
            if( m_pLeaf )
            {
                v( *m_pLeaf );
                return;
            }

            if( m_pLeftChild )
                m_pLeftChild->traverse_subtrees( v );
            if( m_pRightChild )
                m_pRightChild->traverse_subtrees( v );
        }

        typedef boost::shared_ptr< kd_tree< sequence_type > > dimension_split;
        typedef sequence_type                                 leaf;
        typedef boost::shared_ptr< sequence_type >            leaf_ptr;
        numeric_type                                          m_median;
        dimension_split                                       m_pLeftChild;
        dimension_split                                       m_pRightChild;
        leaf_ptr                                              m_pLeaf;
        axis_aligned_bounding_box< sequence_type >            m_region;

    };

}//namespace geometrix;

#endif //GEOMETRIX_KD_TREE_HPP
