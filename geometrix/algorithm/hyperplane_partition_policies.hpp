//
//! Copyright © 2017
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_HYPERPLANE_PARTITION_POLICY_HPP
#define GEOMETRIX_HYPERPLANE_PARTITION_POLICY_HPP
#pragma once

#include <geometrix/utility/random_generator.hpp>
#include <geometrix/primitive/hyperplane_traits.hpp>

namespace geometrix {
    namespace partition_policies {
        
        namespace result_of {
            template <typename T>
            struct autopartition_policy
            {
            private:

                using iterator_type = typename boost::range_iterator<T>::type;
				
            public:
            
				using type = iterator_type;
            
            };
        }//! namespace result_of;

        //! A strategy to select the first geometry type as the partitioning geometry.
        struct autopartition_policy
        {
            template <typename Range>
            typename result_of::autopartition_policy<Range>::type operator()( Range& r, boost::dynamic_bitset<>& usedBits ) const
            {
				std::size_t i = 0;
				for (auto it = boost::begin(r); it != boost::end(r); ++it, ++i)
				{
					if (!usedBits[i])
					{
						usedBits.set(i);
						return it;
					}
				}

				return boost::end(r);
            }
        };
        
        namespace result_of {
            template <typename T>
            struct random_partition_selector_policy : autopartition_policy<T>
            {};
        }//! namespace result_of;

        //! A strategy to select the first geometry type as the partitioning geometry.
        template <typename RNG = boost::mt19937>
        struct random_partition_selector_policy
        {
            random_partition_selector_policy( const RNG& rnd = RNG() )
                : m_rnd( rnd )
            {}

            template <typename Range>
            typename result_of::random_partition_selector_policy<Range>::type operator()( Range& r ) const
            {
                random_index_generator<RNG> rnd(boost::size(r), m_rnd);
				auto it = random_element(r, rnd);
                return it;
            }

            RNG m_rnd;
        };

        namespace result_of {
            template <typename T>
            struct scored_segment_selector_policy : autopartition_policy<T>
            {};
        }//! namespace result_of;
        /*
        //! A strategy to select the first geometry type as the partitioning geometry.
        template <typename NumberComparisonPolicy>
        struct scored_segment_selector_policy
        {
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
            typename result_of::scored_segment_selector_policy<Range>::type operator()( const Range& r ) const
            {
                using plane_t = typename result_of::scored_segment_selector_policy<Range>::type;
                // Blend factor for optimizing for balance or splits (should be tweaked)
                const double K = 0.8;
                // Variables for tracking best splitting plane seen so far
                plane_t bestPlane;
                double bestScore = (std::numeric_limits<double>::max)();
                // Try the plane of each polygon as a dividing plane

                typedef typename boost::range_iterator< const Range >::type const_iterator;
                const_iterator first( boost::begin( r ) ), last( boost::end( r ) );
                for( const_iterator i( first ); i != last; ++i )
                {
                    int numInFront = 0, numBehind = 0, numStraddling = 0;
                    auto plane = make_hyperplane(*i);

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
                            // Coplanar polygons treated as being in front of plane
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
		*/
    }
}//! namespace geometrix;

#endif//! GEOMETRIX_HYPERPLANE_PARTITION_POLICY_HPP
