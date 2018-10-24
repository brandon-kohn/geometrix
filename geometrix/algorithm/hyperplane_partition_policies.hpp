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
#include <boost/dynamic_bitset.hpp>

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
            struct scored_selector_policy : autopartition_policy<T>
            {};
        }//! namespace result_of;
        
        template <typename Extractor, typename NumberComparisonPolicy>
        struct scored_selector_policy
        {
            scored_selector_policy( const Extractor& extractor, const NumberComparisonPolicy& compare = NumberComparisonPolicy() )
            : m_extract(extractor)
            , m_compare( compare )
            {}

            template <typename Range>
            typename result_of::scored_selector_policy<Range>::type operator()( Range& r, boost::dynamic_bitset<>& usedBits ) const
            {
                // Blend factor for optimizing for balance or splits (should be tweaked)
                const double K = 0.8;
                
                // Variables for tracking best splitting plane seen so far
                auto bestScore = (std::numeric_limits<double>::max)();
                auto bestIndex = static_cast<std::size_t>(-1);
                
                // Try the plane of each polygon as a dividing plane
                auto first = boost::begin( r );                
                auto last = boost::end( r );
                
                auto best = last;
                std::size_t index = 0;
                for( auto i = first; i != last; ++i, ++index )
                {
                    if( !usedBits[index] )
                    {
                        int numInFront = 0, numBehind = 0, numStraddling = 0;
                        auto split = make_hyperplane(m_extract(*i));

                        // Test against all other polygons
                        for( auto j = first; j != last; ++j )
                        {
                            // Ignore testing against self
                            if(i == j)
                                continue;
                            // Keep standing count of the various poly-plane relationships
                            switch( classify_simplex_to_plane( m_extract(*j), split, m_compare ) )
                            {
                            case plane_orientation::coplanar_with_plane:
                                // Coplanar polygons treated as being in front of plane
                            case plane_orientation::in_front_of_plane:
                                ++numInFront;
                                break;
                            case plane_orientation::in_back_of_plane:
                                ++numBehind;
                                break;
                            case plane_orientation::straddling_plane:
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
                            bestIndex = index;
                            best = i;
                        }
                    }
                }
                
                if( best != last )
                    usedBits.set(bestIndex);
                return best;
            }

            Extractor m_extract;
            NumberComparisonPolicy m_compare;
        };
    }
}//! namespace geometrix;

#endif//! GEOMETRIX_HYPERPLANE_PARTITION_POLICY_HPP
